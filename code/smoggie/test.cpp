#include <stdio.h>
#include <stdint.h>



  void sendCalCO2(uint16_t conc) {
  	//Cumulative sum of data = 256-(HEAD+LEN+CMD+DATA)%256
	uint8_t cal[6] = {0x11, 0x03, 0x03, 0x02, 0x58, 0x8F};
	cal[3] = (conc >> 8) & 0xFF;
	cal[4] = conc & 0xFF;
	cal[5] = 256 - (cal[0] + cal[1] + cal[2] + cal[3] + cal[4]) % 256;

	printf("%02X %02X %02X %02X %02X %02X",
		cal[0] , cal[1] , cal[2] , cal[3] , cal[4], cal[5]);
  }

int main() {
 sendCalCO2(400);

 return 0;
}
