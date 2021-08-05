/**
 *	License:	Licensed under CC BY NC 4.0
 *  			This license lets others remix, adapt, and build upon your work non-commercially, and although their new works must also acknowledge you 
 *  			and be non-commercial, they don’t have to license their derivative works on the same terms.
 *  			You should have received a copy of the license with this work.
 *
 *  Project:    SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *  			It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *  			It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 *  Author:  	Magnasci SRL , www.magnasci.com 
 *	Web:		www.uradmonitor.com
 *
 *  Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2021 Magnasci SRL, www.magnasci.com
**/

#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#define 			EV3_TYPE 					0x6E// 2 bytes max // goes for E14B
#define 			EV3_KEYA 					167	// 1 byte
#define 			EV3_KEYB 					47	// 1 byte



class Data_EX {
public:
	// common
	double				sensorTemperature,				// bme280: temperature
						sensorHumidity,					// bme280: humidity
						sensorPressure;					// bme280: barometric pressure
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3
	double				sensorPM1,						// zh03a: pm1.0
						sensorPM25,						// zh03a: pm2.5
						sensorPM10;						// zh03a: pm10
#elif DEV_CLASS == SMOGGIE_CO2    
	double 				sensorCO2;						// mh-z19b, MIX, CM1106?, etc
#elif DEV_CLASS == SMOGGIE_GAS   
	uint8_t				type;
	double 				sensorGas,
						sensorVoltage;
	char        		name[10];   
#elif DEV_CLASS == SMOGGIE_RADON    
	double				sensorRadon;					// eurogas, tesla, etc.
#elif DEV_CLASS == MODEL_NOISE
	double				sensorNoise;	
#endif	

	uint8_t				sensor;
	uint8_t 			crcb;


	// E3/E5 implementation
	uint8_t 			dataArray[100],					// unsafe! data structure: create a pointer to hold all values in order
						offset;							// most significant bytes go first (natural reading)

	Data_EX();

	uint16_t packData(bool encrypt, char *szClient, uint16_t len, uint32_t id, uint32_t timesec, uint8_t verHW , uint8_t verSW );

	double getTemperature();
	uint32_t getPressure();
	double getHumidity();
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3
	uint16_t getPM1();		// ug/m^3, 0 to 1000
	uint16_t getPM25();		// ug/m^3, 0 to 1000
	uint16_t getPM10();		// ug/m^3, 0 to 1000
#elif DEV_CLASS == SMOGGIE_CO2    
	uint16_t getCO2();
#elif DEV_CLASS == SMOGGIE_GAS   
	double getGas();		// 
	char* getName();
	double getVoltage(); 
#elif DEV_CLASS == SMOGGIE_RADON    
	uint16_t getRadon();	// Bq/m^3
#elif DEV_CLASS == MODEL_NOISE
	double getNoise();
#endif		
};
