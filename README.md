# breakpad-shared
Breakpad is a set of client and server components which implement a crash-reporting system. It builds out a static library by default, I build it as a shared library to decrease the size of application.

## My Build Environment
Ubuntu 14.04 64-bit  
Android NDK r10e  
gcc 4.8.4  

## Build libbreakpad.so
1) set NDK to PATH  
2) cd ./jni  
3) ndk-build clean  
4) ndk-build  
5) go check output files in ../libs/armeabi-v7a/  
6) go check intermediate files in ../obj/ if you need  

## Build dump_syms and minidump_stackwalk
1) cd ./jni/googlebreakpad/src  
2) ./configure  
3) make  
4) go check output files at ./processor/minidump_stackwalk and ./tools/linux/dump_syms/dump_syms  
