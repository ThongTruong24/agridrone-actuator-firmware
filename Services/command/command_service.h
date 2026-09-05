#ifndef COMMAND_SERVICE_H
#define COMMAND_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

#define COMMAND_SERVICE_BATCH_CAPACITY 8U

typedef struct
{
    uint16_t command_id;
    uint8_t actuator_id;
    uint8_t value;
} command_service_command_t;

typedef struct
{
    uint32_t can_id;
    uint32_t timestamp_ms;
    command_service_command_t commands[COMMAND_SERVICE_BATCH_CAPACITY];
    uint8_t command_count;
    uint8_t source_node_id;
    uint8_t transfer_id;
    uint8_t priority;
} command_service_batch_t;

typedef struct
{
    uint32_t timestamp_ms;
    uint32_t update_count;
    uint32_t value_change_count;
    uint16_t command_id;
    uint8_t id;
    uint8_t value;
    bool valid;
} command_service_gripper_state_t;

typedef struct
{
    uint32_t received_batch_count;
    uint32_t applied_command_count;
    uint32_t rejected_command_count;
} command_service_statistics_t;

typedef void (*command_service_observer_t)(
    const command_service_batch_t *batch,
    void *user_reference);

void command_service_init(void);
void command_service_set_observer(command_service_observer_t observer,
                                  void *user_reference);
bool command_service_get_gripper(uint8_t gripper_id,
                                 command_service_gripper_state_t *out_state);
command_service_statistics_t command_service_get_statistics(void);

#endif /* COMMAND_SERVICE_H */
