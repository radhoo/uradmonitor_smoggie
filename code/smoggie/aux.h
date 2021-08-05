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

#define MIN(x,y) (x<y)?x:y

uint16_t copyBytes(void *dst, uint16_t dst_size, uint16_t dst_offset, void *src, uint8_t src_bytes);

uint16_t di(double val);
double id(uint16_t val);

uint16_t coordToInt16(double latlong, uint16_t multiplier);

uint8_t crc8(const uint8_t *buffer, uint8_t length);

uint32_t crc32(uint8_t *buffer, uint16_t length, uint32_t seed = 0xFFFFFFFFUL);

bool find(char *response,  char *key,  char *startwith,  char *stopwith, char *value, uint8_t size) ;

bool jsonKeyFind(char *response, const char *key, const char *delimiter, char *value,  uint8_t size, uint8_t offset = 2);

uint32_t hex2int(char *hex);
