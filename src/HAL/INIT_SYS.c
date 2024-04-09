#include "RCC.h"
#include "GPIO.h"
#include "SYSTICK.h"
#include "INIT_SYS.h"
#include "UART.h"

static pfun CallBack;
u32 fclk = 21000000;
Peripheral_Enable_Error_t initSys(void){
	Peripheral_Enable_Error_t Ret_Error_status_t = Peripheral_Enable_enumNok;
	RCC_ErrorStatus_t error_status = enableClock( RCC_EN_HSE);
	/*if(error_status == RCC_enumOk){
		error_status = configurePLL(3,200,RCC_PLLCFGR_PLLP_DIV2,RCC_PLL_SRC_HSI);
	}
	if(error_status == RCC_enumOk){
		error_status = enableClock( RCC_EN_PLL);
	}
	if(error_status == RCC_enumOk){
		error_status=selectSysClock(RCC_CLK_PLL);
	}*/
	if(error_status == RCC_enumOk){
		error_status = enableClock( RCC_EN_HSE);
	}
	if(error_status == RCC_enumOk){
		error_status = configure_prescalers(RCC_PRE_AHB_SYSCLK_NOT_DIVIDED);//for systick: ahb/8
	}
	if(error_status == RCC_enumOk){
		error_status = configure_prescalers(RCC_PRE_APB2_DIV_BY_4);
	}
	if(error_status == RCC_enumOk){
		error_status = controlPeripheral(RCC_APB2_USART1EN,RCC_Peripheral_Enable); //fix
		
	}
	if(error_status == RCC_enumOk){
		error_status = controlPeripheral(RCC_AHB1_GPIOAEN,RCC_Peripheral_Enable); //fix
	}
	if(error_status == RCC_enumOk){
		Ret_Error_status_t = Peripheral_Enable_enumOk;
	}
	/*
	SYSTICK_stop();
	SYSTICK_setTimeMS(3);
	SYSTICK_setCallBackFunction(CallBack);
		//initSys_setCBF);
	SYSTICK_start(mode_Periodic);	
	*/
	return Ret_Error_status_t;
}

void initSys_setCBF(pfun cbf){
	if(cbf){
		CallBack=cbf;
	}
}
