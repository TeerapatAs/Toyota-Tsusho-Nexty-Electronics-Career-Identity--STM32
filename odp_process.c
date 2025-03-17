#include "odp_process.h"
#include <stdio.h>
#define STATE_NORMAL    0
#define STATE_ABNORMAL  1
#define STATE_SHUTDOWN  2

#define STATE_YOO		3
#define BATTTHRESHOLD   20

static uint8_t calcSoC(uint8_t* ip_encodedSoC) {
  uint16_t TEMP = 0;
  uint16_t MaxSOC = 255;
  int i = 0;
  // Find average SoC
  while (i != 10) {
    TEMP += ip_encodedSoC[(int)i];
    i++;
  }
  TEMP /= 10;

  TEMP = ((MaxSOC - TEMP) * 100 ) / MaxSOC; //req00702-03
  return (TEMP);
}

void odp_process( uint8_t ip_abnormRst,
                  uint8_t* ip_encodedSoC,
                  uint8_t* op_normalLed,
                  uint8_t* op_abnormalLed,
                  uint8_t* op_d) {
  static state = STATE_NORMAL;
  static uint8_t normalLed;
  static uint8_t abnormalLed;
  static uint8_t d;
  static uint8_t abCnt = 0;
  uint8_t soc;
  uint8_t temp;


  soc = calcSoC(ip_encodedSoC);

  switch (state) {
    case STATE_NORMAL:
      normalLed = 1; abnormalLed = 0 ; d = soc;
      if (soc <= BATTTHRESHOLD) {
        if (abCnt < 3) {
          abCnt++;
          state = STATE_ABNORMAL;
        } else {
        	state = STATE_SHUTDOWN;
        }
      }

      break;

    case STATE_ABNORMAL:
      normalLed = 0; abnormalLed = 1; d = 0;

      if (!ip_abnormRst && (soc > BATTTHRESHOLD))
        state = STATE_NORMAL; // Goto normal

      break;

    case STATE_SHUTDOWN:
      normalLed = 0;
      d = 0;

      // TODO : Add 7 seg support
      if (op_abnormalLed) {
    	  op_abnormalLed = 0;
      }
      else {
    	  op_abnormalLed = 1;
      }

      state = STATE_SHUTDOWN;

      break;
  }


}
  /*
  switch (state) {
    case STATE_NORMAL:
      normalLed = 1; abnormalLed = 0 ; d = soc;
      if (soc <= BATTTHRESHOLD) {
        if (abCnt < 3) {
          abCnt++;
          state = STATE_ABNORMAL;
        } else {
        	state = STATE_SHUTDOWN;
        }
      }

      break;

    case STATE_ABNORMAL:
      normalLed = 0; abnormalLed = 1; d = 0;

      if (!ip_abnormRst && (soc > BATTTHRESHOLD))
        state = STATE_NORMAL; // Goto normal

      break;

    case STATE_SHUTDOWN:
      normalLed = 0;
      d = 0;

      // TODO : Add 7 seg support
      if (op_abnormalLed) {
    	  op_abnormalLed = 0;
      }
      else {
    	  op_abnormalLed = 1;
      }

      state = STATE_SHUTDOWN;

      break;
  }


}
