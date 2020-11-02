#!/usr/bin/env python
import os
import subprocess
import shutil
from pathlib import Path

# Write-Overwrites
path = "./Core"
w_path = "myfile.txt"
r_path = w_path
oldsize = 1


def write(path, str):
    file1 = open(w_path, "w")  # write mode
    file1.write("Tomorrow \n")
    file1.close()


def rm_r(path='.'):
    size = folder_size(path)
    # print(path+","+str(size))
    if size == 1 or size == 0:
        print("not exsist "+path)
    else:
        print("remove "+path)
        if os.path.isdir(path) and not os.path.islink(path):
            shutil.rmtree(path)
        elif os.path.exists(path):
            os.remove(path)


def cp_r(path_src, path_dest):
    print(str(folder_size(path_src))+"," + str(folder_size(path_dest)))
    if folder_size(path_src) == 1:
        pass
    elif folder_size(path_src) == folder_size(path_dest):
        pass
    else:
        os.system("cp -rf"+" "+path_src+" "+path_dest)
        print("cp -rf"+" "+path_src+" "+path_dest)
        pass


def dos2uinx_r(path):
    os.system("find "+path+" -type f -print0 | xargs -0 dos2unix")


def folder_size(path='.'):
    total = 0
    if os.path.isfile(path) or os.path.isdir(path) or os.path.islink(path):
        for entry in os.scandir(path):
            if entry.is_file():
                total += entry.stat().st_size
            elif entry.is_dir():
                total += folder_size(entry.path)
    else:
        total = 1
    return total


def main():
    # pass
    # rm_r()
    cp_r("Middlewares", "./pio_core/packages/framework-stm32cube/f7")
    cp_r("Middlewares/ST/STM32_USB_Host_Library", "../lib_extra/")
    cp_r("Middlewares/Third_Party/FreeRTOS", "../lib_extra/")
    rm_r("Middlewares")
    cp_r("USB_HOST", "../lib_extra/")
    rm_r("USB_HOST")
    cp_r("Drivers", "./pio_core/packages/framework-stm32cube/f7")
    rm_r("Drivers")
    dos2uinx_r("Core")
    os.chdir("../lib_extra/")
    os.system("python ./recursive_copy.py")


if __name__ == '__main__':
    main()
