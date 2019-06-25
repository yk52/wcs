#ifndef BLE_WCS_H_
#define BLE_WCS_H_



// #include "C:\Users\Yumi\Desktop\wcs\src\BLEDevice.h"
// #include "C:\Users\Yumi\Desktop\wcs\ESP32_BLE\src\BLEUtils.h"
// #include "C:\Users\Yumi\Desktop\wcs\ESP32_BLE\src\BLEServer.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
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
		void init(std::string deviceName);
		void init_server(void);
		void init_client(void);
		void setParameter();
		std::string getProperty();
		std::string getValue();
		
	private:
		// member variables
		int m_Skintype;
		int m_UVtreshold;
		int m_AQthreshold;
		bool dataRequest;

};


#endif /* BLE_WCS_H_ */
