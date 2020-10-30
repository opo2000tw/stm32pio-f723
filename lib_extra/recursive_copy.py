import glob
import os
import shutil

dest = './test/src'
filenameH = '*.h'
filenameC = '*.c'

src = './Middlewares'
for file_path in glob.glob(os.path.join(src, '**', filenameH), recursive=True):
    new_path = os.path.join(dest, os.path.basename(file_path))
    shutil.copy(file_path, new_path)
    print(file_path)

for file_path in glob.glob(os.path.join(src, '**', filenameC), recursive=True):
    new_path = os.path.join(dest, os.path.basename(file_path))
    shutil.copy(file_path, new_path)
    print(file_path)

src = './CMSIS'
for file_path in glob.glob(os.path.join(src, '**', filenameH), recursive=True):
    new_path = os.path.join(dest, os.path.basename(file_path))
    shutil.copy(file_path, new_path)
    print(file_path)

for file_path in glob.glob(os.path.join(src, '**', filenameC), recursive=True):
    new_path = os.path.join(dest, os.path.basename(file_path))
    shutil.copy(file_path, new_path)
    print(file_path)