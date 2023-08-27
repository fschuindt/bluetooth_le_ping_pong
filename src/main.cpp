#include <Arduino.h>
#include <Wire.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "2bb6b258-6f36-46a7-85b4-238430e95f83"
#define CHARACTERISTIC_UUID "b31185d3-1e29-48c2-b062-f702ebf7eb6d"

class PingPong : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        Serial.println("Received WRITE from client.");

        if (value == "Ping") {
            pCharacteristic->setValue("Pong");
            pCharacteristic->notify();
            Serial.println("Sent 'Pong' back to client.");
        } else {
            pCharacteristic->setValue("Where is my ping?");
            pCharacteristic->notify();
            Serial.println("Sent 'Where is my Ping?' back to client.");
        }
    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Starting Bluetooth Low Energy.");

    BLEDevice::init("SmartLocks-PoC-v1");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pCharacteristic->setCallbacks(new PingPong());
    pService->start();
    pCharacteristic->setValue("Nothing yet.");

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("Characteristic defined.");
}

void loop() {
    delay(2000);
}
