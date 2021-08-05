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


#include "drv_ze03.h"
#include "../../config.h"
#include <Arduino.h>

uint8_t ZE03::fusedata(char c) {
	buf[index] = c;
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


bool ZE03::checksum() {
	if (index != 8) return false;
  	uint8_t checksum = 0xFF;
  	for (uint8_t i = 1; i < 8; i++)
    	checksum -= buf[i];
  	return ((checksum +1) == buf[8]) ;
}

void ZE03::parsedata() {
	type = buf[4];
	uint16_t conc = buf[2] * 256 + buf[3]; 
    gas = (double)conc * getMultiplier();
  
 //Serial.printf("\nGasid: %X Conc:%.1f\n", type, gas);
}

double ZE03::getGas() { return gas; }

#ifdef MUL_O3
	double ZE03::getMultiplier() {
	  switch (type) {
	    case O3: return MUL_O3;
	    case CL2:return MUL_CL2;
	    case NO2: return MUL_NO2;
	    case SO2: return MUL_SO2;
	    case HCL: return MUL_HCL;
	    case H2S: return MUL_H2S;
	    case CO: return MUL_CO;
	    case C6H6: return MUL_C6H6;
	    case HCN: return MUL_HCN;
	    case NH3: return MUL_NH3;
	    default: return 1;
	  }
	}
#else
	double ZE03::getMultiplier() {
/*  switch (type) {
    case H2S: 
    //case C6H6:
    case HCN: return 0.01;

    case O3:
    case CL2:
    case NO2:
    case SO2:
    case C6H6:
    case HCL: return 0.1;

    case NH3:
    case CO: return 1.0;

    defaut: return 1.0;
  */
    switch (type) {
    	case CO: return 1;
    	default: return 0.01;
	}
}
#endif

void ZE03::getZE03Name( char *out, uint8_t len) {
	switch (type) {
		case O3: strncpy(out, ("O3 "), len); break;
		case NO2: strncpy(out, ("NO2"), len); break;
		case SO2: strncpy(out, ("SO2"), len); break;
		case CO: strncpy(out, ("CO "), len); break;
		case H2S: strncpy(out, ("H2S"), len); break;
		case NH3: strncpy(out, ("NH3"), len); break;
		case CL2: strncpy(out, ("CL2"), len); break;
		case HCL: strncpy(out, ("HCL"), len); break;
		case HCN: strncpy(out, ("HCN"), len); break;
		case C6H6: strncpy(out, ("C6H6"), len); break;
		default: strncpy(out, ("UNK"), len); break;
	}
}
uint8_t ZE03::getType() {
  return type;
}

uint16_t ZE03::getMax() {
	switch (type) {
		case O3: return 20; // reduced 10winsen, normal 20ppm
		case NO2: return 20; // reduced 10winsen, normal 20ppm
		case SO2: return 20;
		case CO: return 200; // reduced 200winsen, normal 1000ppm
		case H2S: return 100;
		case NH3: return 100;
		case CL2: return 10;
		case HCL: return 10;
		default: return 0;
	}
}
double ZE03::getAnalogueConc(double vol) {
	switch (type) {
		// inversed 2V-0: NO2, HCL, O3, CL2, HF
		case  O3: return (getMax() * (vol - 2.0)) / (0.0 - 2.0); break;	// 2.0 - 0.0V	0 -   20ppm
		case CL2: return (getMax() * (vol - 2.0)) / (0.0 - 2.0); break;
		case NO2: return (getMax() * (vol - 2.0)) / (0.0 - 2.0); break;	// 2.0 - 0.0V	0 -   20ppm
		case HCL: return (getMax() * (vol - 2.0)) / (0.0 - 2.0); break;	// 2.0 - 0.0V	0 -   10ppm
		// normalL 0.6-3V: CO, NH3, H2S, H2, PH3, SO2,
		case SO2: return (getMax() * (vol - 0.6)) / (3.0 - 0.6); break;	// 0.6 - 3.0V	0 -   20ppm
		case  CO: return (getMax() * (vol - 0.6)) / (3.0 - 0.6); break; 	// 0.6 - 3.0V	0 - 1000ppm
		case H2S: return (getMax() * (vol - 0.6)) / (3.0 - 0.6); break;	// 0.6 - 3.0V	0 -  100ppm
		case NH3: return (getMax() * (vol - 0.6)) / (3.0 - 0.6); break;
	}
	return 0;
}
