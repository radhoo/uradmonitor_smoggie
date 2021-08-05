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

#define GAS_SENSOR SENSOR_GAS_WINSEN
#define GAS_SENSOR_NAME "ZE03"

enum {
	NH3  = 0x02,// 2
	H2S  = 0x03,// 3
	CO   = 0x04,// 4
	C6H6 = 0x1B,// benzene
	O3   = 0x2A,//42
	SO2  = 0x2B,//43
	NO2  = 0x2C,//44
	HCL  = 0x2E, //46
	CL2  = 0x31,//49
	HCN  = 0xF0
};


class ZE03 {

	uint8_t buf[10];
  	
	uint8_t index;
	double gas;
  uint8_t type;
public:
	ZE03() {
		index = 0;
    gas = 0;
	}
	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	
  double getGas();
  uint8_t getType();
  
  double getMultiplier();
  void getZE03Name(char *out, uint8_t len);

  uint16_t getMax();
  double getAnalogueConc(double vol);
};
