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

/* Toggle PC13 when the selected gripper value actually changes. */
#ifndef APP_TEST_LED_GRIPPER
#define APP_TEST_LED_GRIPPER 1
#endif

/* Gripper actuator ID monitored by APP_TEST_LED_GRIPPER (1..4). */
#ifndef APP_TEST_LED_GRIPPER_ID
#define APP_TEST_LED_GRIPPER_ID 1U
#endif

#if ((APP_TEST_UART_ENABLE != 0) && (APP_TEST_UART_ENABLE != 1))
#error "APP_TEST_UART_ENABLE must be 0 or 1"
#endif

#if ((APP_TEST_CAN_ENABLE != 0) && (APP_TEST_CAN_ENABLE != 1))
#error "APP_TEST_CAN_ENABLE must be 0 or 1"
#endif

#if ((APP_TEST_LED_GRIPPER != 0) && (APP_TEST_LED_GRIPPER != 1))
#error "APP_TEST_LED_GRIPPER must be 0 or 1"
#endif

#if ((APP_TEST_LED_GRIPPER_ID < 1U) || (APP_TEST_LED_GRIPPER_ID > 4U))
#error "APP_TEST_LED_GRIPPER_ID must be in the range 1..4"
#endif

#endif /* APP_CONFIG_H */
