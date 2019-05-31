#ifndef BLE_WCS_H_
#define BLE_WCS_H_



#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class BLE_wcs {
	public:
		//constructors
		BLE_wcs(void) {};
		~BLE_wcs(void) {};
		
		void init_server();
		void init_client();
		
	private:

};


#endif /* BLE_wcs_H_ */