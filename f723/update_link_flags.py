# Custom settings, as referred to as "extra_script" in platformio.ini
#
# See http://docs.platformio.org/en/latest/projectconf.html#extra-script
# See https://docs.platformio.org/en/latest/envvars.html

from SCons.Script import DefaultEnvironment
import time
import os
# from console.utils import wait_key

env = DefaultEnvironment()

os.system("python ./pre_build.py")

env.Append(
    LINKFLAGS=[
        "-mthumb",
        "-mfloat-abi=hard",
        "-mfpu=fpv5-sp-d16",
        "-mcpu=cortex-m7",
        "-mfloat-abi=hard",
        # "-specs=nano.specs"
    ],
    CFLAGS=[
    ]
)
