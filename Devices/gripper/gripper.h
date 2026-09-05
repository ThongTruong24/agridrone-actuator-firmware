#ifndef GRIPPER_H
#define GRIPPER_H

#include <stdbool.h>
#include <stdint.h>

#define GRIPPER_MIN_ID 1U
#define GRIPPER_MAX_ID 4U
#define GRIPPER_COUNT  4U

typedef struct
{
    uint32_t timestamp_ms;
    uint32_t update_count;
    uint32_t value_change_count;
    uint16_t command_id;
    uint8_t id;
    uint8_t value;
    bool valid;
} gripper_state_t;

typedef struct
{
    uint32_t applied_command_count;
    uint32_t rejected_command_count;
} gripper_statistics_t;

typedef enum
{
    GRIPPER_COMMAND_REJECTED = 0,
    GRIPPER_COMMAND_APPLIED_UNCHANGED,
    GRIPPER_COMMAND_APPLIED_CHANGED,
} gripper_command_result_t;

void gripper_init(void);

gripper_command_result_t gripper_apply_command(uint32_t timestamp_ms,
                                                uint8_t gripper_id,
                                                uint8_t value,
                                                uint16_t command_id);

bool gripper_get_state(uint8_t gripper_id, gripper_state_t *out_state);
gripper_statistics_t gripper_get_statistics(void);

#endif /* GRIPPER_H */
