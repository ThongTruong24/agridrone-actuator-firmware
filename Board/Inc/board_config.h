#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

/*
 * The active board must be selected by the build configuration. Never define
 * a board target in source code or infer it from an STM32 device macro.
 */
#if defined(BOARD_AGRIDRONE_F103C8) && defined(BOARD_AGRIDRONE_F446)

#error "Multiple board targets selected by the build configuration"

#elif defined(BOARD_AGRIDRONE_F103C8)

#define BOARD_NAME "Agridrone Actuator F103C8"

#elif defined(BOARD_AGRIDRONE_F446)

#define BOARD_NAME "Agridrone Actuator F446"

#else

#error "No supported board selected by the build configuration"

#endif

#endif /* BOARD_CONFIG_H */
