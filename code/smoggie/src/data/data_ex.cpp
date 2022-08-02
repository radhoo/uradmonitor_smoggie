/**
 *	Project:	uRADMonitor model A/A2/A3/INDUSTRIAL, part of Global environmental monitoring network
 *	Web:		www.uradmonitor.com
 *	License:	Proprietary, all rights reserved.
 *
 *	This file contains proprietary information which may be legally privileged. Any unauthorized use or dissemination is prohibited.
 *	It is for the intended recipient only. If you are not the intended recipient you must not use, disclose, distribute, copy or print this file.
 *
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2018 Magnasci SRL, www.magnasci.com
**/
#include "../../config.h"
#include "data_ex.h"
#include "../../misc.h"
#include <Arduino.h>

Data_EX::Data_EX() {
	sensor = (uint8_t)(SENSOR & 0xFF);
	// common to all variants
	sensorTemperature		= 0;						// bme280: temperature
	sensorHumidity			= 0;						// bme280: humidity
	sensorPressure			= 0;						// bme280: barometric pressure
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3 || DEV_CLASS == SMOGGIE_PM_KIT
	sensorPM1				= 0;						// zh03a: pm1.0
	sensorPM25				= 0;						// zh03a: pm2.5
	sensorPM10				= 0;						// zh03a: pm10
#elif DEV_CLASS == SMOGGIE_CO2    
	sensorCO2				= 0;						// mh-z19b, MIX, CM1106?, etc
#elif DEV_CLASS == SMOGGIE_GAS   
	sensorGas				= 0;						// 
#elif DEV_CLASS == SMOGGIE_RADON    
	sensorRadon				= 0;						// eurogas, tesla, etc.
#elif DEV_CLASS == MODEL_NOISE
	sensorNoise				= 0;	
#endif	
#if DEV_CLASS == SMOGGIE_PM_KIT
	pm03no = 0;
	pm05no = 0;
	pm1no = 0;
	pm25no = 0;
	pm5no = 0;
	pm10no = 0;
#endif
	// init struct
	memset(dataArray, 0, sizeof(dataArray));
	offset = 0;
	
}


