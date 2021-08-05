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

