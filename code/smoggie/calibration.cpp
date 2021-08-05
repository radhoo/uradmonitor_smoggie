/**
 * License:  Licensed under CC BY NC 4.0
 *           This license lets others remix, adapt, and build upon your work non-commercially, and although their new works must also acknowledge you 
 *           and be non-commercial, they don’t have to license their derivative works on the same terms.
 *           You should have received a copy of the license with this work.
 *
 * Project:  SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *           It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *           It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 * Author:   Magnasci SRL , www.magnasci.com 
 * Web:      www.uradmonitor.com
 *
 * Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 * Copyright 2015-2021 Magnasci SRL, www.magnasci.com
**/


#include "calibration.h"
#include "config.h"
// temperature, pressure, humidity, voc, co2, ch2o, pm25, noise, pm1, pm10,  o3, ze03s1, ze03s2, ze03s3, ze03s4

char calNoun[CAL_VALUES][10] = {
  "cal-02", // 0, temperature
  "cal-03", // 1, pressure
  "cal-04", // 2, humidity
  "cal-06", // 3, voc
  "cal-07", // 4, co2
  "cal-08", // 5, formaldehyde
  "cal-09", // 6, pm25
  "cal-11", // 7, noise
  "cal-12", // 8, pm1
  "cal-13", // 9, pm10
  "cal-14", // 10, ozone
  "cal-gas1",
  "cal-gas2",
  "cal-gas3",
  "cal-gas4"
};

uint32_t Calibration::getDefault(uint8_t i) {
	switch (i) {
		case CAL_TEMPERATURE: return CAL_DEF_TEMPERATURE;
		case CAL_PRESSURE: return CAL_DEF_PRESSURE;
		case CAL_HUMIDITY: return CAL_DEF_HUMIDITY;
		case CAL_VOC: return CAL_DEF_VOC;
		case CAL_CO2: return CAL_DEF_CO2;
		case CAL_CH2O: return CAL_DEF_CH2O;
		case CAL_PM25: return CAL_DEF_PM25;
		case CAL_NOISE: return CAL_DEF_NOISE;
		case CAL_PM1: return CAL_DEF_PM1;
		case CAL_PM10: return CAL_DEF_PM10;
		case CAL_O3: return CAL_DEF_O3;
		case CAL_ZE03S1: return CAL_DEF_ZE03S1;
		case CAL_ZE03S2: return CAL_DEF_ZE03S2;
		case CAL_ZE03S3: return CAL_DEF_ZE03S3;
		case CAL_ZE03S4: return CAL_DEF_ZE03S4;
		default: return CAL_DEF_NOCHANGE;
	}
}

// decode linear calibration (a*x+b) param: a is high, b is low
void Calibration::calibrate(double *value, uint8_t parameterIndex, bool forcedPositive) {
	uint32_t calibrationParam = cal[parameterIndex];
	*value = id((calibrationParam >> 16) & 0xFFFF) * (*value) + id(calibrationParam & 0xFFFF);
	if (forcedPositive && (*value < 0)) *value = 0;
}

double Calibration::calibrate(double value, uint8_t parameterIndex, bool forcedPositive) {
  uint32_t calibrationParam = cal[parameterIndex];
  double res = id((calibrationParam >> 16) & 0xFFFF) * value + id(calibrationParam & 0xFFFF);
  if (forcedPositive && (res < 0)) 
    return 0;
  else
    return res;
}

void Calibration::calibrate(uint32_t *value, uint8_t parameterIndex, bool forcedPositive) {
	uint32_t calibrationParam = cal[parameterIndex];
	// decode linear calibration (a*x+b) param: a is high, b is low
	*value = id((calibrationParam >> 16) & 0xFFFF) * (*value) + id(calibrationParam & 0xFFFF);
	if (forcedPositive && (*value < 0)) *value = 0;
}

bool Calibration::setCalibrationParam(uint8_t parameterIndex, uint32_t calibrationParam) {
	if (parameterIndex >= CAL_VALUES) return 0;
	// save in memory
	cal[parameterIndex] = calibrationParam;

	return 1;
}
