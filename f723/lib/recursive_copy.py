import glob
import os
import shutil

Src_dest = './src'
Inc_dest = './inc'
filenameH = '*.h*'
filenameC = '*.c*'

def dos2uinx_r(path):
    os.system("find "+path+" -type f -print0 | xargs -0 dos2unix > /dev/null 2>&1")

# dos2uinx_r(".")

if os.path.exists(Src_dest):
    print("exists")
    shutil.rmtree(Src_dest)
    os.makedirs(Src_dest, exist_ok=True)
else:
    os.makedirs(Src_dest, exist_ok=True)
    print("not exists")

if os.path.exists(Inc_dest):
    print("exists")
    shutil.rmtree(Inc_dest)
    os.makedirs(Inc_dest, exist_ok =True)
else:
    os.makedirs(Inc_dest, exist_ok =True)
    print("not exists")



src = '.'
for file_path in glob.glob(os.path.join(src, '**', filenameH), recursive=True):
    if(file_path.find('@') >= 0):
        pass
    else:
        new_path = os.path.join(Inc_dest, os.path.basename(file_path))
        shutil.copy(file_path, new_path)
        print(file_path)


for file_path in glob.glob(os.path.join(src, '**', filenameC), recursive=True):
    if(file_path.find('@') >= 0):
        pass
    else:
        new_path = os.path.join(Src_dest, os.path.basename(file_path))
        shutil.copy(file_path, new_path)
        print(file_path)

