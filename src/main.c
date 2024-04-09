#include "LED.h"
#include "INIT_SYS.h"
#include "UART_HAL.h"
#include "UART.h"
#include "std_types.h"

void task1(){
	led_setStatus(led_green,led_state_on);
	led_setStatus(led_red,led_state_off);
	led_setStatus(led_red,led_state_on);
	led_setStatus(led_green,led_state_off);
	
}
/**
 * led_setStatus(led_green,led_state_on);
	  led_setStatus(led_red,led_state_on);
       // Add your code here.
	  for(u32 i=0;i<10;i++);
	  led_setStatus(led_red,led_state_off);
	  led_setStatus(led_green,led_state_on);
	  for(u32 i=0;i<10;i++);
*/
int main(int argc, char* argv[]){
	//initSys_setCBF(task1);
    initSys();
	//led_init();
	
	UART1_Init();
    // Infinite loop
	//u8 len=7;
	//u8 buff[]={'s','h','e','r','e','e','n'};

  while (1)
    {
		UART_sendByte('s');
		//UART_TxBufferAsync(buff,len,NULL);
		//UART_getRx();
    }
}

