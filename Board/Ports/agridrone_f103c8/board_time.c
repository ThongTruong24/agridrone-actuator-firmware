#include "board_config.h"

#if defined(BOARD_AGRIDRONE_F103C8)

#include "board_time.h"

#include "main.h"

uint32_t board_time_ms(void)
{
    return HAL_GetTick();
}

uint64_t board_time_us(void)
{
    static uint32_t previous_tick;
    static uint64_t accumulated_ms;
    const uint32_t current_tick = HAL_GetTick();

    accumulated_ms += (uint32_t) (current_tick - previous_tick);
    previous_tick = current_tick;

    return accumulated_ms * 1000ULL;
}

#endif /* BOARD_AGRIDRONE_F103C8 */
