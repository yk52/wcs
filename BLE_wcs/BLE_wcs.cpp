#include "C:\Users\Joanna\Documents\MasterESE\3-Semester\WCS\wcs\BLE_wcs\BLE_wcs.h"


BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
bool messageReceived = false;

std::string rxValue = "default"; 									//  global variable with received data

/*********************************************************************************
*								server callback class
*********************************************************************************/
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
		deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
		deviceConnected = false;
    }
};

/*********************************************************************************
*									callback class
*********************************************************************************/
class MyCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *pCharacteristic) {
		
		rxValue = pCharacteristic->getValue();				// write received message to global variable rxValue
		messageReceived = true;								// set messageReceived flag

    }
};

/*********************************************************************************
*									BLE functions
*********************************************************************************/


// getter for received message
std::string BLE_wcs::getMessage() {
	return rxValue;
}

// init function for ble
void BLE_wcs::init(std::string deviceName)
{
	// Create the BLE Device
	BLEDevice::init(deviceName);

	// Create the BLE Server
	//BLEServer *pServer = BLEDevice::createServer();
	pServer = BLEDevice::createServer();
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

void BLE_wcs::startAdvertising()
{
	// Stop advertising
	pServer->getAdvertising()->start();
}

void BLE_wcs::stopAdvertising()
{
	// Stop advertising
	pServer->getAdvertising()->stop();
}

void BLE_wcs::deinit()
{
	BLEDevice::deinit();
}	

void BLE_wcs::write(std::string txValue) {
	pCharacteristic->setValue(txValue);
	pCharacteristic->notify();
}