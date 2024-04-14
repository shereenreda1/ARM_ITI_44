#ifndef UART_HAL_H
#define UART_HAL_H

#include "std_types.h"
#include "UART.h"


typedef void(*TxCb )(void);
typedef void(*RxCb )(void);

//void UART1_Init(void);
UART_Error_t UART_TxBufferAsync(u8* buffer, u16 length,TxCb cb );
UART_Error_t UART_RxBufferAsync(u8* buffer, u16 length,RxCb cb );


#endif