#include "board_config.h"

#if defined(BOARD_AGRIDRONE_F103C8)

#include "board_can.h"

#include <stddef.h>

#include "main.h"

#define BOARD_CAN_FILTER_BANK_CAPACITY 14U
#define BOARD_CAN_FILTER_IDE_BIT        (1UL << 2U)
#define BOARD_CAN_FILTER_RTR_BIT        (1UL << 1U)

extern CAN_HandleTypeDef hcan;

static board_can_rx_isr_handler_t receive_handler;
static void *receive_handler_user_reference;
static board_can_statistics_t statistics;

static HAL_StatusTypeDef configure_filter(
    const board_can_extended_filter_t *filter,
    uint32_t filter_bank)
{
    const uint32_t filter_word =
        ((filter->id & BOARD_CAN_EXTENDED_ID_MASK) << 3U) |
        BOARD_CAN_FILTER_IDE_BIT;
    const uint32_t mask_word =
        ((filter->mask & BOARD_CAN_EXTENDED_ID_MASK) << 3U) |
        BOARD_CAN_FILTER_IDE_BIT |
        BOARD_CAN_FILTER_RTR_BIT;
    const CAN_FilterTypeDef hal_filter = {
        .FilterIdHigh = (uint16_t) (filter_word >> 16U),
        .FilterIdLow = (uint16_t) filter_word,
        .FilterMaskIdHigh = (uint16_t) (mask_word >> 16U),
        .FilterMaskIdLow = (uint16_t) mask_word,
        .FilterFIFOAssignment = CAN_FILTER_FIFO0,
        .FilterBank = filter_bank,
        .FilterMode = CAN_FILTERMODE_IDMASK,
        .FilterScale = CAN_FILTERSCALE_32BIT,
        .FilterActivation = CAN_FILTER_ENABLE,
        .SlaveStartFilterBank = BOARD_CAN_FILTER_BANK_CAPACITY,
    };

    return HAL_CAN_ConfigFilter(&hcan, &hal_filter);
}

bool board_can_start(const board_can_extended_filter_t *filters,
                     size_t filter_count,
                     board_can_rx_isr_handler_t rx_isr_handler,
                     void *user_reference)
{
    if ((filters == NULL) ||
        (filter_count == 0U) ||
        (filter_count > BOARD_CAN_FILTER_BANK_CAPACITY) ||
        (rx_isr_handler == NULL))
    {
        return false;
    }

    statistics = (board_can_statistics_t) {0};
    receive_handler = rx_isr_handler;
    receive_handler_user_reference = user_reference;

    for (size_t index = 0U; index < filter_count; index++)
    {
        if (configure_filter(&filters[index], (uint32_t) index) != HAL_OK)
        {
            statistics.hardware_error_count++;
            return false;
        }
    }

    if (HAL_CAN_Start(&hcan) != HAL_OK)
    {
        statistics.hardware_error_count++;
        return false;
    }

    if (HAL_CAN_ActivateNotification(&hcan,
                                     CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        statistics.hardware_error_count++;
        return false;
    }

    return true;
}

board_can_statistics_t board_can_get_statistics(void)
{
    return statistics;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *handle)
{
    CAN_RxHeaderTypeDef header;
    board_can_frame_t frame = {0};

    if (handle != &hcan)
    {
        return;
    }

    if (HAL_CAN_GetRxMessage(handle,
                             CAN_RX_FIFO0,
                             &header,
                             frame.data) != HAL_OK)
    {
        statistics.hardware_error_count++;
        return;
    }

    frame.id = (header.IDE == CAN_ID_EXT) ? header.ExtId : header.StdId;
    frame.data_length = (header.DLC <= BOARD_CAN_CLASSIC_MAX_DATA_LENGTH)
                      ? (uint8_t) header.DLC
                      : BOARD_CAN_CLASSIC_MAX_DATA_LENGTH;
    frame.is_extended = header.IDE == CAN_ID_EXT;
    frame.is_remote = header.RTR == CAN_RTR_REMOTE;
    statistics.received_frame_count++;

    if ((receive_handler == NULL) ||
        !receive_handler(&frame, receive_handler_user_reference))
    {
        statistics.consumer_drop_count++;
    }
}

#endif /* BOARD_AGRIDRONE_F103C8 */
