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


#include "drv_pms_generic.h"
 #include <Arduino.h>

uint8_t PMS::fusedata(char c) {
	
	buffer[index] = c;
	if (index == 0 && c != 0x42 && c != 0x44) {  // 0x42 Plantower, 0x44 Mixsen MIX6071
		index = 0; return 0;
	}
	if (index == 1 && c != 0x4d) {
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();

	}

	index ++;


	if (index == 32) index = 0; // start over

	return index;
}

bool PMS::checksum() {
	if (index != 31) return false;

	uint16_t crc = (buffer[30]  << 8) | buffer[31];

	uint16_t sum = 0;
	for (uint8_t i =0; i< 30;i++) sum+= buffer[i];

	return (crc == sum);
}

void PMS::parsedata() {
	pm1ug_std = (buffer[4] << 8) | buffer[5];
	pm25ug_std = (buffer[6] << 8) | buffer[7];
	pm10ug_std = (buffer[8] << 8) | buffer[9];
	
	pm1ug_atm = (buffer[10] << 8) | buffer[11];
	pm25ug_atm = (buffer[12] << 8) | buffer[13];
	pm10ug_atm = (buffer[14] << 8) | buffer[15];
}

// USING ATM ! (since FW15)
uint16_t PMS::getPM1() { return pm1ug_atm; }
uint16_t PMS::getPM25(){ return pm25ug_atm; }
uint16_t PMS::getPM10(){ return pm10ug_atm; }

