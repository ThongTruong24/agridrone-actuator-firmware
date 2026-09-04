#include "can_task.h"

#include <stddef.h>
#include <stdint.h>

#include "app_config.h"
#include "dronecan.h"
#include "dronecan_config.h"
#include "main.h"
#include "queue.h"
#include "task.h"
#include "uavcan.protocol.NodeStatus.h"

#define TEST_UART_PERIOD_MS             1000U
#define TEST_CONSOLE_TASK_STACK_DEPTH    320U
#define TEST_CONSOLE_TASK_PRIORITY       (tskIDLE_PRIORITY + 2U)
#define TEST_CONSOLE_UART_TIMEOUT_MS      100U
#define TEST_CONSOLE_LINE_CAPACITY        160U
#define CAN_TEST_QUEUE_LENGTH              16U
#define CAN_EXTENDED_ID_MASK       0x1FFFFFFFUL
#define CAN_MESSAGE_DESCRIPTOR_MASK 0x00FFFFFFUL
#define CAN_FILTER_IDE_BIT         (1UL << 2U)
#define CAN_FILTER_RTR_BIT         (1UL << 1U)

#if (APP_TEST_CAN_ENABLE == 1)
typedef struct
{
    uint32_t id;
    uint8_t data[8];
    uint8_t dlc;
    uint8_t is_extended;
    uint8_t is_remote;
} CanRxFrame;
#endif

volatile uint32_t can_rx_queue_drop_count;
volatile uint32_t can_rx_hal_error_count;
volatile uint32_t can_rx_frame_count;
volatile uint32_t can_console_uart_error_count;

#if (APP_TEST_CAN_ENABLE == 1)
static CAN_HandleTypeDef *can_instance;
static QueueHandle_t can_test_queue;
#endif
#if ((APP_TEST_UART_ENABLE == 1) || (APP_TEST_CAN_ENABLE == 1))
static UART_HandleTypeDef *console_uart_instance;
#endif

#if (APP_TEST_CAN_ENABLE == 1)
static HAL_StatusTypeDef configure_message_filter(CAN_HandleTypeDef *handle,
                                                   uint32_t can_id,
                                                   uint32_t filter_bank)
{
    const uint32_t filter_word =
        ((can_id & CAN_EXTENDED_ID_MASK) << 3U) |
        CAN_FILTER_IDE_BIT;
    const uint32_t mask_word =
        (CAN_MESSAGE_DESCRIPTOR_MASK << 3U) |
        CAN_FILTER_IDE_BIT |
        CAN_FILTER_RTR_BIT;
    const CAN_FilterTypeDef filter = {
        .FilterIdHigh = (uint16_t) (filter_word >> 16U),
        .FilterIdLow = (uint16_t) filter_word,
        .FilterMaskIdHigh = (uint16_t) (mask_word >> 16U),
        .FilterMaskIdLow = (uint16_t) mask_word,
        .FilterFIFOAssignment = CAN_FILTER_FIFO0,
        .FilterBank = filter_bank,
        .FilterMode = CAN_FILTERMODE_IDMASK,
        .FilterScale = CAN_FILTERSCALE_32BIT,
        .FilterActivation = CAN_FILTER_ENABLE,
        .SlaveStartFilterBank = 14U,
    };

    return HAL_CAN_ConfigFilter(handle, &filter);
}

static HAL_StatusTypeDef can_start(CAN_HandleTypeDef *handle)
{
    HAL_StatusTypeDef status =
        configure_message_filter(handle,
                                 DRONECAN_FC_NODE_STATUS_CAN_ID,
                                 0U);
    if (status != HAL_OK)
    {
        return status;
    }

    status = configure_message_filter(handle,
                                      DRONECAN_FC_ARRAY_COMMAND_CAN_ID,
                                      1U);
    if (status != HAL_OK)
    {
        return status;
    }

    status = HAL_CAN_Start(handle);
    if (status != HAL_OK)
    {
        return status;
    }

    return HAL_CAN_ActivateNotification(handle,
                                         CAN_IT_RX_FIFO0_MSG_PENDING);
}
#endif

