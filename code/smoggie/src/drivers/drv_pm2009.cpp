/**
 *
 *  License:  GPL v3
 *  Project:  SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *            It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *            It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 *  Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *  Copyright 2015-2021 Magnasci SRL, www.magnasci.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

