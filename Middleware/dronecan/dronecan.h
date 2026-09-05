#ifndef DRONECAN_H
#define DRONECAN_H

#include <stdbool.h>
#include <stdint.h>

#define DRONECAN_ACTUATOR_COMMAND_CAPACITY 8U

typedef struct
{
    uint32_t can_id;
    uint32_t uptime_sec;
    uint16_t vendor_specific_status_code;
    uint8_t source_node_id;
    uint8_t transfer_id;
    uint8_t priority;
    uint8_t health;
    uint8_t mode;
    uint8_t sub_mode;
} DronecanNodeStatus;

typedef struct
{
    uint16_t command_id;
    uint8_t actuator_id;
    uint8_t value;
} DronecanActuatorCommand;

typedef struct
{
    uint32_t can_id;
    uint32_t timestamp_ms;
    DronecanActuatorCommand commands[DRONECAN_ACTUATOR_COMMAND_CAPACITY];
    uint8_t command_count;
    uint8_t source_node_id;
    uint8_t transfer_id;
    uint8_t priority;
} DronecanActuatorArrayCommand;

typedef void (*DronecanNodeStatusHandler)(const DronecanNodeStatus *status,
                                          void *user_reference);
typedef void (*DronecanActuatorArrayCommandHandler)(
    const DronecanActuatorArrayCommand *array_command,
    void *user_reference);

typedef enum
{
    DRONECAN_PROCESS_IDLE = 0,
    DRONECAN_PROCESS_FRAME,
    DRONECAN_PROCESS_ERROR,
} dronecan_process_result_t;

typedef struct
{
    uint32_t rx_error_count;
    uint32_t node_status_decode_error_count;
    uint32_t node_status_received_count;
    uint32_t array_command_decode_error_count;
    uint32_t array_command_received_count;
    uint32_t queue_drop_count;
    uint32_t hardware_received_frame_count;
    uint32_t hardware_error_count;
} dronecan_statistics_t;

void dronecan_set_node_status_handler(DronecanNodeStatusHandler handler,
                                      void *user_reference);
void dronecan_set_array_command_handler(
    DronecanActuatorArrayCommandHandler handler,
    void *user_reference);

bool dronecan_init(void);
dronecan_process_result_t dronecan_process(uint32_t timeout_ms);
dronecan_statistics_t dronecan_get_statistics(void);

#endif /* DRONECAN_H */
