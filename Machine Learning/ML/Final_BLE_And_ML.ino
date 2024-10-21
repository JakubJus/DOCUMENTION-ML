#include <ArduinoBLE.h>
#include <Sitting_Standing_and_walking_test_inferencing.h>
#include <LSM6DS3.h>

// BLE Setup
BLEService radarService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Custom BLE service UUID
BLEStringCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 20); // 20-char limit

// IMU and ML Setup
#define CONVERT_G_TO_MS2    9.80665f
#define MAX_ACCEPTED_RANGE  2.0f
LSM6DS3 myIMU(I2C_MODE, 0x6A);

bool debug_nn = false;

void setup() {
    Serial.begin(115200);
    if (!myIMU.begin()) {
        Serial.println("Failed to initialize IMU!");
    } else {
        Serial.println("IMU initialized");
    }

    // BLE Initialization
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        while (1);
    }

    // Set local name and service UUID
    BLE.setLocalName("ML-Bluetooth");
    BLE.setAdvertisedService(radarService);

    // Add the characteristic to the service
    radarService.addCharacteristic(switchCharacteristic);

    // Add the service
    BLE.addService(radarService);

    // Start advertising
    BLE.advertise();
    Serial.println("Waiting for connections...");
}

void loop() {
    // BLE central device
    BLEDevice central = BLE.central();
    
    if (central) {
        Serial.print("Connected to central: ");
        Serial.println(central.address());

        // Continuous inferencing and BLE sending
        while (central.connected()) {
            runInferenceAndSendResult(); // Function to process ML and send results over BLE
        }

        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
    }
}

// Function to handle inferencing and BLE transmission
void runInferenceAndSendResult() {
    float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };

    // Collect data from IMU
    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
        uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);

        buffer[ix] = myIMU.readFloatAccelX();
        buffer[ix+1] = myIMU.readFloatAccelY();
        buffer[ix+2] = myIMU.readFloatAccelZ();

        for (int i = 0; i < 3; i++) {
            if (fabs(buffer[ix + i]) > MAX_ACCEPTED_RANGE) {
                buffer[ix + i] = (buffer[ix + i] >= 0.0) ? MAX_ACCEPTED_RANGE : -MAX_ACCEPTED_RANGE;
            }
        }

        buffer[ix + 0] *= CONVERT_G_TO_MS2;
        buffer[ix + 1] *= CONVERT_G_TO_MS2;
        buffer[ix + 2] *= CONVERT_G_TO_MS2;

        delayMicroseconds(next_tick - micros());
    }

    // Convert raw buffer to a signal
    signal_t signal;
    int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        Serial.println("Failed to create signal from buffer");
        return;
    }

    // Run classifier
    ei_impulse_result_t result = { 0 };
    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        Serial.println("Failed to run classifier");
        return;
    }

    // Find the classification with the highest value
    int max_index = 0;
    float max_value = result.classification[0].value;

    for (int i = 1; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > max_value) {
            max_index = i;
            max_value = result.classification[i].value;
        }
    }

    // Get the classification result label
    String classificationResult = result.classification[max_index].label;

    // Send result via BLE
    switchCharacteristic.setValue(classificationResult.c_str());
    Serial.print("Sent via BLE: ");
    Serial.println(classificationResult);

#if EI_CLASSIFIER_HAS_ANOMALY == 1
    Serial.print("Anomaly score: ");
    Serial.println(result.anomaly);
#endif
}
