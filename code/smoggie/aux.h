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
