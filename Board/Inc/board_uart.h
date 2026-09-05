#ifndef BOARD_UART_H
#define BOARD_UART_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool board_uart_write(const uint8_t *data, size_t length);
uint32_t board_uart_get_error_count(void);

#endif /* BOARD_UART_H */
