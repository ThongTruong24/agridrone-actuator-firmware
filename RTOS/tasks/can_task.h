#ifndef CAN_TASK_H
#define CAN_TASK_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "stm32f1xx_hal.h"

extern volatile uint32_t can_rx_queue_drop_count;
extern volatile uint32_t can_rx_hal_error_count;
extern volatile uint32_t can_rx_frame_count;
extern volatile uint32_t can_console_uart_error_count;

BaseType_t can_tasks_init(CAN_HandleTypeDef *can_handle,
                          UART_HandleTypeDef *console_uart);

#endif /* CAN_TASK_H */
