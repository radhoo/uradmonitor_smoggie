/**
 *	File:			drv_sds.h
 *	Version:		1.0
 *	Date:			Oct 17, 2020
 *	Description:	Nova Fitness Co. SDS sensor
 *	Project:		uRADMonitor SMOGGIE, part of Global environmental monitoring network
 *	License:		Proprietary, all rights reserved. Unauthorized copying of this file, via any medium is strictly prohibited.
 *  
 *	Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 *	Copyright 2015-2020 Magnasci SRL, www.magnasci.com
 *
 */

#pragma once
#include <stdint.h>

class SDS {

	uint8_t buffer[24];
	uint8_t index;
	// pm in ug/m^3
	uint16_t pm1ug_std, pm25ug_std, pm10ug_std,
			pm1ug_atm, pm25ug_atm, pm10ug_atm;
public:
	SDS() {
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