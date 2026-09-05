#include "command_service.h"

#include <stddef.h>

#include "app_config.h"
#include "dronecan.h"
#include "gripper.h"
#include "status_led.h"

static command_service_observer_t batch_observer;
static void *batch_observer_user_reference;
static command_service_statistics_t statistics;

static void on_array_command(
    const DronecanActuatorArrayCommand *array_command,
    void *user_reference)
{
    command_service_batch_t batch = {0};

    (void) user_reference;

    batch.can_id = array_command->can_id;
    batch.timestamp_ms = array_command->timestamp_ms;
    batch.command_count = array_command->command_count;
    batch.source_node_id = array_command->source_node_id;
    batch.transfer_id = array_command->transfer_id;
    batch.priority = array_command->priority;

    for (uint8_t index = 0U;
         (index < array_command->command_count) &&
         (index < COMMAND_SERVICE_BATCH_CAPACITY);
         index++)
    {
        const DronecanActuatorCommand *const source =
            &array_command->commands[index];
        command_service_command_t *const destination =
            &batch.commands[index];

        destination->actuator_id = source->actuator_id;
        destination->value = source->value;
        destination->command_id = source->command_id;

        const gripper_command_result_t result =
            gripper_apply_command(array_command->timestamp_ms,
                                  source->actuator_id,
                                  source->value,
                                  source->command_id);
        if (result != GRIPPER_COMMAND_REJECTED)
        {
            statistics.applied_command_count++;

#if (APP_TEST_LED_GRIPPER == 1)
            if ((result == GRIPPER_COMMAND_APPLIED_CHANGED) &&
                (source->actuator_id ==
                 (uint8_t) APP_TEST_LED_GRIPPER_ID))
            {
                status_led_toggle();
            }
#endif
        }
        else
        {
            statistics.rejected_command_count++;
        }
    }

    statistics.received_batch_count++;

    if (batch_observer != NULL)
    {
        batch_observer(&batch, batch_observer_user_reference);
    }
}

void command_service_init(void)
{
    statistics = (command_service_statistics_t) {0};
    gripper_init();
    dronecan_set_array_command_handler(on_array_command, NULL);
}

void command_service_set_observer(command_service_observer_t observer,
                                  void *user_reference)
{
    batch_observer = observer;
    batch_observer_user_reference = user_reference;
}

bool command_service_get_gripper(uint8_t gripper_id,
                                 command_service_gripper_state_t *out_state)
{
    gripper_state_t state;

    if ((out_state == NULL) || !gripper_get_state(gripper_id, &state))
    {
        return false;
    }

    out_state->timestamp_ms = state.timestamp_ms;
    out_state->update_count = state.update_count;
    out_state->value_change_count = state.value_change_count;
    out_state->command_id = state.command_id;
    out_state->id = state.id;
    out_state->value = state.value;
    out_state->valid = state.valid;
    return true;
}

command_service_statistics_t command_service_get_statistics(void)
{
    return statistics;
}
