#ifndef BOARD_PWM_H
#define BOARD_PWM_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t compare_ticks;
    uint32_t period_ticks;
    uint32_t pulse_width_us;
    uint32_t timer_tick_hz;
    bool running;
} board_pwm_state_t;

bool board_pwm_init(void);
bool board_pwm_set_pulse_width_us(uint16_t pulse_width_us);
bool board_pwm_get_state(board_pwm_state_t *out_state);

#endif /* BOARD_PWM_H */
