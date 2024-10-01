#include "LSM6DS3.h"
#include "Wire.h"

// Create an instance of the LSM6DS3 class
LSM6DS3 myIMU(I2C_MODE, 0x6A);  // I2C device address 0x6A

#define CONVERT_G_TO_MS2 9.80665f  // Conversion factor from G to m/s²
#define FREQUENCY_HZ 50             // Frequency of data reading
#define INTERVAL_MS (1000 / (FREQUENCY_HZ + 1))  // Time interval in milliseconds

static unsigned long last_interval_ms = 0;

void setup() {
    Serial.begin(115200);  // Initialize serial communication
    while (!Serial);       // Wait for the serial port to connect

    // Initialize the IMU
    if (myIMU.begin() != 0) {
        Serial.println("Device error");
    } else {
        Serial.println("Device OK!");
    }
}

void loop() {
    // Read and print accelerometer data at specified intervals
    if (millis() > last_interval_ms + INTERVAL_MS) {
        last_interval_ms = millis();
        Serial.print(myIMU.readFloatAccelX() * CONVERT_G_TO_MS2, 4);  // Read X-axis
        Serial.print('\t');
        Serial.print(myIMU.readFloatAccelY() * CONVERT_G_TO_MS2, 4);  // Read Y-axis
        Serial.print('\t');
        Serial.println(myIMU.readFloatAccelZ() * CONVERT_G_TO_MS2, 4); // Read Z-axis
    }
}
