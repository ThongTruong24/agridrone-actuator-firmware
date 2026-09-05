#include "rtos.h"

#include "actuator_app.h"
#include "can_task.h"
#include "task.h"

static void fatal_stop(void)
{
    taskDISABLE_INTERRUPTS();
    for (;;)
    {
    }
}

BaseType_t rtos_init(const void *legacy_can_handle,
                     const void *legacy_console_uart)
{
    (void) legacy_can_handle;
    (void) legacy_console_uart;

    actuator_app_init();
    return can_tasks_init();
}

void vApplicationStackOverflowHook(TaskHandle_t task,
                                   char *task_name)
{
    (void) task;
    (void) task_name;
    fatal_stop();
}

void vApplicationMallocFailedHook(void)
{
    fatal_stop();
}
