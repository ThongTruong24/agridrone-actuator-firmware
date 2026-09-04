#ifndef RTOS_H
#define RTOS_H

#include "FreeRTOS.h"
#include "stm32f1xx_hal.h"

BaseType_t rtos_init(CAN_HandleTypeDef *can_handle,
                     UART_HandleTypeDef *console_uart);

#endif /* RTOS_H */
