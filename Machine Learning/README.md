# Seeed Studio XIAO nRF52840 Sense with Edge Impulse

This project guides you through using the **Seeed Studio XIAO nRF52840 Sense** for human motion detection with Edge Impulse. For detailed setup instructions, visit the [Seeed Studio Wiki](https://wiki.seeedstudio.com/XIAOEI/).

## Hardware Requirements

- Seeed Studio XIAO nRF52840 Sense
- Li-Po Battery (702025)
- Dupont or Grove cables

## Software Requirements

1. **Node.js** (v12 or higher)
2. **Arduino CLI**
3. **Edge Impulse CLI**

To install the Edge Impulse CLI, run:
```bash
npm install -g edge-impulse-cli
```

## Compile and load you IMU with the following code to get raw data
```
// XIAO BLE Sense LSM6DS3 Accelerometer Raw Data 

#include "LSM6DS3.h"
#include "Wire.h"

//Create a instance of class LSM6DS3
LSM6DS3 myIMU(I2C_MODE, 0x6A);  //I2C device address 0x6A

#define CONVERT_G_TO_MS2 9.80665f
#define FREQUENCY_HZ 50
#define INTERVAL_MS (1000 / (FREQUENCY_HZ + 1))

static unsigned long last_interval_ms = 0;


void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  if (myIMU.begin() != 0) {
    Serial.println("Device error");
  } else {
    Serial.println("Device OK!");
  }
}



void loop() {
  if (millis() > last_interval_ms + INTERVAL_MS) {
    last_interval_ms = millis();
    Serial.print(myIMU.readFloatAccelX() * CONVERT_G_TO_MS2, 4);
    Serial.print('\t');
    Serial.print(myIMU.readFloatAccelY() * CONVERT_G_TO_MS2, 4);
    Serial.print('\t');
    Serial.println(myIMU.readFloatAccelZ() * CONVERT_G_TO_MS2, 4);
  }
}
```

Next:
1. Run the line below in CMD:
```
edge-impulse-data-forwarder
```
2. Go to edgeimpulse website and create a new project.
3. Go to "Data Acquisition" and connect new device
4. In the CMD, you can now choose the project you want to connect the device to
5. Data is now sent to the website
6. Choose a lable like "walking" "standing" etc. and start recording
7. Split the sample
8. Save
9. Go to "create impulse" and click save
10. Go to "Spectral features" and save parameters + train the model
11. Go to "Classifier" and classify all samples
12. Go to "Deployment" and choose "Arduino library" + export
13. Add the library to your Arduino sketch
## The code should look like this, change the "#include" with the name of the library you exported
```
/* Includes ---------------------------------------------------------------- */
#include <ML_test_inferencing.h>
#include <LSM6DS3.h>
/* Constant defines -------------------------------------------------------- */
#define CONVERT_G_TO_MS2    9.80665f
#define MAX_ACCEPTED_RANGE  2.0f        // starting 03/2022, models are generated setting range to +-2, but this example use Arudino library which set range to +-4g. If you are using an older model, ignore this value and use 4.0f instead

/*
 ** NOTE: If you run into TFLite arena allocation issue.
 **
 ** This may be due to may dynamic memory fragmentation.
 ** Try defining "-DEI_CLASSIFIER_ALLOCATION_STATIC" in boards.local.txt (create
 ** if it doesn't exist) and copy this file to
 ** `<ARDUINO_CORE_INSTALL_PATH>/arduino/hardware/<mbed_core>/<core_version>/`.
 **
 ** See
 ** (https://support.arduino.cc/hc/en-us/articles/360012076960-Where-are-the-installed-cores-located-)
 ** to find where Arduino installs cores on your machine.
 **
 ** If the problem persists then there's not enough memory for this model and application.
 */

/* Private variables ------------------------------------------------------- */
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
LSM6DS3 myIMU(I2C_MODE, 0x6A);
/**
* @brief      Arduino setup function
*/

const int RED_ledPin =  11;
const int BLUE_ledPin =  12;
const int GREEN_ledPin =  13; 

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    //u8g2.begin();
    Serial.println("Edge Impulse Inferencing Demo");

    //if (!IMU.begin()) {
      if (!myIMU.begin()) {
        ei_printf("Failed to initialize IMU!\r\n");
    }
    else {
        ei_printf("IMU initialized\r\n");
    }

    if (EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME != 3) {
        ei_printf("ERR: EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME should be equal to 3 (the 3 sensor axes)\n");
        return;
    }
}

/**
 * @brief Return the sign of the number
 * 
 * @param number 
 * @return int 1 if positive (or 0) -1 if negative
 */
float ei_get_sign(float number) {
    return (number >= 0.0) ? 1.0 : -1.0;
}

/**
* @brief      Get data and run inferencing
*
* @param[in]  debug  Get debug info if true
*/
void loop()
{
    uint8_t buf1[64]="idle";
    uint8_t buf2[64]="left&right";
    uint8_t buf3[64]="up&down";
    
  
    ei_printf("\nStarting inferencing in 2 seconds...\n");

    delay(2000);

    ei_printf("Sampling...\n");

    // Allocate a buffer here for the values we'll read from the IMU
    float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };

    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
        // Determine the next tick (and then sleep later)
        uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);

buffer[ix] = myIMU.readFloatAccelX();
buffer[ix+1] = myIMU.readFloatAccelY();
buffer[ix+2] = myIMU.readFloatAccelZ();

//buffer[ix] = myIMU.readFloatGyroX();
//buffer[ix+1] = myIMU.readFloatGyroY();
//buffer[ix+2] = myIMU.readFloatGyroZ();

        for (int i = 0; i < 3; i++) {
            if (fabs(buffer[ix + i]) > MAX_ACCEPTED_RANGE) {
                buffer[ix + i] = ei_get_sign(buffer[ix + i]) * MAX_ACCEPTED_RANGE;
            }
        }

        buffer[ix + 0] *= CONVERT_G_TO_MS2;
        buffer[ix + 1] *= CONVERT_G_TO_MS2;
        buffer[ix + 2] *= CONVERT_G_TO_MS2;

        delayMicroseconds(next_tick - micros());
    }

    // Turn the raw buffer in a signal which we can the classify
    signal_t signal;
    int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        ei_printf("Failed to create signal from buffer (%d)\n", err);
        return;
    }

    // Run the classifier
    ei_impulse_result_t result = { 0 };

    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", err);
        return;
    }

    // print the predictions
    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \n");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {

        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
        
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif

}
```
