#ifndef ACTUATOR_APP_H
#define ACTUATOR_APP_H

#include <stdbool.h>
#include <stdint.h>

#define ACTUATOR_APP_GRIPPER_MIN_ID 1U
#define ACTUATOR_APP_GRIPPER_MAX_ID 4U
#define ACTUATOR_APP_GRIPPER_COUNT  4U

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
 * User-facing snapshot of all actuator data. Future actuator groups belong
 * here; fixed hardware and test settings remain in Config/app_config.h.
 */
typedef struct
{
    ActuatorChannelData grippers[ACTUATOR_APP_GRIPPER_COUNT];
    uint32_t received_batch_count;
    uint32_t applied_command_count;
    uint32_t unsupported_command_count;
} ActuatorAppData;

void actuator_app_init(void);

bool actuator_app_get_gripper(uint8_t gripper_id,
                              ActuatorChannelData *out_data);

bool actuator_app_get_data(ActuatorAppData *out_data);

#endif /* ACTUATOR_APP_H */
