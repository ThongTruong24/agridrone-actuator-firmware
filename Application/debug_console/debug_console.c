#include "debug_console.h"

#include <stdbool.h>
#include <stddef.h>

#include "app_config.h"
#include "board_uart.h"
#include "command_service.h"
#include "health_service.h"

#define DEBUG_CONSOLE_HELLO_PERIOD_MS 1000U
#define DEBUG_CONSOLE_LINE_CAPACITY    160U

#if ((APP_TEST_UART_ENABLE == 1) || (APP_TEST_CAN_ENABLE == 1))
static void console_write(const uint8_t *data, size_t length)
{
    (void) board_uart_write(data, length);
}
#endif

#if (APP_TEST_CAN_ENABLE == 1)
static size_t append_text(uint8_t *buffer,
                          size_t position,
                          const char *text)
{
    while ((*text != '\0') && (position < DEBUG_CONSOLE_LINE_CAPACITY))
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

    while ((digits > 0U) && (position < DEBUG_CONSOLE_LINE_CAPACITY))
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
           (position < DEBUG_CONSOLE_LINE_CAPACITY))
    {
        buffer[position++] = reversed_digits[--digit_count];
    }

    return position;
}

static const char *node_health_text(uint8_t health)
{
    switch (health)
    {
        case HEALTH_SERVICE_NODE_HEALTH_OK:
            return "OK";
        case HEALTH_SERVICE_NODE_HEALTH_WARNING:
            return "WARNING";
        case HEALTH_SERVICE_NODE_HEALTH_ERROR:
            return "ERROR";
        case HEALTH_SERVICE_NODE_HEALTH_CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

static const char *node_mode_text(uint8_t mode)
{
    switch (mode)
    {
        case HEALTH_SERVICE_NODE_MODE_OPERATIONAL:
            return "OPERATIONAL";
        case HEALTH_SERVICE_NODE_MODE_INITIALIZATION:
            return "INITIALIZATION";
        case HEALTH_SERVICE_NODE_MODE_MAINTENANCE:
            return "MAINTENANCE";
        case HEALTH_SERVICE_NODE_MODE_SOFTWARE_UPDATE:
            return "SOFTWARE_UPDATE";
        case HEALTH_SERVICE_NODE_MODE_OFFLINE:
            return "OFFLINE";
        default:
            return "UNKNOWN";
    }
}

static void on_node_status(const health_service_node_status_t *status,
                           void *user_reference)
{
    uint8_t line[DEBUG_CONSOLE_LINE_CAPACITY];
    size_t position = 0U;

    (void) user_reference;

    position = append_text(line, position, "NodeStatus id=0x");
    position = append_hex(line, position, status->can_id, 8U);
    position = append_text(line, position, " src=");
    position = append_uint32(line, position, status->source_node_id);
    position = append_text(line, position, " uptime=");
    position = append_uint32(line, position, status->uptime_sec);
    position = append_text(line, position, "s health=");
    position = append_text(line, position, node_health_text(status->health));
    position = append_text(line, position, "(");
    position = append_uint32(line, position, status->health);
    position = append_text(line, position, ") mode=");
    position = append_text(line, position, node_mode_text(status->mode));
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
    console_write(line, position);
}

static void on_array_command(const command_service_batch_t *batch,
                             void *user_reference)
{
    uint8_t line[DEBUG_CONSOLE_LINE_CAPACITY];
    size_t position = 0U;

    (void) user_reference;

    position = append_text(line, position, "ArrayCommand id=0x");
    position = append_hex(line, position, batch->can_id, 8U);
    position = append_text(line, position, " src=");
    position = append_uint32(line, position, batch->source_node_id);
    position = append_text(line, position, " timestamp=");
    position = append_uint32(line, position, batch->timestamp_ms);
    position = append_text(line, position, "ms count=");
    position = append_uint32(line, position, batch->command_count);
    position = append_text(line, position, " tid=");
    position = append_uint32(line, position, batch->transfer_id);
    position = append_text(line, position, " prio=");
    position = append_uint32(line, position, batch->priority);
    position = append_text(line, position, "\r\n");
    console_write(line, position);

    for (uint8_t index = 0U;
         (index < batch->command_count) &&
         (index < COMMAND_SERVICE_BATCH_CAPACITY);
         index++)
    {
        position = 0U;
        position = append_text(line, position, "  command[");
        position = append_uint32(line, position, index);
        position = append_text(line, position, "] actuator_id=");
        position = append_uint32(line,
                                 position,
                                 batch->commands[index].actuator_id);
        position = append_text(line, position, " value=");
        position = append_uint32(line,
                                 position,
                                 batch->commands[index].value);
        position = append_text(line, position, " command_id=");
        position = append_uint32(line,
                                 position,
                                 batch->commands[index].command_id);
        position = append_text(line, position, "\r\n");
        console_write(line, position);
    }
}
#endif /* APP_TEST_CAN_ENABLE */

void debug_console_init(void)
{
#if (APP_TEST_CAN_ENABLE == 1)
    health_service_set_observer(on_node_status, NULL);
    command_service_set_observer(on_array_command, NULL);
#endif
}

void debug_console_process(uint32_t now_ms)
{
#if (APP_TEST_UART_ENABLE == 1)
    static const uint8_t hello_message[] = "hello\r\n";
    static uint32_t previous_hello_ms;
    static bool time_initialized;

    if (!time_initialized)
    {
        previous_hello_ms = now_ms;
        time_initialized = true;
    }
    else if ((uint32_t) (now_ms - previous_hello_ms) >=
             DEBUG_CONSOLE_HELLO_PERIOD_MS)
    {
        console_write(hello_message, sizeof(hello_message) - 1U);
        health_service_signal_activity();
        previous_hello_ms = now_ms;
    }
#else
    (void) now_ms;
#endif
}
