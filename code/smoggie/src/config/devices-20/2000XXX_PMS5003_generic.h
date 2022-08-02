#pragma once

#include "../sensors.h"


#define 			ENCRYPT_PAYLOAD										// Payload should always be encrypted, E3/E3B format
#define 			DEFAULT_SEND_INTERVAL 		60	 					// 5min for LoraWAN
#define 			DEFAULT_WARMUP		 		10	 					// 110sec for CO2
#define 			VER_HW						5
#define				DEV_CLASS					0x20
#define				DEFAULT_SERVER 				"data.uradmonitor.com"	// The name of the virtual host which you want to  contact to - hostname
#define 			DEFAULT_SCRIPT 				"/api/v1/upload/ex"	// server resource to call
#define 			DEFAULT_INTERNET_SSID		"WRM_2.4GHz"			// default WIFI to connect to
#define 			DEFAULT_INTERNET_KEY		"qq_wifi_4929"			// default WIFI key
#define 			DEFAULT_MUTE				1

#define       		DEFAULT_DISABLEAP     		0         // 0 if not used, or number of seconds to disable internal hotspot
#define 			DEFAULT_ENCRYPT				1
#define 			USE_SHT

#define 			SENSOR 						SENSOR_PMS5003
#define 			SENSOR_BAUDRATE				9600


//#define DEBUG

#include "../../data/data_ex.h"

// drivers
#ifdef USE_SHT
	#include "../../drivers/drv_sht21.h"
#else
	#include "../../drivers/drv_bme280.h"
#endif

#include "../../drivers/drv_pms_generic.h"

#define SENSOR_CLASS PMS
#define SENSOR_NAME "PLANTOWER PMS5003"
#define DATA_CLASS	 Data_EX
#define FACTOR 0.63
#define INCREASE_LIFESPAN // cannot use IL on PMSA003 Plantower sensors

/*
// normal enclosure
#define				CAL_DEF_TEMPERATURE			0x00648384		// -9 // new
#define				CAL_DEF_PRESSURE			0x00640000		// 1*x + 0
#define				CAL_DEF_HUMIDITY			0x00640BB8		// +30 //new
#define				CAL_DEF_VOC					0x00640000		// 1*x + 0
#define				CAL_DEF_CO2					0x00640000		// 1*x + 0
#define				CAL_DEF_CH2O				0x00640000		// 1*x + 0
#define				CAL_DEF_PM25				0x00640000		// 1*x + 0
#define				CAL_DEF_NOISE				0x00640000		// 1*x + 0
#define				CAL_DEF_PM1					0x00640000		// 1*x + 0
#define				CAL_DEF_PM10				0x00640000		// 1*x + 0
#define				CAL_DEF_O3					0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S1				0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S2				0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S3				0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S4				0x00640000		// 1*x + 0
*/
// stevenson enclosure
#define				CAL_DEF_TEMPERATURE			0x00648366		// -8.7
#define				CAL_DEF_PRESSURE			0x00640000		// 1*x + 0
#define				CAL_DEF_HUMIDITY			0x006409C4		// +25
#define				CAL_DEF_VOC					0x00640000		// 1*x + 0
#define				CAL_DEF_CO2					0x00640000		// 1*x + 0
#define				CAL_DEF_CH2O				0x00640000		// 1*x + 0
#define				CAL_DEF_PM25				0x00640000		// 1*x + 0
#define				CAL_DEF_NOISE				0x00640000		// 1*x + 0
#define				CAL_DEF_PM1					0x00640000		// 1*x + 0
#define				CAL_DEF_PM10				0x00640000		// 1*x + 0
#define				CAL_DEF_O3					0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S1				0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S2				0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S3				0x00640000		// 1*x + 0
#define				CAL_DEF_ZE03S4				0x00640000		// 1*x + 0
