
#include "INIT_SYS.h"
#include "RCC.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"


u8 buffer2[3]={'r','l','s'};

int main (void)
{
    initSys();
	
	gpiomode_t mode={.mode=gpio_mode_AF,.type=gpio_type_pp};
    gpiopin_t GpioTX={.port=GPIOA,.pin=PIN9,.speed=gpio_speed_high,.mode_t=&mode}; 
	gpiopin_t GpioRX={.port=GPIOA,.pin=PIN10,.speed=gpio_speed_high,.mode_t=&mode}; 
    gpio_initPin(&GpioTX);
    gpio_initPin(&GpioRX); 
	gpio_setAF(GPIO_AF7_USART1, GpioTX.port);
	gpio_setAF(GPIO_AF7_USART1, GpioRX.port);


    uart_cfg_t USART1_cfg={.oversampling=oversampling_16,.wordLength=word_length_8_mask,.parity_selection=parity_none,.stopBits=stop_bits_1_mask,.baudrate=115200};
    UART1_Init(&USART1_cfg);
    while(1)
    {
       UART_sendByte(buffer2[0]);
         
    }
}
