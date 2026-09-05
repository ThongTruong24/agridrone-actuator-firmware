#include "board_config.h"

#if defined(BOARD_AGRIDRONE_F103C8)

#include "board_gpio.h"

#include "main.h"

void board_status_led_toggle(void)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

#endif /* BOARD_AGRIDRONE_F103C8 */
