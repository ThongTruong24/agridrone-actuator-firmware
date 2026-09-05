#ifndef DRONECAN_PORT_H
#define DRONECAN_PORT_H

#include <stdbool.h>
#include <stdint.h>

#include "board_can.h"

typedef struct
{
    uint32_t queue_drop_count;
} dronecan_port_statistics_t;

bool dronecan_port_init(void);
bool dronecan_port_receive(board_can_frame_t *out_frame,
                           uint32_t timeout_ms);
dronecan_port_statistics_t dronecan_port_get_statistics(void);

#endif /* DRONECAN_PORT_H */
