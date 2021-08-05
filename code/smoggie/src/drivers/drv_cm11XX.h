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


class CM11XX {

	uint8_t buf[10];
  uint8_t read_cmd[4] = 	{0x11, 0x01, 0x01, 0xED}; // 110101ED
  uint8_t abc_off[10] = 	{0x11, 0x07, 0x10, 0x64, 0x02, 0x07, 0x01, 0x90, 0x64, 0x76},
  			abc_on[10]=		{0x11, 0x07, 0x10, 0x64, 0x00, 0x07, 0x01, 0x90, 0x64, 0x78},	// 7 days
  			calco2[6] = 	{0x11, 0x03, 0x03, 0x02, 0x58, 0x8F};							// default 600ppm set
		
  uint8_t read_size = 4;
	uint8_t index;
	uint16_t co2;
public:
	CM11XX() {
		index = 0;
    co2 = 0;
	}

 
	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	uint8_t* getSendCmd();
  	uint8_t getSendCmdSize() { return read_size; }
  	uint16_t getCO2();
  
  	uint8_t* getCalCmd(uint16_t conc);
  	uint8_t getCalCmdSize() { return 6; }

  	uint8_t* getDisableABCCmd() { return  abc_off;}
  	uint8_t getDisableABCCmdSize() { return  10;}
	

	uint8_t* getEnableABCCmd() { return  abc_on;}
  	uint8_t getEnableABCCmdSize() { return  10;}
	
};
