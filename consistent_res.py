# This python used to ensure resource consistent.
#
# desc:
# Compare files in source and destination directory, delete old file, update
# outdated file in destination directory and copy new file from source directory
# to destination
#
# input:
# 1. absolute path of source resources directory
# 2. absolute path of destination resources directory
import os
import sys
import fnmatch
import shutil
import time
from distutils import dir_util

# get arugments
command_path = sys.argv[0]
res_src_dir = sys.argv[1].replace("\\", "/")
res_dst_dir = sys.argv[2].replace("\\", "/")

# print information
print "## Call python to enure resource consistent."
print "## Sources resources directory:",res_src_dir
print "## Destination resources directory:",res_dst_dir

# verify source resources directory exists
if not os.path.exists(res_src_dir):
    print "## Source resource directory is not exists"
    if os.path.exists(res_dst_dir):
        print "## Remove destination directory",res_dst_dir
        shutil.rmtree(res_dst_dir)
    quit(0)

# ensure destination directory exists
if not os.path.exists(res_dst_dir):
    print "## copy all directory tree from " + res_src_dir + " to " + res_dst_dir
    dir_util.copy_tree(res_src_dir, res_dst_dir)
    quit(0)

# get src and dst files without root directory
print "## compare data between " + res_src_dir + " and " + res_dst_dir
src_files = []
dst_files = []
len_src_dir = len(res_src_dir)
len_dst_dir = len(res_dst_dir)
for root, dirnames, filenames in os.walk(res_src_dir):
    for filename in fnmatch.filter(filenames, '*'):
        f = root + "/" + filename
        f = f.replace("\\", "/")
        src_files.append(f[len_src_dir:])
for root, dirnames, filenames in os.walk(res_dst_dir):
    for filename in fnmatch.filter(filenames, '*'):
        f = root + "/" + filename
        f = f.replace("\\", "/")
        dst_files.append(f[len_dst_dir:])

# delete not exist files
for f in dst_files:
    if f not in src_files:
        rf = res_dst_dir + f
        print "remove", rf
        os.remove(rf)

# add new file and update outdated file
for f in src_files:
    src_f = res_src_dir + f
    dst_f = res_dst_dir + f
    if f not in dst_files:
        dst_dir = os.path.dirname(dst_f)
        if not os.path.exists(dst_dir):
            os.makedirs(dst_dir)
        print "copy " + src_f + " to " + dst_f
        shutil.copy2(src_f, dst_f)
    else:
        src_mtime = os.path.getmtime(src_f)
        dst_mtime = os.path.getmtime(dst_f)
        if abs(src_mtime - dst_mtime) > 1:
            print "update " + dst_f
            shutil.copy2(src_f, dst_f)
