/**
 *	File:       	pms_generic.h
 *	Version:  		1.0
 *	Date:       	Jan 20, 2018 - OCT 17, 2020
 *	Description:	Plantower PMS7003 / PMSA003 / PMS5003 / PMS1003 driver class
 *	Project:		uRADMonitor AIR, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2020 Magnasci SRL, www.magnasci.com
 *
 */

#pragma once
#include <stdint.h>

class PMS {

	uint8_t buffer[32];
	uint8_t index;
	// pm in ug/m^3
	uint16_t pm1ug_std, pm25ug_std, pm10ug_std,
			pm1ug_atm, pm25ug_atm, pm10ug_atm;
	uint16_t	pm03no, pm05no, pm1no, pm25no, pm5no, pm10no;	// 2B each


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
		// counts
		pm03no = 0; 
		pm05no = 0;
		pm1no = 0; 
		pm25no = 0;
		pm5no = 0;
		pm10no = 0;	// 2B each

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

	uint16_t getPM03No() { return pm03no; }
	uint16_t getPM05No() { return pm05no; }
	uint16_t getPM1No() { return pm1no; }
	uint16_t getPM25No() { return pm25no; }
	uint16_t getPM5No() { return pm5no; }
	uint16_t getPM10No() { return pm10no; }
};
