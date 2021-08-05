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
 
#pragma once
#include <stdint.h>

#define CO2_SENSOR SENSOR_CO2_WINSEN
#define CO2_SENSOR_NAME "MH-Z19B"

class MHZ19B {

	uint8_t buf[10];
  uint8_t read_cmd[9] = {0xFF,0x01,0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79}; // FF0186000000000079
  uint8_t read_size = 9;
	uint8_t index;
	uint16_t co2;
public:
	MHZ19B() {
		index = 0;
    co2 = 0;
	}
	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	uint8_t* getSendCmd();
  uint8_t getSendCmdSize() { return read_size; }
  uint16_t getCO2();
};
