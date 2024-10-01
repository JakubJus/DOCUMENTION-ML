#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // Pin for the LED

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  // Begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  // Set advertised local name and service UUID
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // Add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // Add service
  BLE.addService(ledService);

  // Set the initial value for the characteristic
  switchCharacteristic.writeValue(0);

  // Start advertising
  BLE.advertise();
  Serial.println("BLE LED Peripheral");
}

void loop() {
  // Listen for Bluetooth® Low Energy peripherals to connect
  BLEDevice central = BLE.central();

  // If a central is connected to peripheral
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // While the central is still connected
    while (central.connected()) {
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   
          Serial.println("LED on");
          digitalWrite(ledPin, LOW); // Turn LED on
        } else {                              
          Serial.println(F("LED off"));
          digitalWrite(ledPin, HIGH); // Turn LED off
        }
      }
    }

    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
