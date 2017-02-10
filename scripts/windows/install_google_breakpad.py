#!/usr/bin/env python

import os
import argparse
import shutil
import string
import sys


# parse args
parser = argparse.ArgumentParser()
parser.add_argument('src', help='google_breakpad root directory where build was done')
parser.add_argument('dst', help='destination root directory where google_breakpad have to be installed')

args = parser.parse_args()


# test args
if not os.path.isdir(args.src):
    print(args.src + ' not such directory')
    sys.exit(1)



# customize args
google_breakpad_src_dir = os.path.join( args.src, 'src' )
dst_dir = os.path.join( args.dst )


# function to define src and dst in copy function
def convertSrcAndDst( srcFile, dstFile, subDir ):
    src = os.path.join(google_breakpad_src_dir, srcFile.replace('/', os.path.sep))
    dst = os.path.join(dst_dir, subDir, dstFile.replace('/', os.path.sep))
    return src, dst


def copyFile(src, dst):
    # print what we will doing
    print (src + ' ==>> ' + dst )

    # be sure dst directory exist or create it
    dst_dir = os.path.dirname(dst)
    if not os.path.exists(dst_dir):
        os.makedirs(dst_dir)

    # copy
    shutil.copy2(src, dst)




google_breakpad_header_files = [
    'client/windows/common/ipc_protocol.h',
    'client/windows/crash_generation/client_info.h',
    'client/windows/crash_generation/crash_generation_client.h',
    'client/windows/crash_generation/crash_generation_server.h',
    'client/windows/crash_generation/minidump_generator.h',
    'client/windows/handler/exception_handler.h',
    'common/scoped_ptr.h',
    'common/windows/string_utils-inl.h',
    'common/windows/http_upload.h',
    'google_breakpad/common/breakpad_types.h',
    'google_breakpad/common/minidump_format.h',
    'google_breakpad/common/minidump_cpu_amd64.h',
    'google_breakpad/common/minidump_cpu_arm.h',
    'google_breakpad/common/minidump_cpu_arm64.h',
    'google_breakpad/common/minidump_cpu_mips.h',
    'google_breakpad/common/minidump_cpu_ppc.h',
    'google_breakpad/common/minidump_cpu_ppc64.h',
    'google_breakpad/common/minidump_cpu_sparc.h',
    'google_breakpad/common/minidump_cpu_x86.h',
    'google_breakpad/common/minidump_exception_linux.h',
    'google_breakpad/common/minidump_exception_mac.h',
    'google_breakpad/common/minidump_exception_ps3.h',
    'google_breakpad/common/minidump_exception_solaris.h',
    'google_breakpad/common/minidump_exception_win32.h'
    ]

# copy headers
for f in google_breakpad_header_files:
    src, dst = convertSrcAndDst(f, f, os.path.join('include', 'breakpad'))
    copyFile(src, dst)






google_breakpad_library_dir_base = [
    'client/windows',
    'client/windows/crash_generation',
    'client/windows/handler'
    ]
google_breakpad_library_dir_end = 'lib'
google_breakpad_library_files = [
    'crash_generation_client.lib',
    'crash_generation_server.lib',
    'common.lib',
    'exception_handler.lib'
    ]

build_type = [
    'Debug',
    'Release'
    ]

# copy libraries
for dir in google_breakpad_library_dir_base:
    for buildType in build_type:
        for file in google_breakpad_library_files:
            path = os.path.join(dir, buildType, 'lib', file)
            src, dst = convertSrcAndDst(path, os.path.basename(path), os.path.join('lib', buildType))
            if os.path.exists(src):
                copyFile(src, dst)



google_breakpad_binary_dir_base = [
    'tools/windows/dump_syms/Release'
    ]
google_breakpad_binary_files = [
    'dump_syms.exe'
    ]

for dir in google_breakpad_binary_dir_base:
    for buildType in build_type:
        for file in google_breakpad_binary_files:
            path = os.path.join(dir, buildType, 'lib', file)
            src, dst = convertSrcAndDst(path, os.path.basename(path), 'bin')
            if os.path.exists(src):
                copyFile(src, dst)


