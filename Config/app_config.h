#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* Send "hello" to the console once per second from the test task. */
#ifndef APP_TEST_UART_ENABLE
#define APP_TEST_UART_ENABLE 0
#endif

/* Receive CAN frames and print them to the console from the same test task. */
#ifndef APP_TEST_CAN_ENABLE
#define APP_TEST_CAN_ENABLE 1
#endif

#if ((APP_TEST_UART_ENABLE != 0) && (APP_TEST_UART_ENABLE != 1))
#error "APP_TEST_UART_ENABLE must be 0 or 1"
#endif

#if ((APP_TEST_CAN_ENABLE != 0) && (APP_TEST_CAN_ENABLE != 1))
#error "APP_TEST_CAN_ENABLE must be 0 or 1"
#endif

#endif /* APP_CONFIG_H */
