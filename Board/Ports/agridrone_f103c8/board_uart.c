#include "board_config.h"

#if defined(BOARD_AGRIDRONE_F103C8)

#include "board_uart.h"

#include <stddef.h>

#include "main.h"

#define BOARD_UART_TX_TIMEOUT_MS 100U

extern UART_HandleTypeDef huart2;

static uint32_t error_count;

bool board_uart_write(const uint8_t *data, size_t length)
{
    if ((data == NULL) || (length == 0U) || (length > UINT16_MAX))
    {
        return false;
    }

    if (HAL_UART_Transmit(&huart2,
                          data,
                          (uint16_t) length,
                          BOARD_UART_TX_TIMEOUT_MS) != HAL_OK)
    {
        error_count++;
        return false;
    }

    return true;
}

uint32_t board_uart_get_error_count(void)
{
    return error_count;
}

#endif /* BOARD_AGRIDRONE_F103C8 */
