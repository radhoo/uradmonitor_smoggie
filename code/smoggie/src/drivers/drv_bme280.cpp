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
 
#include "drv_bme280.h"
#include <Wire.h>
#include "Arduino.h"
#include "../../config.h"

uint8_t readmem(uint8_t address, uint8_t reg, uint8_t *buff, uint8_t bytes) {
  
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


uint8_t readmem(uint8_t address, uint8_t reg) {
  Wire.beginTransmission((uint8_t)address);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)address, 1);
  return Wire.read();
}

uint8_t writemem(uint8_t address, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(reg);            // sends instruction byte
  Wire.write(value);             // sends potentiometer value byte
  Wire.endTransmission(); 
}

uint8_t writemem(uint8_t address, uint8_t reg, uint8_t *buff, uint8_t bytes) {
  Wire.beginTransmission(address);
  Wire.write(reg);            // sends instruction byte
  Wire.write(buff, bytes);             // sends potentiometer value byte
  Wire.endTransmission(); 
}

bool BME280::start(uint8_t a) {
	m_addr = a;
#if DEV_CLASS == MODEL_A3
	Wire.begin(D5, D6); // SDA, SCL or GPI14, GPIO12
#else
	  Wire.begin();
#endif

	readmem(m_addr, BME280_REGISTER_CHIPID, &m_id, 1);
	if (m_id != BME280_ID) return false;

  // reset the device using soft-reset this makes sure the IIR is off, etc.
  writemem(m_addr, BME280_REGISTER_SOFTRESET, 0xB6);
 
  // wait for chip to wake up.
  delay(300);
	getcalibration();
 
	writemem(m_addr, BME280_REGISTER_CONTROLHUMID, 0x05); // Set before CONTROL (DS 5.4.3)
  writemem(m_addr, BME280_REGISTER_CONFIG, 0x0);
	writemem(m_addr, BME280_REGISTER_CONTROL, 183);//B7
  delay(100);
	return true;
}

void BME280::getcalibration() {

  // if chip is still reading calibration, delay
  while ((readmem(m_addr, BME280_REGISTER_STATUS) & (1 << 0)) != 0);
   
  // read cal data
	readmem(m_addr, BME280_REGISTER_T1, buffer, 26);

	reg_T1 = buffer[0] | (buffer[1]<<8) ;
	reg_T2 = buffer[2] | (buffer[3]<<8) ;
	reg_T3 = buffer[4] | (buffer[5]<<8) ;

  reg_P1 = buffer[6] | (buffer[7]<<8) ;
	reg_P2 = buffer[8] | (buffer[9]<<8) ;
	reg_P3 = buffer[10] | (buffer[11]<<8) ;
	reg_P4 = buffer[12] | (buffer[13]<<8) ;
	reg_P5 = buffer[14] | (buffer[15]<<8) ;
	reg_P6 = buffer[16] | (buffer[17]<<8) ;
	reg_P7 = buffer[18] | (buffer[19]<<8) ;
	reg_P8 = buffer[20] | (buffer[21]<<8) ;
	reg_P9 = buffer[22] | (buffer[23]<<8) ;

	reg_H1 = buffer[25];

	readmem(m_addr, BME280_REGISTER_H2, buffer, 7);
	reg_H2 = buffer[0] | (buffer[1]<<8) ;
	reg_H3 = buffer[2] ;
	reg_H4 = (buffer[3]<<4) | (buffer[4]&0x0f) ;
	reg_H5 = ((buffer[4]&0xf0) >> 4) | (buffer[5]<<4);
	reg_H6 = buffer[6] ;
}

void BME280::readSensors(double *temperature, double *pressure, double *humidity) {
  // read entire data buffer
	readmem(m_addr, BME280_REGISTER_PRESSUREDATA, buffer, 8);

	// temperature
	adc_TPH = ((int32_t)buffer[3] << 12) | ((int32_t)buffer[4] << 4) | (buffer[5] >> 4) ;
  var1 = ((((adc_TPH >> 3) - ((int32_t)reg_T1 << 1))) * ((int32_t)reg_T2)) >> 11;
	var2 = (((((adc_TPH >> 4) - ((int32_t)reg_T1)) * ((adc_TPH >> 4) - ((int32_t)reg_T1))) >> 12) * ((int32_t)reg_T3)) >> 14;
	t_fine = var1 + var2;
	float TT  =  (((t_fine * 5 + 128) >> 8) / 100.00) ;
  *temperature =  (((t_fine * 5 + 128) >> 8) / 100.00) ;

	// pressure
	adc_TPH = ((int32_t)buffer[0] << 12) | ((int32_t)buffer[1] << 4) | ((int32_t)buffer[2] >> 4) ;
	var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
	var2 = (((var1 >> 2) * (var1 >> 2)) >> 11 ) * ((int32_t)reg_P6);
	var2 = var2 + ((var1 * ((int32_t)reg_P5)) << 1);
	var2 = (var2 >> 2) + (((int32_t)reg_P4) << 16);
	var1 = (((reg_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13 )) >> 3) + ((((int32_t)reg_P2) * var1) >> 1)) >> 18;
	var1 = ((((32768 + var1)) * ((int32_t)reg_P1)) >> 15);
	if (var1 != 0) {
		p = (((uint32_t)(((int32_t) 1048576) - adc_TPH) - (var2 >> 12))) * 3125;
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

	// humidity
	adc_TPH = ((int32_t)buffer[6] << 8) | (buffer[7] );
	var1 = (t_fine - ((int32_t) 76800));
	var1 = (((((adc_TPH << 14) - (((int32_t)reg_H4) << 20) - (((int32_t)reg_H5) * var1)) +
			((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)reg_H6)) >> 10) * (((var1 *((int32_t)reg_H3)) >> 11) +
					((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)reg_H2) + 8192) >> 14));
	var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)reg_H1)) >> 4));
	var1 = (var1 < 0 ? 0 : var1);
	var1 = (var1 > 419430400 ? 419430400 : var1);
	*humidity =  (var1 >> 12) / 1024.0;
}
