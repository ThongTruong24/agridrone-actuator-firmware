#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include <stdint.h>

void debug_console_init(void);
void debug_console_process(uint32_t now_ms);

#endif /* DEBUG_CONSOLE_H */
