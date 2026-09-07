#include "gripper.h"

#include <stddef.h>
#include <string.h>

#include "board_pwm.h"

#define GRIPPER_PWM_MIN_PULSE_US   1000U
#define GRIPPER_PWM_MAX_PULSE_US   2000U
#define GRIPPER_PWM_PULSE_RANGE_US \
    (GRIPPER_PWM_MAX_PULSE_US - GRIPPER_PWM_MIN_PULSE_US)

static gripper_state_t states[GRIPPER_COUNT];
static gripper_statistics_t statistics;

static bool is_valid_id(uint8_t gripper_id)
{
    return (gripper_id >= GRIPPER_MIN_ID) &&
           (gripper_id <= GRIPPER_MAX_ID);
}

static uint8_t clamp_value(uint32_t value)
{
    return (value > UINT8_MAX) ? UINT8_MAX : (uint8_t) value;
}

static uint16_t value_to_pulse_width_us(uint32_t value)
{
    const uint8_t clamped_value = clamp_value(value);

    return (uint16_t)
        (GRIPPER_PWM_MIN_PULSE_US +
         (((uint32_t) clamped_value * GRIPPER_PWM_PULSE_RANGE_US) /
          UINT8_MAX));
}

void gripper_init(void)
{
    (void) memset(states, 0, sizeof(states));
    statistics = (gripper_statistics_t) {0};

    for (uint8_t index = 0U; index < GRIPPER_COUNT; index++)
    {
        states[index].id = (uint8_t) (index + GRIPPER_MIN_ID);
    }

    (void) board_pwm_init();
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

    const uint8_t clamped_value = clamp_value(value);

    if ((gripper_id == GRIPPER_MIN_ID) &&
        !board_pwm_set_pulse_width_us(
            value_to_pulse_width_us(clamped_value)))
    {
        statistics.rejected_command_count++;
        return GRIPPER_COMMAND_REJECTED;
    }

    gripper_state_t *const state =
        &states[gripper_id - GRIPPER_MIN_ID];
    const bool value_changed =
        state->valid && (state->value != clamped_value);

    state->timestamp_ms = timestamp_ms;
    state->command_id = command_id;
    state->value = clamped_value;
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
