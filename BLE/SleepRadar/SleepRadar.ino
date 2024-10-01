#include <ArduinoBLE.h>
#include <sleepbreathingradarBLE.h>

SleepBreathingRadarBLE radar;
BLEService radarService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy Radar Service

// Bluetooth® Low Energy String Characteristic
BLEStringCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 20);

int last_val = 0;

void setup() {
  Serial.begin(9600);
  radar.SerialInit();
  while (!Serial);

  // Begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Seeed Studio XIAO nRF52840 with radar sensor demo failed!");
    while (1);
  }

  // Set advertised local name and service UUID
  BLE.setLocalName("Seeed Studio XIAO nRF52840");
  BLE.setAdvertisedService(radarService);

  // Add the characteristic to the service
  radarService.addCharacteristic(switchCharacteristic);

  // Add service
  BLE.addService(radarService);

  // Start advertising
  BLE.advertise();
  Serial.println("Waiting for connections...");
}

void loop() {
  // Listen for Bluetooth® Low Energy peripherals to connect
  BLEDevice central = BLE.central();

  // If a central is connected to peripheral
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // While the central is still connected
    while (central.connected()){
      radar.recvRadarBytes(); // Receive radar data
      if (radar.newData) {
        byte dataMsg[radar.dataLen + 3] = {0x00};
        dataMsg[0] = 0x53; // Add header frame
        for (byte n = 0; n < radar.dataLen; n++) dataMsg[n + 1] = radar.Msg[n];
        dataMsg[radar.dataLen + 1] = 0x54;
        dataMsg[radar.dataLen + 2] = 0x43;
        radar.newData = false; // Reset newData flag
        int new_val = radar.Sleep_inf(dataMsg); // Process data
        if (new_val != last_val) {
          radar.OutputAssignment(new_val);
          switchCharacteristic.setValue(radar.str);
          last_val = new_val;
        }
      }
    }

    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
