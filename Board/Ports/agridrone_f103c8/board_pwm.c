#include "board_config.h"

#if defined(BOARD_AGRIDRONE_F103C8)

#include "board_pwm.h"

#include <stddef.h>

#include "main.h"

/* THACO actuator ID 1 is wired to TIM2_CH1 on PA0. */
#define BOARD_PWM_ACTUATOR_CHANNEL TIM_CHANNEL_1
#define BOARD_PWM_MICROSECONDS_PER_SECOND 1000000UL

extern TIM_HandleTypeDef htim2;

static bool initialized;

static uint32_t timer_tick_hz(void)
{
    uint32_t timer_clock_hz = HAL_RCC_GetPCLK1Freq();

    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1)
    {
        timer_clock_hz *= 2U;
    }

    return timer_clock_hz / (htim2.Instance->PSC + 1U);
}

bool board_pwm_init(void)
{
    if (initialized)
    {
        return true;
    }

    __HAL_TIM_SET_COMPARE(&htim2, BOARD_PWM_ACTUATOR_CHANNEL, 0U);

    if (HAL_TIM_PWM_Start(&htim2, BOARD_PWM_ACTUATOR_CHANNEL) != HAL_OK)
    {
        return false;
    }

    initialized = true;
    return true;
}

bool board_pwm_set_pulse_width_us(uint16_t pulse_width_us)
{
    if (!initialized && !board_pwm_init())
    {
        return false;
    }

    const uint32_t tick_hz = timer_tick_hz();
    if (tick_hz == 0U)
    {
        return false;
    }

    const uint32_t period_counts =
        __HAL_TIM_GET_AUTORELOAD(&htim2) + 1U;
    uint32_t compare = (uint32_t)
        (((uint64_t) pulse_width_us * tick_hz) /
         BOARD_PWM_MICROSECONDS_PER_SECOND);

    if (compare > period_counts)
    {
        compare = period_counts;
    }

    __HAL_TIM_SET_COMPARE(&htim2, BOARD_PWM_ACTUATOR_CHANNEL, compare);
    return true;
}

bool board_pwm_get_state(board_pwm_state_t *out_state)
{
    if (out_state == NULL)
    {
        return false;
    }

    out_state->compare_ticks =
        __HAL_TIM_GET_COMPARE(&htim2, BOARD_PWM_ACTUATOR_CHANNEL);
    out_state->period_ticks =
        __HAL_TIM_GET_AUTORELOAD(&htim2) + 1U;
    out_state->timer_tick_hz = timer_tick_hz();
    out_state->pulse_width_us =
        (out_state->timer_tick_hz == 0U)
            ? 0U
            : (uint32_t)
                (((uint64_t) out_state->compare_ticks *
                  BOARD_PWM_MICROSECONDS_PER_SECOND) /
                 out_state->timer_tick_hz);
    out_state->running = initialized;
    return true;
}

#endif /* BOARD_AGRIDRONE_F103C8 */
