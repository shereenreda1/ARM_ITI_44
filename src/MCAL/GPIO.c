#include "GPIO.h"

#define OSPEEDR_bit_offset 2
#define MODER_bit_offset 2
#define OTYPER_bit_offset 1
#define PUPDR_bit_offset 2
#define AFR_bit_offset 4

GPIO_Error_t gpio_initPin(gpiopin_t * pin){
	GPIO_Error_t Ret_ErrorStatus = GPIO_enum_Nok;
	if((pin->mode_t->mode != gpio_mode_in)&&
		(pin->mode_t->mode != gpio_mode_out)&&
		(pin->mode_t->mode != gpio_mode_analog)&&
		(pin->mode_t->mode != gpio_mode_AF)&&
		(pin->mode_t->pupd != gpio_pupd_no_pu_pd)&&
		(pin->mode_t->pupd != gpio_pupd_pd)&&
		(pin->mode_t->pupd != gpio_pupd_pu)&&
		(pin->mode_t->type != gpio_type_pp)&&
		(pin->mode_t->type != gpio_type_od) &&
		(pin->speed != gpio_speed_low)&&
		(pin->speed != gpio_speed_medium)&&
		(pin->speed != gpio_speed_high)&&
		(pin->speed != gpio_speed_veryhigh)&&
		(pin->port != GPIOA)&&
		(pin->port != GPIOB)&&
		(pin->port != GPIOC)&&
		(pin->port != GPIOD)&&
		(pin->port != GPIOE)&&
		(pin->port != GPIOH)&&
		(pin->pin < PIN0) &&
		(pin->pin > PIN31)){
			Ret_ErrorStatus = GPIO_enum_invalidValue;
	}
	else{
		((gpio_t*)pin->port) -> MODER |= (pin->mode_t ->mode)<<(MODER_bit_offset*(pin->pin));
		((gpio_t*)pin->port) -> PUPDR |= (pin->mode_t ->pupd)<<(PUPDR_bit_offset*(pin->pin));
		if((pin->mode_t ->mode) == gpio_mode_out || (pin->mode_t ->mode) == gpio_mode_AF){
			((gpio_t*)pin->port) -> OTYPER |= (pin->mode_t ->type)<<(OTYPER_bit_offset*(pin->pin));
			((gpio_t*)pin->port) -> OSPEEDR |= (pin->speed)<<(OSPEEDR_bit_offset*(pin->pin));

		}
	}
	/*	u32 pin_location_in_OSPEEDR = (pin->speed)<<(pin->pin)*OSPEEDR_bit_offset;
	((gpio_t*)pin->port) -> OSPEEDR= pin_location_in_OSPEEDR ;

	switch ((gpio_t*)pin->mode){
		case gpio_mode_analog_no_pu_pd:
	 	case gpio_mode_in_floationg:
		case gpio_mode_in_pu:
		case gpio_mode_in_pd:
			((gpio_t*)pin->port) -> MODER = gpio_mode_in;
			((gpio_t*)pin->port) -> PUPDR = ((gpio_t*)pin->mode);
			break;
		case gpio_mode_out_pp_no_pu_pd:
		case gpio_mode_out_pp_pu:
		case gpio_mode_out_pp_pd:
			((gpio_t*)pin->port) -> MODER = gpio_mode_out;
			((gpio_t*)pin->port) -> PUPDR = ((gpio_t*)pin->mode);
			break;

	}*/
	return Ret_ErrorStatus;
}
GPIO_Error_t gpio_setPinValue(void* port,u32 pin,u8 value){
	GPIO_Error_t Ret_ErrorStatus =GPIO_enum_Nok;
	if(!port){
		Ret_ErrorStatus=GPIO_enum_NullPointer;
	}
	else{
		Ret_ErrorStatus= GPIO_enum_Ok;
		((gpio_t*)port) -> BSRR = (value<<pin);
	}
	return Ret_ErrorStatus;
}
u8 gpio_getPinValue(void* port,u32 pin){
	return (((gpio_t*)port)->IDR &(1<<pin))>>pin;
}

GPIO_Error_t gpio_setAF(u64 value, void* port){
	GPIO_Error_t Ret_ErrorStatus = GPIO_enum_Nok;
	if(!port){
		Ret_ErrorStatus =GPIO_enum_NullPointer;
	}
	else{
		u32 * ptr = (u32*) &value;
		ptr++;
		switch(*ptr){
			case 1:
				Ret_ErrorStatus = GPIO_enum_Ok;
				((gpio_t *)port)->AFRL |= (u32) value;
				break;
			case 2:
				Ret_ErrorStatus = GPIO_enum_Ok;
				((gpio_t *)port)->AFRH |= (u32) value;
				break;
			default:
				Ret_ErrorStatus = GPIO_enum_invalidValue;
		}
	}
	return Ret_ErrorStatus;
}
