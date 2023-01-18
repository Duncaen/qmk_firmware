## chip/board settings
# the next two should match the directories in
#  <chibios>/os/hal/ports/$(MCU_FAMILY)/$(MCU_SERIES)
MCU_FAMILY = STM32
MCU_SERIES = STM32L0xx

# linker script to use
MCU_LDSCRIPT = STM32L072xZ

# startup code to use
# is should exist in <chibios>/os/common/ports/ARMCMx/compilers/GCC/mk/
MCU_STARTUP = stm32l0xx

# it should exist either in <chibios>/os/hal/boards/
#  or <this_dir>/boards
BOARD = ST_NUCLEO64_L073RZ

# Cortex version
MCU  = cortex-m0plus

# ARM version, CORTEX-M0/M1 are 6, CORTEX-M3/M4/M7 are 7
ARMV = 6

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no        # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = yes        # Commands for debug and configuration
NKRO_ENABLE = no            # USB Nkey Rollover
DIP_SWITCH_ENABLE = yes     # Enable DIP switch support

DFU_ARGS = -d 0483:df11 -a 0 -s 0x08000000:leave

LAYOUTS = 60_hhkb

BOOTLOADER = stm32-dfu
STM32_BOOTLOADER_ADDRESS = 0x1FF00000

OPT_DEFS += -g -fno-lto

# Custom matrix file for the HHKB
CUSTOM_MATRIX = lite
SRC += matrix.c
#SRC += analog.c
