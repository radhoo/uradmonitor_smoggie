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


#include "drv_sds.h"
 #include <Arduino.h>

uint8_t SDS::fusedata(char c) {
	//Serial.printf("%X", c);
	buffer[index] = c;
	if ((index == 0 && c != 0xAA) || (index == 1 && c != 0xC0) || (index == 9 && c != 0xAB)) {
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();

	}

	index ++;


	if (index == 10) index = 0; // start over

	return index;
}

bool SDS::checksum() {
	if (index != 9) return false;
//for (uint8_t i =2; i<= 10;i++) sum+= buffer[i];
	uint8_t crc = buffer[8];

	uint8_t sum = 0;
	for (uint8_t i =2; i<= 7;i++) sum+= buffer[i];

	return (crc == sum);
}

void SDS::parsedata() {
	pm1ug_atm = 0;// ((buffer[3] << 8) | buffer[2]) / 10; // used
	pm25ug_atm = ((buffer[3] << 8) | buffer[2]) / 10; // used
	pm10ug_atm = ((buffer[5] << 8) | buffer[4]) / 10; // used
	//Serial.printf("%d %d %d", pm1ug_atm, pm25ug_atm, pm10ug_atm);
}

uint16_t SDS::getPM1() { return pm1ug_atm; }
uint16_t SDS::getPM25(){ return pm25ug_atm; }
uint16_t SDS::getPM10(){ return pm10ug_atm; }

