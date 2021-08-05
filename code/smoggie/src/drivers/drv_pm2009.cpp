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


#include "drv_pm2009.h"
#include <Arduino.h>

uint8_t PM2009::fusedata(char c) {
	
	buffer[index] = c;
	if (index == 0 && c != 0x16) {
		index = 0; return 0;
	}
	if (index == 1 && c != 0x35) {
		index = 0; return 0;
	}
	if (index == 2 && c != 0x0B) {
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();

	}

	index ++;


	if (index == 56) index = 0; // start over

	return index;
}

bool PM2009::checksum() {
	if (index != 55) return false;
	uint8_t checksum = 0xFF;
  	for (uint8_t i = 0; i < 55; i++)
    	checksum -= buffer[i];
  	return ((checksum +1) == buffer[55]) ;
}

void PM2009::parsedata() {
	uint16_t tmp1 = 0, tmp25 = 0, tmp10 = 0;

	tmp1 = (buffer[5] << 8) | buffer[6];
	tmp25 = (buffer[9] << 8) | buffer[10];
	tmp10 = (buffer[13] << 8) | buffer[14];
	if (tmp1 <= tmp25 && tmp25 <= tmp10) {
		pm1ug = tmp1;
		pm25ug = tmp25;
		pm10ug = tmp10;
	}
}

uint8_t* PM2009::getSendCmd(){ 
  return read_cmd;
}

uint16_t PM2009::getPM1() { return pm1ug; }
uint16_t PM2009::getPM25() { return pm25ug; }
uint16_t PM2009::getPM10() { return pm10ug; }