#if ((APP_TEST_UART_ENABLE == 1) || (APP_TEST_CAN_ENABLE == 1))
static void console_write(const uint8_t *data, uint16_t length)
{
    if (HAL_UART_Transmit(console_uart_instance,
                          data,
                          length,
                          TEST_CONSOLE_UART_TIMEOUT_MS) != HAL_OK)
    {
        can_console_uart_error_count++;
    }
}

static TickType_t ticks_until_period(TickType_t now,
                                     TickType_t last_tick,
                                     TickType_t period)
{
    const TickType_t elapsed = now - last_tick;

    return (elapsed >= period) ? 0U : (period - elapsed);
}
#endif

#if (APP_TEST_CAN_ENABLE == 1)
static size_t append_text(uint8_t *buffer,
                          size_t position,
                          const char *text)
{
    while ((*text != '\0') && (position < TEST_CONSOLE_LINE_CAPACITY))
    {
        buffer[position++] = (uint8_t) *text++;
    }

    return position;
}

static size_t append_hex(uint8_t *buffer,
                         size_t position,
                         uint32_t value,
                         uint8_t digits)
{
    static const uint8_t hex_digits[] = "0123456789ABCDEF";

    while ((digits > 0U) && (position < TEST_CONSOLE_LINE_CAPACITY))
    {
        const uint8_t shift = (uint8_t) ((digits - 1U) * 4U);
        buffer[position++] = hex_digits[(value >> shift) & 0x0FU];
        digits--;
    }

    return position;
}

static size_t append_uint32(uint8_t *buffer,
                            size_t position,
                            uint32_t value)
{
    uint8_t reversed_digits[10];
    uint8_t digit_count = 0U;

    do
    {
        reversed_digits[digit_count++] = (uint8_t) ('0' + (value % 10U));
        value /= 10U;
    }
    while ((value != 0U) && (digit_count < sizeof(reversed_digits)));

    while ((digit_count > 0U) &&
           (position < TEST_CONSOLE_LINE_CAPACITY))
    {
        buffer[position++] = reversed_digits[--digit_count];
    }

    return position;
}

