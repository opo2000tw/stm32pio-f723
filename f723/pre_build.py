#!/usr/bin/env python
import os
import subprocess
import shutil
from pathlib import Path
import glob

lib_path = Path("lib")
inc_path = Path("inc")
src_path = Path("src")
py_path = Path("library.json")


def rm_r(path='.'):
    size = folder_size(path)
    if size == 1 or size == 0:
        print("not exsist "+str(path))
    else:
        print("remove "+str(path))
        if os.path.isdir(path) and not os.path.islink(path):
            shutil.rmtree(path)
        elif os.path.exists(path):
            os.remove(path)


def cp_r(path_src, path_dest):
    new_path = Path.joinpath(lib_path, path_src)
    if folder_size(path_src) == 1:
        pass
        print(str("file not exsist"))
        return new_path, 1
    elif folder_size(path_src) == folder_size(path_dest):
        pass
        print(str("file not changed"))
        return new_path, 1
    else:
        # rm_r(new_path)
        src = str(path_src)
        dst = Path.joinpath(lib_path, path_src)
        dst.mkdir(exist_ok=True, parents=True)
        shutil.copytree(src, dst, dirs_exist_ok=True)
        return new_path, 0


def dos2uinx_r(path):
    os.system("find "+path+" -type f -print0 | xargs -0 dos2unix > /dev/null 2>&1")


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


paths = [Path("Middlewares"), Path("USB_HOST"), Path("Drivers")]


def main():
    pass
    for path in paths:
        new_root_path, break_flag = cp_r(path, lib_path)
        if break_flag == 1:
            break
        new_inc_path = Path.joinpath(new_root_path, inc_path)
        for file in glob.glob(os.path.join(new_root_path, '**', '*.h*'), recursive=True):
            file_path = Path(file)
            new_file_path = Path.joinpath(new_inc_path, file_path.name)
            if(file.find('@') >= 0):
                pass
            elif(file_path == new_file_path):
                pass
            else:
                Path(new_inc_path).mkdir(parents=True, exist_ok=True)
                shutil.copy2(str(file_path), str(new_file_path))
        new_src_path = Path.joinpath(new_root_path, src_path)
        for file in glob.glob(os.path.join(new_root_path, '**', '*.c*'), recursive=True):
            file_path = Path(file)
            new_file_path = Path.joinpath(new_src_path, file_path.name)
            if(file.find('@') >= 0):
                pass
            elif(file_path == new_file_path):
                pass
            else:
                Path(new_src_path).mkdir(parents=True, exist_ok=True)
                shutil.copy2(str(file_path), str(new_file_path))

        # os.system("python"+lib_path+i+"recursive.py")
        # rm_r(i)


if __name__ == '__main__':
    main()
