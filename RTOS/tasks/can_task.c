#include "can_task.h"

#include <stddef.h>

#include "app_config.h"
#include "board_time.h"
#include "debug_console.h"
#include "dronecan.h"
#include "task.h"

#define TEST_CONSOLE_TASK_STACK_DEPTH 320U
#define TEST_CONSOLE_TASK_PRIORITY    (tskIDLE_PRIORITY + 2U)
#define TEST_CONSOLE_WAIT_MS          1000U

#if ((APP_TEST_UART_ENABLE == 1) || (APP_TEST_CAN_ENABLE == 1))
static void test_console_task(void *argument)
{
    (void) argument;

    debug_console_process(board_time_ms());

    for (;;)
    {
#if (APP_TEST_CAN_ENABLE == 1)
        (void) dronecan_process(TEST_CONSOLE_WAIT_MS);
#else
        vTaskDelay(pdMS_TO_TICKS(TEST_CONSOLE_WAIT_MS));
#endif
        debug_console_process(board_time_ms());
    }
}
#endif

BaseType_t can_tasks_init(void)
{
#if ((APP_TEST_UART_ENABLE == 0) && (APP_TEST_CAN_ENABLE == 0))
    return pdPASS;
#else
#if (APP_TEST_CAN_ENABLE == 1)
    if (!dronecan_init())
    {
        return pdFAIL;
    }
#endif

    return xTaskCreate(test_console_task,
                       "test_console",
                       TEST_CONSOLE_TASK_STACK_DEPTH,
                       NULL,
                       TEST_CONSOLE_TASK_PRIORITY,
                       NULL);
#endif
}
