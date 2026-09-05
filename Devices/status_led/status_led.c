#include "status_led.h"

#include "board_gpio.h"

void status_led_toggle(void)
{
    board_status_led_toggle();
}
