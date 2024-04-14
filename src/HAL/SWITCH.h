#ifndef SWITCH_H
#define SWITCH_H

#include "std_types.h"
#include "SWITCH_CFG.h"

#define state_NOT_PRESSED       0
#define state_PRESSED           1
#define connection_PULL_UP      0
#define connection_PULL_DOWN    1

typedef struct{
    u32 * port;
    u32 pin;
    u8 state;
    u8 connection;
}switch_cfg_t;


/// @brief get switch state
/// @param state ,options:  state_xx
/// @param switch_id ,options:  switch_xx
void SWITCH_getStatus(u32 switch_id ,u8* state);

void SWITCH_Init(void);
#endif