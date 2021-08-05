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

#include "../../config.h"
#include "data_ex.h"
#include "../../aux.h"
#include <Arduino.h>

Data_EX::Data_EX() {
	sensor = (uint8_t)(SENSOR & 0xFF);
	// common to all variants
	sensorTemperature		= 0;						// bme280: temperature
	sensorHumidity			= 0;						// bme280: humidity
	sensorPressure			= 0;						// bme280: barometric pressure
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3
	sensorPM1				= 0;						// zh03a: pm1.0
	sensorPM25				= 0;						// zh03a: pm2.5
	sensorPM10				= 0;						// zh03a: pm10
#elif DEV_CLASS == SMOGGIE_CO2    
	sensorCO2				= 0;						// mh-z19b, MIX, CM1106?, etc
#elif DEV_CLASS == SMOGGIE_GAS   
	sensorGas				= 0;						// 
#elif DEV_CLASS == SMOGGIE_RADON    
	sensorRadon				= 0;						// eurogas, tesla, etc.
#elif DEV_CLASS == MODEL_NOISE
	sensorNoise				= 0;	
#endif	
	// init struct
	memset(dataArray, 0, sizeof(dataArray));
	offset = 0;
	
}


uint16_t Data_EX::packData(bool encrypt, char *szClient, uint16_t len, uint32_t id, uint32_t timesec, uint8_t verHW, uint8_t verSW) {
	return 0;
}

double Data_EX::getTemperature() {
	return sensorTemperature;
}
uint32_t Data_EX::getPressure() {
	return (uint32_t)sensorPressure;
}
double Data_EX::getHumidity() {
	return sensorHumidity;
}
#if DEV_CLASS == SMOGGIE_PM || DEV_CLASS == MODEL_A3
	uint16_t Data_EX::getPM1() {
		return (uint16_t)sensorPM1;
	}
	uint16_t Data_EX::getPM25() {
		return (uint16_t)sensorPM25;
	}
	uint16_t Data_EX::getPM10() {
		return (uint16_t)sensorPM10;
	}
#elif DEV_CLASS == SMOGGIE_CO2    
	uint16_t Data_EX::getCO2() {
		return (uint16_t)sensorCO2;
	}
#elif DEV_CLASS == SMOGGIE_GAS   
	double Data_EX::getGas() {
		return sensorGas;
	}
	char* Data_EX::getName() {
	  return name;
	}
	/*uint8_t Data_EX::getType() {
		return (uint8_t)type;
	}*/
	double Data_EX::getVoltage() {
		return sensorVoltage;
	}
#elif DEV_CLASS == SMOGGIE_RADON    
	uint16_t Data_EX::getRadon() {	// Bq/m^3
		return sensorRadon;
	}
#elif DEV_CLASS == MODEL_NOISE
	double Data_EX::getNoise() {
		return sensorNoise;
	}	
#endif	
