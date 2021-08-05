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
 
#include "drv_mhz19b.h"

uint8_t MHZ19B::fusedata(char c) {
	buf[index] = c;
 //Serial.printf("%02X ", c);
	if (index == 0 && c != 0xFF) {
		index = 0; return 0;
	}
	if (index == 1 && c != 0x86) {
		index = 0; return 0;
	}
	if (checksum()) {
		parsedata();
	}

	index ++;


	if (index == 9) index = 0; // start over

	return index;
}

uint8_t* MHZ19B::getSendCmd(){ 
  return read_cmd;
}

bool MHZ19B::checksum() {
	if (index != 8) return false;
  uint8_t checksum = 0xFF;
  for (uint8_t i = 1; i < 8; i++)
    checksum -= buf[i];
  return ((checksum +1) == buf[8]) ;
}

void MHZ19B::parsedata() {
	co2 = buf[2] * 256 + buf[3]; 
}

uint16_t MHZ19B::getCO2() { return co2; }
