/*
** BMP280 Sensor Library for AVR Microcontrollers
** Version: 	0.1.0
** Date: 		August, 2022
** License:		LGPL v3
** Copyright: 	(C) 2009 - 2015 Radu Motisan, radu.motisan@gmail.com
*  Ported to Arduino (C)2022 Radu Motisan, radu.motisan@magnasci.com
** Description:	The BMP280 from Bosch is a temperature and pressure sensor with a I2C interface
**
** www.pocketmagic.net
**
** This file is a part of "Portable Environmental Monitor" open source project presented on
** https://hackaday.io/project/4977-portable-environmental-monitor
**
** This project is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 3 of the License,
** or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "drv_bmp280.h"
#include <Wire.h>
#include "Arduino.h"
#include "../../config.h"

uint8_t bmp_readmem(uint8_t address, uint8_t reg, uint8_t *buff, uint8_t bytes) {
  
	Wire.beginTransmission(address);    // Get the slave's attention, tell it we're sending a command byte
	Wire.write(reg);                    //  The command byte, sets pointer to register with address of 0x32
	Wire.endTransmission();
	Wire.requestFrom(address,bytes);          // Tell slave we need to read 1byte from the current register
	for (uint8_t i = 0 ; i< bytes; i++) {
		if (!Wire.available()) return i;
			buff[i] = Wire.read();
	}
	return bytes;
}

uint8_t bmp_readmem(uint8_t address, uint8_t reg) {
	Wire.beginTransmission((uint8_t)address);
	Wire.write((uint8_t)reg);
	Wire.endTransmission();
	Wire.requestFrom((uint8_t)address, 1);
	return Wire.read();
}

uint8_t bmp_writemem(uint8_t address, uint8_t reg, uint8_t value) {
	Wire.beginTransmission(address);
	Wire.write(reg);            // sends instruction byte
	Wire.write(value);             // sends potentiometer value byte
	Wire.endTransmission(); 
}

uint8_t bmp_writemem(uint8_t address, uint8_t reg, uint8_t *buff, uint8_t bytes) {
	Wire.beginTransmission(address);
	Wire.write(reg);            // sends instruction byte
	Wire.write(buff, bytes);             // sends potentiometer value byte
	Wire.endTransmission(); 
}

bool BMP280::start(uint8_t a) {
	m_addr = a;
#if DEV_CLASS == MODEL_A3
	Wire.begin(D5, D6); // SDA, SCL or GPI14, GPIO12
#else
	Wire.begin();
#endif

	bmp_readmem(m_addr, BMP280_REGISTER_CHIPID, &m_id, 1);
	if (m_id != BMP280_ID) return false;

	// reset the device using soft-reset this makes sure the IIR is off, etc.
	bmp_writemem(m_addr, BMP280_REGISTER_SOFTRESET, 0xB6);

	// wait for chip to wake up.
	delay(300);
	getcalibration();

	bmp_writemem(m_addr, BMP280_REGISTER_CONTROLHUMID, 0x05); // Set before CONTROL (DS 5.4.3)
	bmp_writemem(m_addr, BMP280_REGISTER_CONFIG, 0x0);
	bmp_writemem(m_addr, BMP280_REGISTER_CONTROL, 183);//B7
	delay(100);
	return true;
}

void BMP280::getcalibration() {
	// if chip is still reading calibration, delay
	while ((bmp_readmem(m_addr, BMP280_REGISTER_STATUS) & (1 << 0)) != 0);
   
	// read cal data
	bmp_readmem(m_addr, BMP280_REGISTER_T1, buffer, 24);

	reg_T1 = buffer[0] | (buffer[1]<<8);
	reg_T2 = buffer[2] | (buffer[3]<<8);
	reg_T3 = buffer[4] | (buffer[5]<<8);

	reg_P1 = buffer[6] | (buffer[7]<<8);
	reg_P2 = buffer[8] | (buffer[9]<<8);
	reg_P3 = buffer[10] | (buffer[11]<<8);
	reg_P4 = buffer[12] | (buffer[13]<<8);
	reg_P5 = buffer[14] | (buffer[15]<<8);
	reg_P6 = buffer[16] | (buffer[17]<<8);
	reg_P7 = buffer[18] | (buffer[19]<<8);
	reg_P8 = buffer[20] | (buffer[21]<<8);
	reg_P9 = buffer[22] | (buffer[23]<<8);
}

void BMP280::readSensors(double *temperature, double *pressure) {
  // read entire data buffer
	bmp_readmem(m_addr, BMP280_REGISTER_PRESSUREDATA, buffer, 8);

	// temperature
	adc_TP = ((int32_t)buffer[3] << 12) | ((int32_t)buffer[4] << 4) | (buffer[5] >> 4) ;
  	var1 = ((((adc_TP >> 3) - ((int32_t)reg_T1 << 1))) * ((int32_t)reg_T2)) >> 11;
	var2 = (((((adc_TP >> 4) - ((int32_t)reg_T1)) * ((adc_TP >> 4) - ((int32_t)reg_T1))) >> 12) * ((int32_t)reg_T3)) >> 14;
	t_fine = var1 + var2;
	float TT  =  (((t_fine * 5 + 128) >> 8) / 100.00) ;
  	*temperature =  (((t_fine * 5 + 128) >> 8) / 100.00) ;

	// pressure
	adc_TP = ((int32_t)buffer[0] << 12) | ((int32_t)buffer[1] << 4) | ((int32_t)buffer[2] >> 4) ;
	var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
	var2 = (((var1 >> 2) * (var1 >> 2)) >> 11 ) * ((int32_t)reg_P6);
	var2 = var2 + ((var1 * ((int32_t)reg_P5)) << 1);
	var2 = (var2 >> 2) + (((int32_t)reg_P4) << 16);
	var1 = (((reg_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13 )) >> 3) + ((((int32_t)reg_P2) * var1) >> 1)) >> 18;
	var1 = ((((32768 + var1)) * ((int32_t)reg_P1)) >> 15);
	if (var1 != 0) {
		p = (((uint32_t)(((int32_t) 1048576) - adc_TP) - (var2 >> 12))) * 3125;
		if (p < 0x80000000) {
			p = (p << 1) / ((uint32_t)var1);
		} else {
			p = (p / (uint32_t)var1) * 2;
		}
		var1 = (((int32_t)reg_P9) * ((int32_t)(((p>>3) * (p >> 3)) >> 13))) >> 12;
		var2 = (((int32_t)(p >> 2)) * ((int32_t)reg_P8)) >> 13;
		p = (uint32_t)((int32_t)p + ((var1 + var2 + reg_P7) >> 4));
		*pressure = p;
	}
}
