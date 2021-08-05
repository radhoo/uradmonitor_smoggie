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
 
#include "drv_cm11XX.h"


uint8_t CM11XX::fusedata(char c) {
	buf[index] = c;
 
	if (index == 0 && c != 0x16) {
		index = 0; return 0;
	}
	if (index == 1 && c != 0x05) {
		index = 0; return 0;
	}
  if (index == 2 && c != 0x01) {
   index = 0; return 0;
  }
  
	if (checksum()) {
		parsedata();
	}

	index ++;


	if (index == 8) index = 0; // start over

	return index;
}

uint8_t* CM11XX::getSendCmd(){ 
  return read_cmd;
}

bool CM11XX::checksum() {
  
  if (index != 7) return false;
  
  
  uint8_t checksum = 0;
  for (uint8_t i = 0; i < 7; i++)
    checksum = (checksum + buf[i]) % 256;
  checksum = 256 - checksum;
  return (checksum  == buf[7]) ;
}


void CM11XX::parsedata() {
	co2 = buf[3] * 256 + buf[4]; 
}

uint16_t CM11XX::getCO2() { return co2; }

uint8_t* CM11XX::getCalCmd(uint16_t conc) {
	 //Cumulative sum of data = 256-(HEAD+LEN+CMD+DATA)%256
	calco2[3] = (conc >> 8) & 0xFF;
	calco2[4] = conc & 0xFF;
	calco2[5] = 256 - (calco2[0] + calco2[1] + calco2[2] + calco2[3] + calco2[4]) % 256;
	return calco2;
  }
