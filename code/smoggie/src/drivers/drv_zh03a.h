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

#pragma once
#include <stdint.h>

class ZH03A {

	uint8_t buffer[24];
	uint8_t index;
	// pm in ug/m^3
	uint16_t pm1ug_std, pm25ug_std, pm10ug_std,
			pm1ug_atm, pm25ug_atm, pm10ug_atm;
public:
	ZH03A() {
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
