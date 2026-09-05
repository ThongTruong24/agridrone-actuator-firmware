#ifndef ACTUATOR_APP_H
#define ACTUATOR_APP_H

#include <stdbool.h>
#include <stdint.h>

#define ACTUATOR_APP_GRIPPER_MIN_ID 1U
#define ACTUATOR_APP_GRIPPER_MAX_ID 4U
#define ACTUATOR_APP_GRIPPER_COUNT  4U

/*
 * Generic runtime value of one actuator channel. Future actuator groups can
 * reuse this type without placing mutable state in the Config layer.
 */
typedef struct
{
    uint32_t timestamp_ms;
    uint32_t update_count;
    uint32_t value_change_count;
    uint16_t command_id;
    uint8_t actuator_id;
    uint8_t value;
    bool valid;
} ActuatorChannelData;

/*
 * User-facing actuator state. Add arrays for future actuator types here while
 * retaining ActuatorChannelData as the common per-channel representation.
 */
typedef struct
{
    ActuatorChannelData grippers[ACTUATOR_APP_GRIPPER_COUNT];
    uint32_t applied_command_count;
    uint32_t unsupported_command_count;
} ActuatorAppData;

/* Read-only by convention; update it through actuator_app_apply_command(). */
extern ActuatorAppData actuator_app_data;

void actuator_app_init(void);

bool actuator_app_apply_command(uint32_t timestamp_ms,
                                uint8_t actuator_id,
                                uint8_t value,
                                uint16_t command_id);

bool actuator_app_get_gripper(uint8_t gripper_id,
                              ActuatorChannelData *out_data);

const ActuatorAppData *actuator_app_get_data(void);

#endif /* ACTUATOR_APP_H */
