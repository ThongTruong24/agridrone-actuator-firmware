#ifndef DRONECAN_H
#define DRONECAN_H

#include <stdint.h>

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

typedef void (*DronecanNodeStatusHandler)(const DronecanNodeStatus *status,
                                          void *user_reference);

#define DRONECAN_ACTUATOR_COMMAND_CAPACITY 8U

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

typedef void (*DronecanActuatorArrayCommandHandler)(
    const DronecanActuatorArrayCommand *array_command,
    void *user_reference);

extern volatile uint32_t dronecan_rx_error_count;
extern volatile uint32_t dronecan_node_status_decode_error_count;
extern volatile uint32_t dronecan_node_status_received_count;
extern volatile uint32_t dronecan_array_command_decode_error_count;
extern volatile uint32_t dronecan_array_command_received_count;

void dronecan_init(DronecanNodeStatusHandler node_status_handler,
                   DronecanActuatorArrayCommandHandler array_command_handler,
                   void *user_reference);

int16_t dronecan_process_rx_frame(uint32_t extended_can_id,
                                  const uint8_t *data,
                                  uint8_t data_length,
                                  uint64_t timestamp_usec);

void dronecan_cleanup_stale_transfers(uint64_t timestamp_usec);

#endif /* DRONECAN_H */
