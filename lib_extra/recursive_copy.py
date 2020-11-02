import glob
import os
import shutil

dest = './test/src'
filenameH = '*.h'
filenameC = '*.c'


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


def dos2uinx_r(path):
    os.system("find "+path+" -type f -print0 | xargs -0 dos2unix")


rm_r("./test")
if not os.path.isdir(dest):
    os.system("mkdir "+"./test")
    os.system("mkdir "+dest)
else:
    pass

# src = './Middlewares'
# for file_path in glob.glob(os.path.join(src, '**', filenameH), recursive=True):
#     new_path = os.path.join(dest, os.path.basename(file_path))
#     shutil.copy(file_path, new_path)
#     print(file_path)

# for file_path in glob.glob(os.path.join(src, '**', filenameC), recursive=True):
#     new_path = os.path.join(dest, os.path.basename(file_path))
#     shutil.copy(file_path, new_path)
#     print(file_path)

# src = './CMSIS'
# for file_path in glob.glob(os.path.join(src, '**', filenameH), recursive=True):
#     new_path = os.path.join(dest, os.path.basename(file_path))
#     shutil.copy(file_path, new_path)
#     print(file_path)

# for file_path in glob.glob(os.path.join(src, '**', filenameC), recursive=True):
#     new_path = os.path.join(dest, os.path.basename(file_path))
#     shutil.copy(file_path, new_path)
#     print(file_path)

src = '.'
for file_path in glob.glob(os.path.join(src, '**', filenameH), recursive=True):
    new_path = os.path.join(dest, os.path.basename(file_path))
    shutil.copy(file_path, new_path)
    print(file_path)

for file_path in glob.glob(os.path.join(src, '**', filenameC), recursive=True):
    new_path = os.path.join(dest, os.path.basename(file_path))
    shutil.copy(file_path, new_path)
    print(file_path)

dos2uinx_r(".")
