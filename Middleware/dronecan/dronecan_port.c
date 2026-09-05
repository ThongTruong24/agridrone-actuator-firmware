#include "dronecan_port.h"

#include <stddef.h>

#include "FreeRTOS.h"
#include "dronecan_config.h"
#include "queue.h"

#define DRONECAN_RX_QUEUE_LENGTH      16U
#define DRONECAN_MESSAGE_FILTER_MASK  0x00FFFFFFUL

static QueueHandle_t receive_queue;
static dronecan_port_statistics_t statistics;

static bool receive_from_isr(const board_can_frame_t *frame,
                             void *user_reference)
{
    BaseType_t higher_priority_task_woken = pdFALSE;

    (void) user_reference;

    if ((frame == NULL) || (receive_queue == NULL))
    {
        statistics.queue_drop_count++;
        return false;
    }

    if (xQueueSendFromISR(receive_queue,
                          frame,
                          &higher_priority_task_woken) != pdPASS)
    {
        statistics.queue_drop_count++;
        return false;
    }

    portYIELD_FROM_ISR(higher_priority_task_woken);
    return true;
}

bool dronecan_port_init(void)
{
    const board_can_extended_filter_t filters[] = {
        {
            .id = DRONECAN_FC_NODE_STATUS_CAN_ID,
            .mask = DRONECAN_MESSAGE_FILTER_MASK,
        },
        {
            .id = DRONECAN_FC_ARRAY_COMMAND_CAN_ID,
            .mask = DRONECAN_MESSAGE_FILTER_MASK,
        },
    };

    statistics = (dronecan_port_statistics_t) {0};
    receive_queue = xQueueCreate(DRONECAN_RX_QUEUE_LENGTH,
                                 sizeof(board_can_frame_t));
    if (receive_queue == NULL)
    {
        return false;
    }

    if (!board_can_start(filters,
                         sizeof(filters) / sizeof(filters[0]),
                         receive_from_isr,
                         NULL))
    {
        vQueueDelete(receive_queue);
        receive_queue = NULL;
        return false;
    }

    return true;
}

bool dronecan_port_receive(board_can_frame_t *out_frame,
                           uint32_t timeout_ms)
{
    if ((out_frame == NULL) || (receive_queue == NULL))
    {
        return false;
    }

    return xQueueReceive(receive_queue,
                         out_frame,
                         pdMS_TO_TICKS(timeout_ms)) == pdPASS;
}

dronecan_port_statistics_t dronecan_port_get_statistics(void)
{
    return statistics;
}
