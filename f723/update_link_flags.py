# Custom settings, as referred to as "extra_script" in platformio.ini
#
# See http://docs.platformio.org/en/latest/projectconf.html#extra-script
# See https://docs.platformio.org/en/latest/envvars.html

import subprocess
import shutil
from pathlib import Path
import glob
import os
from glob import glob
from os.path import basename, isdir, isfile, join
from shutil import copy
from string import Template
import sys

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()
platform = env.PioPlatform()

FRAMEWORK_DIR = platform.get_package_dir("framework-stm32cube")
FRAMEWORK_CORE = env.BoardConfig().get("build.mcu")[5:7].lower()
MCU_FAMILY = env.BoardConfig().get("build.mcu")[0:7]

print("=========================================================[post]=========================================================")

t_path = "./pio_core/packages/framework-stm32cubef7/Drivers/STM32F7xx_HAL_Driver/Inc/stm32f7xx_hal_conf.h"
xx_hal_conf = Path(t_path)
print("[-]"+xx_hal_conf.name)
xx_hal_conf.unlink()

env.Append(
    LINKFLAGS=[
        "-mfloat-abi=hard",
        "-mfpu=fpv5-sp-d16",
        "-mcpu=cortex-m7",
        "-mfloat-abi=hard",
    ],
    CFLAGS=[
    ],
    CPPPATH=[
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "Drivers", "CMSIS", "Include"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "Drivers", "CMSIS", "Device",
        #      "ST", MCU_FAMILY.upper() + "xx", "Include"),

        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "Drivers",
        #      MCU_FAMILY.upper() + "xx_HAL_Driver", "Inc"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "Drivers",
        #      "BSP", "Components", "Common"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE,
        #      "FreeRTOS",  "Source", "CMSIS_RTOS_V2"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE,
        #      "FreeRTOS",  "Source", "include"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "USB_HOST",
        #      "App"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "USB_HOST",
        #      "Target"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "Middlewares", "ST",
        #      "STM32_USB_Device_Library", "Core", "Inc"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "Middlewares", "ST",
        #      "STM32_USB_Device_Library", "Core", "Src"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "Middlewares", "ST",
        #      "STM32_USB_Device_Library", "Class", "CDC", "Inc"),
        # join(FRAMEWORK_DIR, FRAMEWORK_CORE, "Middlewares", "ST",
        #      "STM32_USB_Device_Library", "Class", "CDC", "Src"),
    ],
)
