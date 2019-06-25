/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include "C:\Users\Yumi\Desktop\wcs\BLE_wcs\BLE_wcs.h"




/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second. 
*/



BLECharacteristic *pCharacteristic;
bool deviceConnected = false;


std::string property = "default";
std::string value = "default";
bool messageReceived = false;

	std::string rxValue; 									//  global variable with received data


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
		deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
		deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *pCharacteristic) {
		
		rxValue = pCharacteristic->getValue();				// write received message to global variable rxValue
		messageReceived = true;								// set messageReceived flag

    }
};

/*
bool BLE_wcs::getMessageReceived() {
	return messageReceived;
}

void BLE_wcs::setMessageReceived(bool received) {
	messageReceived = received;
}
*/
void BLE_wcs::init(std::string deviceName)
{
	// Create the BLE Device
	BLEDevice::init(deviceName);

	// Create the BLE Server
	BLEServer *pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());

	// Create the BLE Service
	BLEService *pService = pServer->createService(SERVICE_UUID_UART);

	// Create a BLE Characteristic
	pCharacteristic = pService->createCharacteristic(
						CHARACTERISTIC_UUID_TX,
						BLECharacteristic::PROPERTY_NOTIFY
						);


	// create Descriptor for notify/indicate enable
	pCharacteristic->addDescriptor(new BLE2902());

	BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

	pCharacteristic->setCallbacks(new MyCallbacks());

	// Start the service
	pService->start();

	// Start advertising
	pServer->getAdvertising()->start();
}








void BLE_wcs::init_server(void)
{
	/* Create Service and a separate Characteristic for each Sensor */
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

