#include <ArduinoBLE.h>

const int buttonPin = D1;
int oldButtonState = LOW;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(buttonPin, INPUT_PULLUP);
  BLE.begin();
  Serial.println("Bluetooth® Central - LED control");
  BLE.scanForName("XIAO Peripheral");
}

void loop() {
  BLEDevice peripheral = BLE.available();
  if (peripheral) {
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "XIAO Peripheral") {
      // Skip unknown devices
      return;
    }

    // Stop scanning
    BLE.stopScan();
    
    // Connect to the peripheral
    if (peripheral.connect()) {
      Serial.println("Connected to peripheral");
      // Discover services and characteristics
      if (peripheral.discoverAttributes()) {
        Serial.println("Discovered attributes");
      } else {
        Serial.println("Attributes not discovered");
      }
    } else {
      Serial.println("Failed to connect to peripheral");
      return;
    }

    // Get the characteristic for writing
    BLECharacteristic switchCharacteristic = peripheral.characteristic("19B10001-E8F2-537E-4F6C-D104768A1214");
    
    // Control loop
    while (peripheral.connected()) {
      int buttonState = digitalRead(buttonPin);

      // Button pressed
      if (buttonState == LOW && oldButtonState == HIGH) {
        // Toggle the LED
        int ledState = switchCharacteristic.value() ? 0 : 1; // Toggle value
        switchCharacteristic.writeValue(ledState);
        Serial.print("Sent LED state: ");
        Serial.println(ledState);
      }

      oldButtonState = buttonState; // Store button state
      delay(100); // Debounce delay
    }

    Serial.println("Disconnected from peripheral");
    peripheral.disconnect(); // Disconnect after use
    BLE.scanForName("XIAO Peripheral"); // Scan again for other devices
  }
}
