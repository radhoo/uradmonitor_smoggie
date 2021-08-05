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

class PMS {

	uint8_t buffer[32];
	uint8_t index;
	// pm in ug/m^3
	uint16_t pm1ug_std, pm25ug_std, pm10ug_std,
			pm1ug_atm, pm25ug_atm, pm10ug_atm;
public:
	PMS() {
		// instant value
		pm1ug_std = 0;
		pm25ug_std = 0;
		pm10ug_std = 0;
		// compensated values
		pm1ug_atm = 0;
		pm25ug_atm = 0;
		pm10ug_atm = 0;
		index = 0;
	}
	uint8_t fusedata(char c);
	bool checksum();
	void parsedata();
	//uint16_t getPM1Std();
	//uint16_t getPM25Std();
	//uint16_t getPM10Std();

	uint16_t getPM1();
	uint16_t getPM25();
	uint16_t getPM10();
};
