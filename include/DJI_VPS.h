#ifndef __DJI_VPS_H__
#define __DJI_VPS_H__

#include "stdio.h"

#define THROTTLE_CHANNEL    2
#define AILERON_CHANNEL     0
#define RUDDER_CHANNEL      3
#define ELEVATOR_CHANNEL    1
#define P1_CHANNEL          4
#define S1_CHANNEL          5
#define S2_CHANNEL          6

#define CHANNEL_MIN         364    
#define CHANNEL_MAX         1684    

#define SW_POS_1_VALUE      1541    
#define SW_POS_2_VALUE      1024   
#define SW_POS_3_VALUE      511     

enum{
    SW_POS_1,
    SW_POS_2,
    SW_POS_3
};

typedef struct{
    uint8_t throttle;
    uint8_t aileron;
    uint8_t rudder;
    uint8_t elevator;
    uint8_t p1;
    uint8_t s1;
    uint8_t s2;
    uint8_t err;
}channels_control_t;

void sbusInit(void);

#endif
