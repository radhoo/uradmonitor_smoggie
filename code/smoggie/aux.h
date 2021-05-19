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
