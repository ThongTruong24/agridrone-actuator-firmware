#include "gripper.h"

#include <stddef.h>
#include <string.h>

static gripper_state_t states[GRIPPER_COUNT];
static gripper_statistics_t statistics;

static bool is_valid_id(uint8_t gripper_id)
{
    return (gripper_id >= GRIPPER_MIN_ID) &&
           (gripper_id <= GRIPPER_MAX_ID);
}

void gripper_init(void)
{
    (void) memset(states, 0, sizeof(states));
    statistics = (gripper_statistics_t) {0};

    for (uint8_t index = 0U; index < GRIPPER_COUNT; index++)
    {
        states[index].id = (uint8_t) (index + GRIPPER_MIN_ID);
    }
}

gripper_command_result_t gripper_apply_command(uint32_t timestamp_ms,
                                                uint8_t gripper_id,
                                                uint8_t value,
                                                uint16_t command_id)
{
    if (!is_valid_id(gripper_id))
    {
        statistics.rejected_command_count++;
        return GRIPPER_COMMAND_REJECTED;
    }

    gripper_state_t *const state =
        &states[gripper_id - GRIPPER_MIN_ID];
    const bool value_changed = state->valid && (state->value != value);

    state->timestamp_ms = timestamp_ms;
    state->command_id = command_id;
    state->value = value;
    state->valid = true;
    state->update_count++;
    statistics.applied_command_count++;

    if (value_changed)
    {
        state->value_change_count++;
        return GRIPPER_COMMAND_APPLIED_CHANGED;
    }

    return GRIPPER_COMMAND_APPLIED_UNCHANGED;
}

bool gripper_get_state(uint8_t gripper_id, gripper_state_t *out_state)
{
    if ((out_state == NULL) || !is_valid_id(gripper_id))
    {
        return false;
    }

    *out_state = states[gripper_id - GRIPPER_MIN_ID];
    return true;
}

gripper_statistics_t gripper_get_statistics(void)
{
    return statistics;
}
