#include "HAL_CFG.h"
#include "UART_HAL.h"
#include "GPIO.h"

const UART_pin_t UART_PINS[_number_of_pins]={
    [Tx]={
        .pin = PIN9,
        .port = GPIOA,
        .AF_value =GPIO_AF7_USART1

    },
    [Rx]={
        .pin = PIN10,
        .port = GPIOA,
        .AF_value =GPIO_AF7_USART1
    }
};
const frame_t frame ={
    .stopBits = stop_bits_1_mask,
    .dataBits = word_length_8_mask,
    .parity_enable = parity_disable_mask
};
const UART_configuration_t uart_config ={
    .baudrate = 9600,
    .frame = &frame
};