uint16_t Data_EX::packData(bool encrypt, char *szClient, uint16_t len, uint32_t id, uint32_t timesec, uint8_t verHW, uint8_t verSW) {
	uint8_t tmp8 = 0;
	uint16_t tmp16 = 0;
	uint32_t tmp32 = 0;
	offset = 0;
	// get in order: most significant first
	offset = copyBytes((void *)dataArray, len,  offset, (void *)&id, 4); // 4B: device ID : t=4
	offset = copyBytes(dataArray, len,  offset, (void *)&verHW, 1); // 1B: ver HW : t=5
	offset = copyBytes(dataArray, len,  offset, (void *)&verSW, 1); // 1B: ver SW : t=6
	offset = copyBytes(dataArray, len,  offset, (void *)&timesec, 4); // 4B: local time : t=10

	// other sensor data
  	tmp16 = coordToInt16(sensorTemperature, 100); offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B: temperature : t=12, resolution 0.01 degrees
	tmp16 = sensorPressure - 65535; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B: complementary pressure : t=14
	tmp8 = sensorHumidity *2; offset = copyBytes(dataArray, len,  offset, (void *)&tmp8, 1); // 1B: humidity : t=15, resolution 0.5RH
	tmp8 = sensor; offset = copyBytes(dataArray, len, offset, (void *)&tmp8, 1); //1B t= 16
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3	 
	tmp16 = sensorPM1; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B: PM1 : t=18
	tmp16 = sensorPM25; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B: PM2.5 : t=20
	tmp16 = sensorPM10; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B: PM10 : t=22
#elif DEV_CLASS == SMOGGIE_CO2    
	tmp16 = sensorCO2; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B: t = 18
#elif DEV_CLASS == SMOGGIE_GAS   
	tmp8 = type; offset = copyBytes(dataArray, len, offset, (void *)&tmp8, 1); //1B t=17
	tmp32 = (uint32_t)(sensorGas * 1000.0); offset = copyBytes(dataArray, len,  offset, (void *)&tmp32, 4); // 4B t=21
	tmp16 = (uint16_t)(sensorVoltage * 1000.0); offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B t=23
#elif DEV_CLASS == SMOGGIE_RADON    
	tmp16 = sensorRadon; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B t=18
#elif DEV_CLASS == MODEL_NOISE
	tmp16 = coordToInt16(sensorNoise, 10); offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B t=18
#elif DEV_CLASS == SMOGGIE_PM_KIT
	tmp16 = sensorPM1; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B: PM1 : t=18
	tmp16 = sensorPM25; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B: PM2.5 : t=20
	tmp16 = sensorPM10; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B: PM10 : t=22
	// extra 2x6 bytes
	tmp16 = pm03no; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B:  : t=24
	tmp16 = pm05no; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B:  : t=26
	tmp16 = pm1no; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B:  : t=28
	tmp16 = pm25no; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B:  : t=30
	tmp16 = pm5no; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B:  : t=32
	tmp16 = pm10no; offset = copyBytes(dataArray, len,  offset, (void *)&tmp16, 2); // 2B:  : t=34

	//6EEB303E2C9E5BC3646B849A843BAD11C70B5C3374DB0C0324ABBCD3D47B6CA3844B1C04 72 t=36
#endif

	// HASH: compute crc32, xor it with deviceID and add it at the end
	uint8_t crc = encrypt ? crc8(dataArray, offset) ^ (id & 0x000000FF) : crc8(dataArray, offset);

	offset = copyBytes(dataArray, len,  offset, (void *)&crc, 1); //  t= 23 / 19 / 24 / 19 / 19 / 35   +1B MSG_ID

	// encode all but the last byte which is used as seed for the LCG PRNG
	uint8_t key = dataArray[offset - 1];
	memset(szClient, 0, len);
	// EV2, first 2 bytes are non-encrypted E-type sequence
	if (encrypt)
		sprintf(szClient, "%02X", (uint8_t) EV3_TYPE); // t=44/63 ! or on nogps: // t=44/63-14=49!

	

	for (int i=0;i<offset;i++) {
		if (encrypt) {
			// encrypt data with symmetric key (encryption I), all except last byte
			key = (EV3_KEYA * key + EV3_KEYB) % 256;
			sprintf(szClient, "%s%02X", szClient, (i < offset-1)?dataArray[i] ^ key : dataArray[i]);
		} else
			sprintf(szClient, "%s%02X", szClient, dataArray[i]);
	}
	#ifdef DEBUG
	Serial.printf("\nData-enc: [%s]\n", szClient);
	#endif

	return offset;
}

double Data_EX::getTemperature() {
	return sensorTemperature;
}
uint32_t Data_EX::getPressure() {
	return (uint32_t)sensorPressure;
}
double Data_EX::getHumidity() {
	return sensorHumidity;
}
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3 || DEV_CLASS == SMOGGIE_PM_KIT
	uint16_t Data_EX::getPM1() {
		return (uint16_t)sensorPM1;
	}
	uint16_t Data_EX::getPM25() {
		return (uint16_t)sensorPM25;
	}
	uint16_t Data_EX::getPM10() {
		return (uint16_t)sensorPM10;
	}
#elif DEV_CLASS == SMOGGIE_CO2    
	uint16_t Data_EX::getCO2() {
		return (uint16_t)sensorCO2;
	}
#elif DEV_CLASS == SMOGGIE_GAS   
	double Data_EX::getGas() {
		return sensorGas;
	}
	char* Data_EX::getName() {
	  return name;
	}
	/*uint8_t Data_EX::getType() {
		return (uint8_t)type;
	}*/
	double Data_EX::getVoltage() {
		return sensorVoltage;
	}
#elif DEV_CLASS == SMOGGIE_RADON    
	uint16_t Data_EX::getRadon() {	// Bq/m^3
		return sensorRadon;
	}
#elif DEV_CLASS == MODEL_NOISE
	double Data_EX::getNoise() {
		return sensorNoise;
	}	
#endif	
#if DEV_CLASS == SMOGGIE_PM_KIT
	uint16_t Data_EX::getPM03No() { return (uint16_t)pm03no; }
	uint16_t Data_EX::getPM05No() { return (uint16_t)pm05no; }
	uint16_t Data_EX::getPM1No() { return (uint16_t)pm1no; }
	uint16_t Data_EX::getPM25No() { return (uint16_t)pm25no; }
	uint16_t Data_EX::getPM5No() { return (uint16_t)pm5no; }
	uint16_t Data_EX::getPM10No() { return (uint16_t)pm10no; }
#endif
