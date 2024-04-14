#include "UART_HAL.h"
#include "UART.h"
#include "GPIO.h"
#include "NVIC.h"

#define ready 0
#define busy 1

typedef struct{
    u8* data;
    u32 pos;
    u32 size;
}buffer_t;

typedef struct{
    buffer_t buffer;
    u8 state;
    TxCb cbf;
}TxReq_t;

typedef struct{
    buffer_t buffer;
    u8 state;
    RxCb cbf;
}RxReq_t;

TxReq_t TxReq;
RxReq_t RxReq;


UART_Error_t UART_TxBufferAsync(u8* buffer, u16 length,TxCb cb ){
    UART_Error_t Ret_ErrorStatus =UART_enum_Nok;
    if(!buffer){
        Ret_ErrorStatus = UART_enum_NullPointer;
    }else{
        Ret_ErrorStatus =UART_enum_Ok;
        if(TxReq.state == ready){
            TxReq.buffer.data=buffer;
            TxReq.buffer.size=length;
            TxReq.buffer.pos=0;
            TxReq.state=busy;
            TxReq.cbf=cb;
            UART_sendByte(TxReq.buffer.data[0]);
            TxReq.buffer.pos++;
            UART_enableInterrupt(TxEIE_mask);
        }
    }
    return Ret_ErrorStatus;
    
}

UART_Error_t UART_RxBufferAsync(u8* buffer, u16 length,RxCb cb ){
    UART_Error_t Ret_ErrorStatus =UART_enum_Nok;
    if(!buffer){
        Ret_ErrorStatus = UART_enum_NullPointer;
    }else{
        Ret_ErrorStatus =UART_enum_Ok;
        if(RxReq.state == ready){
        UART_disableInterrupt(RxNEIE_mask);
        RxReq.buffer.data=buffer;
        RxReq.buffer.size=length;
        RxReq.buffer.pos=0;
        RxReq.state=busy;
        RxReq.cbf=cb;
        UART_enableInterrupt(RxNEIE_mask);
        }
    }
    return Ret_ErrorStatus;
}


void  USART1_IRQHandler(void){
    if(UART_TxDone()){
        UART_disableInterrupt(TxEIE_mask);
        
        if(TxReq.buffer.pos < TxReq.buffer.size){
            UART_sendByte(TxReq.buffer.data[TxReq.buffer.pos]);
            TxReq.buffer.pos++;
        }else{
            UART_disableInterrupt(TxEIE_mask);
            TxReq.state = ready;
            TxReq.buffer.pos=0;
            if(TxReq.cbf){
                TxReq.cbf();
            }
        }
        
          
    }
    if(UART_IsRxDone()){
        if(RxReq.buffer.pos < RxReq.buffer.size){
            u8 data;
            UART_getRx(&data);
            RxReq.buffer.data[RxReq.buffer.pos]=data;
            RxReq.buffer.pos++;
        }else if(RxReq.buffer.pos == RxReq.buffer.size){
            UART_disableInterrupt(RxNEIE_mask);
            RxReq.state = ready;
            RxReq.buffer.pos=0;
            if(RxReq.cbf){
                RxReq.cbf();
            }
        }
    }
}
