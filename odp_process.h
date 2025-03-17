#ifndef __ODP_PROCESS__
#define __ODP_PROCESS__
#include <stdint.h>

static uint8_t calcSoC(uint8_t*);
void odp_process(uint8_t ip_abnormRst, uint8_t* ip_encodedSoC, uint8_t* op_normalLed, uint8_t* op_abnormalLed, uint8_t* op_d);




#endif
