/**
 *
 * License:  GPL v3
 * Project:  SMOGGIE is an ultra-low cost automated air quality monitor with a rain proof enclosure and a simple mount system to make installation easy. 
 *           It features a high quality laser scatering Particulate Matter sensor for PM1, PM2.5 and PM10 and an additional sensor for temperature, pressure and humidity. 
 *           It connects to the internet via Wifi and can be powered by a standard 5V micro-usb cable. Readings are accessed via the uRADMonitor API or decentralized via your local network. This monitor is lab tested for data accuracy.
 *
 * Copyright 2013-2015 Radu Motisan, radu.motisan@gmail.com
 * Copyright 2015-2021 Magnasci SRL, www.magnasci.com
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


#include "aux.h"
#include <string.h>
#include <math.h>

//  copy specified src_bytes from src to dst at specified offset
uint16_t copyBytes(void *dst, uint16_t dst_size, uint16_t dst_offset, void *src, uint8_t src_bytes) {
  if (!dst || !src_bytes || dst_size < dst_offset + src_bytes) return dst_offset;

  for (int i=0; i< src_bytes;i++)
    ((uint8_t *)dst)[dst_offset + i] = !src? 0 : ((uint8_t *)src)[src_bytes - i - 1]; // either 0 if empty source or most significant bytes go first
  return dst_offset + src_bytes;
}

// an uint16_t stores 16 bits, we use first bit for sign, and following 15 bits for number (0..32767)
// result is divided by 100 for a real with 2 decimals, max is 327.00
uint16_t di(double val) {
  uint16_t res = 0;
  uint16_t mask = 1<<15;
  if (val > 327 || val < -327) return res;
  if (val < 0) {
    val *= -1000;
    res = val / 10; // fix double approximation issue
    res |= mask;
  } else {
    val *= 1000;
    res = val / 10; // fix double approximation issue
  }
  return res;
}

// an uint16_t stores 16 bits, we use first bit for sign, and following 15 bits for number (0..32767)
// result is divided by 100 for a real with 2 decimals, max is 327.00
double id(uint16_t val) {
  double res = 0;
  uint16_t mask = 1<<15;
  // check negative number
  if (val & mask) {
    val &= ~mask; // remove sign bit
    res = val;
    res = -res;
  } else {
    res = val;
  }
  res /= 100.0; // restore the 2 decimals
  return res;
}

uint16_t coordToInt16(double latlong, uint16_t multiplier) {
  uint32_t res = fabs(latlong) * multiplier;
  if (latlong < 0) res |= 0x8000;
  return res & 0xFFFF;
}

//CRC-8/MAXIM - based on the CRC8 formulas by Dallas/Maxim
uint8_t crc8(const uint8_t *buffer, uint8_t length) {
  uint8_t crc = 0x00;
  for (int i=0;i<length;i++) {
    crc = crc ^ *(buffer++);
    for (int j=0;j<8;j++) {
      if (crc & 1)
          crc = (crc >> 1) ^ 0x8C;
      else
          crc = crc >> 1;
    }
  }
  return crc; // not-complemented!
}

// original CRC32
// note: if the seed is not "big" enough, first values in buffer will not have any influence
uint32_t crc32(uint8_t *buffer, uint16_t length, uint32_t seed) {
  uint32_t crc = seed;
  uint16_t i, j;
  for (i=0; i<length; i++) {
        crc = crc ^ *(buffer++);
    for (j=0; j<8; j++) {
      if (crc & 1)
            crc = (crc>>1) ^ 0xEDB88320UL ;
      else
            crc = crc >>1 ;
        }
    }
  return crc ^ 0xFFFFFFFFUL;
}

/*
 * Searches for a substring in the rigth part of a string after a given key
 * The substring should be delimited by two other substrings, startwith and stopwith
 * the result is returned in pointer 'value'
 * True on success, false if the search op didn't find any results
 */
bool find(char *response,  char *key,  char *startwith,  char *stopwith, char *value, uint8_t size) {
  memset(value, 0, size);
  char *s1 = strstr(response, key);
  uint8_t len1 = strlen(key);
  if (s1 && len1) {
    char *s2 = strstr(s1 + len1, startwith);
    uint8_t len2 = strlen(startwith);
    if (s2) {
      char *s3 = strstr(s2 + len2, stopwith);
      if (s3) {
        strncpy(value, s2 + len2, MIN(s3 - s2 - len2, size - 1) );
        return true;
      }
    }
  }
  return false;
}



/**
 * jsonKeyFind
 * finds a key and copies its value to the value output pointer
 * {"success":"ok"}
 * +CIFSR:APIP,"192.168.4.1"
 *  +CIFSR:STAIP,"192.168.2.106"
 */
bool jsonKeyFind(char *response, const char *key, const char *delimiter, char *value,  uint8_t size, uint8_t offset) {
  memset(value, 0, size);

  char *s1 = strstr(response, key);
  uint8_t len = strlen(key);
  if (s1 && len) {
    char *s2 = strstr(s1 + len + offset, delimiter);
    if (s2) {
      memset(value,0, size);
      strncpy(value, s1 + len + offset, MIN(s2 - s1 - len - offset, size - 1) );
      return true;
    }
  }
  return false;
}

uint8_t hexdigit(uint8_t digit) {
  if (digit >= '0' && digit <= '9') return digit  - '0';
    else if (digit >= 'a' && digit <='f') return digit - ('a' - 10);
    else if (digit >= 'A' && digit <='F') return digit - ('A' - 10);
    else return 0; //error
}

// take a hex string and convert it to a 32bit number (max 8 hex digits)
uint32_t hex2int(char *hex) {
    uint32_t val = 0;
     while (*hex) {
      // shift 4 to make space for new digit, and add the 4 bits of the new digit
      val = (val << 4) | (hexdigit(*hex++) & 0xF);
    }
    return val;
}
