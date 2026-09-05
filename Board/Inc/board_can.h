#ifndef BOARD_CAN_H
#define BOARD_CAN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BOARD_CAN_CLASSIC_MAX_DATA_LENGTH 8U
#define BOARD_CAN_EXTENDED_ID_MASK        0x1FFFFFFFUL

typedef struct
{
    uint32_t id;
    uint8_t data[BOARD_CAN_CLASSIC_MAX_DATA_LENGTH];
    uint8_t data_length;
    bool is_extended;
    bool is_remote;
} board_can_frame_t;

typedef struct
{
    uint32_t id;
    uint32_t mask;
} board_can_extended_filter_t;

typedef bool (*board_can_rx_isr_handler_t)(const board_can_frame_t *frame,
                                           void *user_reference);

typedef struct
{
    uint32_t received_frame_count;
    uint32_t hardware_error_count;
    uint32_t consumer_drop_count;
} board_can_statistics_t;

bool board_can_start(const board_can_extended_filter_t *filters,
                     size_t filter_count,
                     board_can_rx_isr_handler_t rx_isr_handler,
                     void *user_reference);

board_can_statistics_t board_can_get_statistics(void);

#endif /* BOARD_CAN_H */
