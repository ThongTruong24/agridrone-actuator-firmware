#include "actuator_app.h"

#include <stddef.h>
#include <string.h>

#include "app_config.h"
#include "main.h"

ActuatorAppData actuator_app_data;

static bool is_gripper_id(uint8_t actuator_id)
{
    return (actuator_id >= ACTUATOR_APP_GRIPPER_MIN_ID) &&
           (actuator_id <= ACTUATOR_APP_GRIPPER_MAX_ID);
}

void actuator_app_init(void)
{
    (void) memset(&actuator_app_data, 0, sizeof(actuator_app_data));

    for (uint8_t index = 0U;
         index < ACTUATOR_APP_GRIPPER_COUNT;
         index++)
    {
        actuator_app_data.grippers[index].actuator_id =
            (uint8_t) (index + ACTUATOR_APP_GRIPPER_MIN_ID);
    }
}

bool actuator_app_apply_command(uint32_t timestamp_ms,
                                uint8_t actuator_id,
                                uint8_t value,
                                uint16_t command_id)
{
    if (!is_gripper_id(actuator_id))
    {
        actuator_app_data.unsupported_command_count++;
        return false;
    }

    ActuatorChannelData *const gripper =
        &actuator_app_data.grippers[actuator_id -
                                    ACTUATOR_APP_GRIPPER_MIN_ID];
    const bool value_changed = gripper->valid &&
                               (gripper->value != value);

    gripper->timestamp_ms = timestamp_ms;
    gripper->command_id = command_id;
    gripper->value = value;
    gripper->valid = true;
    gripper->update_count++;
    actuator_app_data.applied_command_count++;

    if (value_changed)
    {
        gripper->value_change_count++;

#if (APP_TEST_LED_GRIPPER == 1)
        if (actuator_id == (uint8_t) APP_TEST_LED_GRIPPER_ID)
        {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }
#endif
    }

    return true;
}

bool actuator_app_get_gripper(uint8_t gripper_id,
                              ActuatorChannelData *out_data)
{
    if ((out_data == NULL) || !is_gripper_id(gripper_id))
    {
        return false;
    }

    *out_data = actuator_app_data.grippers[gripper_id -
                                           ACTUATOR_APP_GRIPPER_MIN_ID];
    return true;
}

const ActuatorAppData *actuator_app_get_data(void)
{
    return &actuator_app_data;
}
