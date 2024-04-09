#include "UART.h"
#include "stdint.h"
#include "math.h"

#define oversampling        16
#define mantissaDiv_mask    0xfff0
#define mantissa_leftshift  4
#if oversampling == 16
    #define fractionDiv_mask 0xf
#elif oversampling == 8
    #define fractionDiv_mask 0x7
#endif
#define UART_Enable_Mask                0x2000
#define UART_Transmission_Enable_mask   0x8
#define UART_Reciever_Enable_Mask       0x4
#define TC_mask                         0x40

typedef struct{
    u32 SR;
    u32 DR;
    u32 BRR;
    u32 CR1;
    u32 CR2;
    u32 CR3;
    u32 GTPR;
}UART_reg_t;

volatile UART_reg_t* const UART1 = (UART_reg_t* const) 0x40011000; //fix



u8 UART_TxDone(void){
    return (u8)(UART1->SR & TxEIE_mask);
}

u8 UART_IsRxDone(void){
    return (u8)(UART1->SR & RxNEIE_mask);
}

u8 UART_getRx(void){
    return (u8)(UART1->DR);
}

void UART_sendByte(u8 byte){
    while(! UART_TxDone());
    UART1->DR=byte;
    //while(!(UART1->SR & TC_mask));
   
}

void UART_enableInterrupt(u8 interrupt){
    UART1->CR1 |= interrupt;
}

void UART_disableInterrupt(u8 interrupt){
    UART1->CR1 &= ~interrupt;
}

void UART_setBaudrate(u32 baudrate,u32 fclk){
    u32 mantissaDiv;
    u8 fractionDiv;
    u8 carry;
    f32 uartDiv =(f32) fclk/(oversampling * baudrate);
    u32 mantissa = (u32)uartDiv;
    u32 fraction = ceil(( uartDiv - mantissa )*oversampling);
    if(fraction >= oversampling){
        carry = fraction -(oversampling-1);
        mantissaDiv = (u32)(uartDiv + carry);
        fractionDiv = fraction & (oversampling -1); 
    }else{
        mantissaDiv = mantissa;
        fractionDiv = fraction;
    }
    UART1->BRR= (mantissaDiv << mantissa_leftshift)|(fractionDiv & fractionDiv_mask);
}

UART_Error_t UART_configFrame(frame_t* frame){
    UART_Error_t Ret_statusError =UART_enum_Nok;
    if((frame->dataBits != word_length_8_mask )&&
        (frame->dataBits != word_length_9_mask ) &&
        (frame->stopBits != stop_bits_1_mask) &&
        (frame->stopBits != stop_bits_2_mask) &&
        (frame->parity_enable != parity_enable_mask) &&
        (frame->parity_enable != parity_disable_mask)&&
        (frame->parityType != parity_even_selection)&&
        (frame->parityType != parity_odd_selection)){
            Ret_statusError = UART_enum_invalidValue;
        }
        else{
            Ret_statusError = UART_enum_Ok;
            UART1->CR1 |= (frame->dataBits)|(frame->parity_enable);
            UART1->CR1 |= frame->parityType;
            UART1->CR2 |= frame->stopBits;
            if(oversampling == 8){
                UART1->CR1 |= oversampling_8_mask;
            }
        }
    return Ret_statusError;
}

UART_Error_t UART_control(u8 status,u16 mode){
    UART_Error_t Ret_ErrorStatus = UART_enum_Nok;
    switch(status){
        case Enable:
            Ret_ErrorStatus =UART_enum_Ok;
            UART1->CR1 |= UART_Enable_Mask;
            u8* ptr =(u8*)&mode;
            ptr++;
            if(*ptr != 0x01){
                Ret_ErrorStatus = UART_enum_invalidValue;
            }else{
                Ret_ErrorStatus = UART_enum_Ok;
                switch(mode){
                    case mode_Rx_only:
                        UART1->CR1 |= UART_Reciever_Enable_Mask;
                        UART1->CR1 |= RxNEIE_mask;
                        break;
                    case mode_Tx_only:
                        UART1->CR1 |= UART_Transmission_Enable_mask;
                        UART1->CR1 |= TxEIE_mask;
                        break;
                    case mode_Tx_Rx:
                        UART1->CR1 |= (UART_Transmission_Enable_mask | UART_Reciever_Enable_Mask);
                        UART1->CR1 |=(TxEIE_mask|RxNEIE_mask);
                        break;
                    default:
                        Ret_ErrorStatus=UART_enum_invalidValue;
                        break;
                }
            }
            UART1->SR =0;
            break;
        case Disable:
            Ret_ErrorStatus =UART_enum_Ok;
            UART1->CR1 &= ~UART_Enable_Mask;
            break;
        default:
            Ret_ErrorStatus = UART_enum_invalidValue;
            break;
    }
    return Ret_ErrorStatus;
}