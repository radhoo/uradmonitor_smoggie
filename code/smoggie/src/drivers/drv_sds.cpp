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

