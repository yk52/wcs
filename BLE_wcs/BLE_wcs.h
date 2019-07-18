#ifndef BLE_WCS_H_
#define BLE_WCS_H_



#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <stdio.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SERVICE_UUID_UART      "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"   // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"   // RX characteristic UUID
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"   // TX characteristic UUID


class BLE_wcs {
	public:
		//constructors
		BLE_wcs(void) {};
		~BLE_wcs(void) {};
		
		// member functions
		std::string getMessage(void);
		void init(std::string);
		void deinit();
		void write(std::string);

		// message received flag
		bool messageReceived;
		
	private:



};


#endif /* BLE_wcs_H_ */