static const char *node_health_text(uint8_t health)
{
    switch (health)
    {
        case UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK:
            return "OK";
        case UAVCAN_PROTOCOL_NODESTATUS_HEALTH_WARNING:
            return "WARNING";
        case UAVCAN_PROTOCOL_NODESTATUS_HEALTH_ERROR:
            return "ERROR";
        case UAVCAN_PROTOCOL_NODESTATUS_HEALTH_CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

static const char *node_mode_text(uint8_t mode)
{
    switch (mode)
    {
        case UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL:
            return "OPERATIONAL";
        case UAVCAN_PROTOCOL_NODESTATUS_MODE_INITIALIZATION:
            return "INITIALIZATION";
        case UAVCAN_PROTOCOL_NODESTATUS_MODE_MAINTENANCE:
            return "MAINTENANCE";
        case UAVCAN_PROTOCOL_NODESTATUS_MODE_SOFTWARE_UPDATE:
            return "SOFTWARE_UPDATE";
        case UAVCAN_PROTOCOL_NODESTATUS_MODE_OFFLINE:
            return "OFFLINE";
        default:
            return "UNKNOWN";
    }
}

static uint64_t dronecan_time_usec(void)
{
    static TickType_t previous_tick;
    static uint64_t accumulated_ticks;
    const TickType_t current_tick = xTaskGetTickCount();

    accumulated_ticks += (TickType_t) (current_tick - previous_tick);
    previous_tick = current_tick;

    return (accumulated_ticks * 1000000ULL) /
           (uint64_t) configTICK_RATE_HZ;
}

static void node_status_received(const DronecanNodeStatus *status,
                                 void *user_reference)
{
    uint8_t line[TEST_CONSOLE_LINE_CAPACITY];
    size_t position = 0U;

    (void) user_reference;

    position = append_text(line, position, "NodeStatus id=0x");
    position = append_hex(line, position, status->can_id, 8U);
    position = append_text(line, position, " src=");
    position = append_uint32(line, position, status->source_node_id);
    position = append_text(line, position, " uptime=");
    position = append_uint32(line, position, status->uptime_sec);
    position = append_text(line, position, "s health=");
    position = append_text(line,
                           position,
                           node_health_text(status->health));
    position = append_text(line, position, "(");
    position = append_uint32(line, position, status->health);
    position = append_text(line, position, ") mode=");
    position = append_text(line,
                           position,
                           node_mode_text(status->mode));
    position = append_text(line, position, "(");
    position = append_uint32(line, position, status->mode);
    position = append_text(line, position, ") sub=");
    position = append_uint32(line, position, status->sub_mode);
    position = append_text(line, position, " vendor=0x");
    position = append_hex(line,
                          position,
                          status->vendor_specific_status_code,
                          4U);
    position = append_text(line, position, " tid=");
    position = append_uint32(line, position, status->transfer_id);
    position = append_text(line, position, " prio=");
    position = append_uint32(line, position, status->priority);
    position = append_text(line, position, "\r\n");

    console_write(line, (uint16_t) position);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

static void array_command_received(
    const DronecanActuatorArrayCommand *array_command,
    void *user_reference)
{
    uint8_t line[TEST_CONSOLE_LINE_CAPACITY];
    size_t position = 0U;

    (void) user_reference;

    position = append_text(line, position, "ArrayCommand id=0x");
    position = append_hex(line, position, array_command->can_id, 8U);
    position = append_text(line, position, " src=");
    position = append_uint32(line,
                             position,
                             array_command->source_node_id);
    position = append_text(line, position, " timestamp=");
    position = append_uint32(line,
                             position,
                             array_command->timestamp_ms);
    position = append_text(line, position, "ms count=");
    position = append_uint32(line,
                             position,
                             array_command->command_count);
    position = append_text(line, position, " tid=");
    position = append_uint32(line,
                             position,
                             array_command->transfer_id);
    position = append_text(line, position, " prio=");
    position = append_uint32(line,
                             position,
                             array_command->priority);
    position = append_text(line, position, "\r\n");
    console_write(line, (uint16_t) position);

    for (uint8_t index = 0U;
         (index < array_command->command_count) &&
         (index < DRONECAN_ACTUATOR_COMMAND_CAPACITY);
         index++)
    {
        position = 0U;
        position = append_text(line, position, "  command[");
        position = append_uint32(line, position, index);
        position = append_text(line, position, "] actuator_id=");
        position = append_uint32(
            line,
            position,
            array_command->commands[index].actuator_id);
        position = append_text(line, position, " value=");
        position = append_uint32(line,
                                 position,
                                 array_command->commands[index].value);
        position = append_text(line, position, " command_id=");
        position = append_uint32(
            line,
            position,
            array_command->commands[index].command_id);
        position = append_text(line, position, "\r\n");
        console_write(line, (uint16_t) position);
    }

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
#endif

#if ((APP_TEST_UART_ENABLE == 1) || (APP_TEST_CAN_ENABLE == 1))
static void test_console_task(void *argument)
{
    TickType_t now = xTaskGetTickCount();
#if (APP_TEST_UART_ENABLE == 1)
    static const uint8_t hello_message[] = "hello\r\n";
    const TickType_t hello_period = pdMS_TO_TICKS(TEST_UART_PERIOD_MS);
    TickType_t last_hello_tick = now;
#endif
#if (APP_TEST_CAN_ENABLE == 1)
    const TickType_t cleanup_period =
        pdMS_TO_TICKS(DRONECAN_RX_CLEANUP_PERIOD_MS);
    TickType_t last_cleanup_tick = now;
    CanRxFrame frame;
#endif

    (void) argument;

#if (APP_TEST_CAN_ENABLE == 1)
    dronecan_init(node_status_received,
                  array_command_received,
                  NULL);

    if (can_start(can_instance) != HAL_OK)
    {
        Error_Handler();
    }
#endif

    for (;;)
    {
        TickType_t wait_ticks = portMAX_DELAY;

        now = xTaskGetTickCount();

#if (APP_TEST_UART_ENABLE == 1)
        wait_ticks = ticks_until_period(now,
                                        last_hello_tick,
                                        hello_period);
#endif

#if (APP_TEST_CAN_ENABLE == 1)
        const TickType_t cleanup_wait =
            ticks_until_period(now, last_cleanup_tick, cleanup_period);
        if (cleanup_wait < wait_ticks)
        {
            wait_ticks = cleanup_wait;
        }

        if (xQueueReceive(can_test_queue, &frame, wait_ticks) == pdPASS)
        {
            if ((frame.is_extended != 0U) &&
                (frame.is_remote == 0U))
            {
                (void) dronecan_process_rx_frame(frame.id,
                                                 frame.data,
                                                 frame.dlc,
                                                 dronecan_time_usec());
            }
        }
#else
        vTaskDelay(wait_ticks);
#endif

        now = xTaskGetTickCount();

#if (APP_TEST_UART_ENABLE == 1)
        if ((now - last_hello_tick) >= hello_period)
        {
            console_write(hello_message,
                          (uint16_t) (sizeof(hello_message) - 1U));
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            last_hello_tick = now;
        }
#endif

#if (APP_TEST_CAN_ENABLE == 1)
        if ((now - last_cleanup_tick) >= cleanup_period)
        {
            dronecan_cleanup_stale_transfers(dronecan_time_usec());
            last_cleanup_tick = now;
        }
#endif
    }
}
#endif

BaseType_t can_tasks_init(CAN_HandleTypeDef *can_handle,
                          UART_HandleTypeDef *console_uart)
{
#if ((APP_TEST_UART_ENABLE == 0) && (APP_TEST_CAN_ENABLE == 0))
    (void) can_handle;
    (void) console_uart;
    return pdPASS;
#else
    if (console_uart == NULL)
    {
        return pdFAIL;
    }

#if (APP_TEST_CAN_ENABLE == 1)
    if (can_handle == NULL)
    {
        return pdFAIL;
    }

    can_instance = can_handle;
    can_test_queue = xQueueCreate(CAN_TEST_QUEUE_LENGTH,
                                  sizeof(CanRxFrame));
    if (can_test_queue == NULL)
    {
        return pdFAIL;
    }
#else
    (void) can_handle;
#endif

    console_uart_instance = console_uart;

    if (xTaskCreate(test_console_task,
                    "test_console",
                    TEST_CONSOLE_TASK_STACK_DEPTH,
                    NULL,
                    TEST_CONSOLE_TASK_PRIORITY,
                    NULL) != pdPASS)
    {
#if (APP_TEST_CAN_ENABLE == 1)
        vQueueDelete(can_test_queue);
        can_test_queue = NULL;
#endif
        return pdFAIL;
    }

    return pdPASS;
#endif
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *handle)
{
#if (APP_TEST_CAN_ENABLE == 1)
    CAN_RxHeaderTypeDef header;
    CanRxFrame frame = {0};
    BaseType_t higher_priority_task_woken = pdFALSE;

    if ((handle != can_instance) || (can_test_queue == NULL))
    {
        return;
    }

    if (HAL_CAN_GetRxMessage(handle,
                             CAN_RX_FIFO0,
                             &header,
                             frame.data) != HAL_OK)
    {
        can_rx_hal_error_count++;
        return;
    }

    frame.id = (header.IDE == CAN_ID_EXT) ? header.ExtId : header.StdId;
    frame.dlc = (header.DLC <= 8U) ? (uint8_t) header.DLC : 8U;
    frame.is_extended = (header.IDE == CAN_ID_EXT) ? 1U : 0U;
    frame.is_remote = (header.RTR == CAN_RTR_REMOTE) ? 1U : 0U;
    can_rx_frame_count++;

    if (xQueueSendFromISR(can_test_queue,
                          &frame,
                          &higher_priority_task_woken) != pdPASS)
    {
        can_rx_queue_drop_count++;
    }

    portYIELD_FROM_ISR(higher_priority_task_woken);
#else
    (void) handle;
#endif
}
