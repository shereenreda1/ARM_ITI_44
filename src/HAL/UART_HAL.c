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

extern u32 fclk;
extern const UART_pin_t UART_PINS[_number_of_pins];
extern const UART_configuration_t uart_config;


static void UART_configure(UART_configuration_t str_config){
    UART_configFrame(str_config.frame);
    UART_setBaudrate(str_config.baudrate,fclk);
   
}

void UART1_Init(void){
    gpiopin_t pinUart;
    gpiomode_t  pin_mode ={ .mode=gpio_mode_AF, .type=gpio_type_pp, .pupd=gpio_pupd_pu};
	pinUart.mode_t = &pin_mode;
	pinUart.speed= gpio_speed_medium;
	for(u32 idx=0;idx<_number_of_pins ;idx++){
		pinUart.pin =UART_PINS[idx].pin;
		pinUart.port=UART_PINS[idx].port;
		gpio_initPin(&pinUart);
        gpio_setAF(UART_PINS[idx].AF_value, UART_PINS[idx].port);
	}
    UART_configure(uart_config);
    UART_control(Enable,mode_Tx_Rx);
    NVIC_enableIRQ(NVIC_USART1_INTERRUPT);
}


void UART_TxBufferAsync(u8* buffer, u16 length,TxCb cb ){
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

void UART_RxBufferAsync(u8* buffer, u16 length,RxCb cb ){
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

void  USART1_IRQHandler(void){
    if(UART_TxDone()){
        UART_sendByte('s');
       // UART_disableInterrupt(TxEIE_mask);
        /*
        if(TxReq.buffer.pos < TxReq.buffer.size){
            UART_sendByte(TxReq.buffer.data[TxReq.buffer.pos]);
            TxReq.buffer.pos++;
        }else{
            UART_disableInterrupt(TxE);
            TxReq.state = ready;
            if(TxReq.cbf){
                TxReq.cbf();
            }
        }
        */
    }
    if(UART_IsRxDone()){
        if(RxReq.buffer.pos < RxReq.buffer.size){
            RxReq.buffer.data[RxReq.buffer.pos]=UART_getRx();
            RxReq.buffer.pos++;
        }else{
            UART_disableInterrupt(RxNEIE_mask);
            RxReq.state = ready;
            if(RxReq.cbf){
                RxReq.cbf();
            }
        }
    }
}