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

print(
    "=========================================================[pre]=========================================================")

print()