#include "actuator_app.h"

#include <stddef.h>

#include "command_service.h"
#include "debug_console.h"
#include "health_service.h"

void actuator_app_init(void)
{
    command_service_init();
    health_service_init();
    debug_console_init();
}

bool actuator_app_get_gripper(uint8_t gripper_id,
                              ActuatorChannelData *out_data)
{
    command_service_gripper_state_t state;

    if ((out_data == NULL) ||
        !command_service_get_gripper(gripper_id, &state))
    {
        return false;
    }

    out_data->timestamp_ms = state.timestamp_ms;
    out_data->update_count = state.update_count;
    out_data->value_change_count = state.value_change_count;
    out_data->command_id = state.command_id;
    out_data->actuator_id = state.id;
    out_data->value = state.value;
    out_data->valid = state.valid;
    return true;
}

bool actuator_app_get_data(ActuatorAppData *out_data)
{
    if (out_data == NULL)
    {
        return false;
    }

    *out_data = (ActuatorAppData) {0};

    for (uint8_t index = 0U;
         index < ACTUATOR_APP_GRIPPER_COUNT;
         index++)
    {
        const uint8_t id = (uint8_t) (ACTUATOR_APP_GRIPPER_MIN_ID + index);
        (void) actuator_app_get_gripper(id, &out_data->grippers[index]);
    }

    const command_service_statistics_t statistics =
        command_service_get_statistics();
    out_data->received_batch_count = statistics.received_batch_count;
    out_data->applied_command_count = statistics.applied_command_count;
    out_data->unsupported_command_count = statistics.rejected_command_count;
    return true;
}
