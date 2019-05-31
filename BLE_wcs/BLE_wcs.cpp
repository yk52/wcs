/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include "C:\Users\Joanna\Documents\Arduino\libraries\BLE_wcs\BLE_wcs.h"




void BLE_wcs::init_server()
{
	BLEDevice::init("Vitameter");
	BLEServer *pServer = BLEDevice::createServer();
	BLEService *pService = pServer->createService(SERVICE_UUID);
	BLECharacteristic *pCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID,
											 BLECharacteristic::PROPERTY_READ |
											 BLECharacteristic::PROPERTY_WRITE
										   );

	pCharacteristic->setValue("Hello World says Team Vita");
	pService->start();
	// BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(true);
	pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	pAdvertising->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
}

void BLE_wcs::init_client()
{
}

