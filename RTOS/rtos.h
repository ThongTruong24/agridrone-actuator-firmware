#ifndef RTOS_H
#define RTOS_H

#include "FreeRTOS.h"

/*
 * The opaque arguments preserve the existing CubeMX-owned main.c hook while
 * preventing STM32 HAL types from leaking into the custom public API.
 */
BaseType_t rtos_init(const void *legacy_can_handle,
                     const void *legacy_console_uart);

#endif /* RTOS_H */
