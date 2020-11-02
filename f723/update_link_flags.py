# Custom settings, as referred to as "extra_script" in platformio.ini
#
# See http://docs.platformio.org/en/latest/projectconf.html#extra-script

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

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