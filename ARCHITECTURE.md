# Firmware architecture

The repository separates common product code, public Board APIs, board-specific
implementations, and complete CubeMX/CubeIDE target projects.

```text
COMMON CODE
===========
Algorithms  Application  Config  Devices  Middleware
RTOS        Services     ThirdParty       Utils
                         |
                         v
                  Board public API
                    (Board/Inc)
                         |
                         v
              Board/Targets/<target>
                         |
                         v
       Targets/<target>/Core + Drivers + startup
                         |
                         v
                        MCU
```

## Physical layout

```text
Board/
|-- Inc/                         public, HAL-free APIs
`-- Targets/
    `-- agridrone_f103c8/        F103 implementation of those APIs

Targets/
`-- agridrone_f103c8/            complete F103 CubeMX/CubeIDE project
    |-- Core/
    |-- Drivers/
    |-- .settings/
    |-- .project
    |-- .cproject
    |-- .mxproject
    |-- drone_actuator.ioc
    `-- STM32F103C8TX_FLASH.ld
```

`Targets/<target>/` owns the target's `.ioc`, generated `Core`, generated
`Drivers`, startup, linker script, CubeIDE project metadata, and settings. It
does not own a copy of product code. Future targets such as
`Targets/agridrone_f446/` follow the same structure and are generated for their
actual MCU independently.

## CubeIDE integration

Open or import `Targets/agridrone_f103c8/` as the `drone_actuator` CubeIDE
project. Its `.project` exposes root common directories with Eclipse linked
folders using `PARENT-2-PROJECT_LOC`; no path contains a workstation username.
Its Debug and Release configurations compile these source roots:

```text
Application
Board/Targets/agridrone_f103c8
Devices
Middleware
RTOS
Services
ThirdParty
Core
Drivers
```

`Algorithms`, `Config`, and `Utils` remain visible linked directories but do not
contain C translation units at present. Header include paths reference the
linked common folders. `Core` and `Drivers` are target-local physical folders.
The F103 build defines `BOARD_AGRIDRONE_F103C8` and `STM32F103xB`, uses the
target-local `STM32F103C8TX_FLASH.ld`, and compiles only
`Core/Startup/startup_stm32f103c8tx.s`.

CubeIDE managed build is the source of truth. `Debug/` and `Release/` are
derived outputs and are ignored both at repository root and under targets.

## Layer ownership

| Path | Responsibility |
| --- | --- |
| `Application/` | Top-level orchestration and user-facing snapshots/debug output |
| `Services/` | Product rules coordinating protocols and devices |
| `Devices/` | Device state and device-specific behavior |
| `Middleware/` | DroneCAN transport, libcanard integration, and DSDL decoding |
| `RTOS/` | Task creation and scheduling glue |
| `Config/` | Fixed common compile-time configuration |
| `Board/Inc/` | MCU-independent hardware interfaces |
| `Board/Targets/<target>/` | The selected target's HAL-facing implementation |
| `Targets/<target>/` | CubeMX-generated platform project and CubeIDE metadata |
| `ThirdParty/` | External FreeRTOS and libcanard sources |

Common layers must not include STM32 HAL/LL headers, generated peripheral
headers, or expose `HandleTypeDef`. HAL-facing custom code belongs in the
selected `Board/Targets/<target>/` implementation.

## Current runtime ownership

- `Board/Targets/agridrone_f103c8/board_can.c` owns CAN1 HAL access, filters,
  FIFO extraction, and ISR-to-task delivery.
- The other F103 Board files map the console to USART2, status LED to PC13, and
  time to the HAL tick.
- `Middleware/dronecan/` owns the FreeRTOS RX queue, libcanard, transfer
  acceptance, DSDL decoding, and protocol diagnostics.
- `Services/command/` maps actuator commands into gripper state;
  `Services/health/` stores the latest NodeStatus snapshot.
- `Application/debug_console/` owns optional UART and semantic CAN test output.
- `RTOS/tasks/can_task.*` runs the combined `test_console` task in task context.

The filesystem migration does not change clocks, CAN timing/filter/pins, UART,
GPIO, interrupt priority, FreeRTOS configuration, DroneCAN IDs/DSDL, gripper
behavior, or refresh timing.

## Board selection

Exactly one board symbol is selected by each target's CubeIDE build
configuration. The current target uses `BOARD_AGRIDRONE_F103C8`. Public
`Board/Inc/board_config.h` rejects no selection and multiple selections. The
future symbol `BOARD_AGRIDRONE_F446` is reserved there, but no F446 target,
generated code, HAL copy, startup, or board implementation exists yet.

To add F4 later, generate `Targets/agridrone_f446/`, implement
`Board/Targets/agridrone_f446/`, select `BOARD_AGRIDRONE_F446` in that project's
Debug and Release configurations, and ensure its Board source entry excludes
the F103 implementation.

## CubeMX regeneration

Open `Targets/agridrone_f103c8/drone_actuator.ioc`, generate code in place,
refresh the CubeIDE project, then Clean and Build. Keep product logic outside
generated files wherever possible. After regeneration, confirm the linked
resources, common include paths, `BOARD_AGRIDRONE_F103C8`, and the Board-specific
source entry are still present.

Two existing generated integration points remain intentional technical debt:

- `Core/Src/main.c` calls `rtos_init(&hcan, &huart2)` before starting the
  scheduler; the compatibility API does not expose those handles to common code.
- `Core/Src/stm32f1xx_it.c` forwards SysTick to FreeRTOS and contains the current
  SVC/PendSV ownership integration.

They are now located under `Targets/agridrone_f103c8/Core/` and were moved
without functional edits by this migration.
