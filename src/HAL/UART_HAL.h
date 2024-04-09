#ifndef UART_HAL_H
#define UART_HAL_H

#include "std_types.h"
#include "HAL_CFG.h"
#include "UART.h"

typedef struct{
    void* port;
    u8 pin;
    u64 AF_value;
}UART_pin_t;

typedef struct{
    u32 baudrate;
    frame_t* frame;
}UART_configuration_t;



typedef void(*TxCb )(void);
typedef void(*RxCb )(void);

void UART1_Init(void);
void UART_TxBufferAsync(u8* buffer, u16 length,TxCb cb );
void UART_RxBufferAsync(u8* buffer, u16 length,RxCb cb );


#endif