# MCU targets

Each directory under `Targets/` is a complete, independently importable
STM32CubeMX/STM32CubeIDE project. Product code remains shared at the repository
root and is exposed to each CubeIDE project through portable linked resources.

To add a new board:

1. Create `Targets/<new_board>/`.
2. Create the CubeMX project inside that directory.
3. Configure its MCU, clocks, pins, and peripherals for the real hardware.
4. Generate `Core/`, `Drivers/`, startup, linker, and CubeIDE metadata there.
5. Add linked resources/source locations for the common root directories.
6. Define exactly one `BOARD_<TARGET>` symbol in every build configuration.
7. Compile only `Board/Targets/<new_board>/` as the Board implementation.
8. Implement the public APIs from `Board/Inc/` for the new hardware.
9. Build, flash, and test the target on its board.

Do not copy common product directories into a target. A future F446 target must
be generated from its own CubeMX project; do not reuse F103 HAL, startup, or
generated code.
