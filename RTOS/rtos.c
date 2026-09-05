#include "rtos.h"

#include "actuator_app.h"
#include "can_task.h"
#include "main.h"
#include "task.h"

BaseType_t rtos_init(CAN_HandleTypeDef *can_handle,
                     UART_HandleTypeDef *console_uart)
{
    actuator_app_init();
    return can_tasks_init(can_handle, console_uart);
}

void vApplicationStackOverflowHook(TaskHandle_t task,
                                   char *task_name)
{
    (void) task;
    (void) task_name;

    Error_Handler();
}

void vApplicationMallocFailedHook(void)
{
    Error_Handler();
}
