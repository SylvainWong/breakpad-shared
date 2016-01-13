# breakpad-shared
Breakpad is a set of client and server components which implement a crash-reporting system. It builds out a static library by default, I build it as a shared library to decrease the size of application.

## My Build Environment
Ubuntu 14.04 64-bit  
Android NDK r10e  

## Build
1) set NDK to PATH  
2) cd ./jni  
3) ndk-build clean  
4) ndk-build  
5) go check output files in ../libs/armeabi-v7a/  
6) go check intermediate files in ../obj/ if you need  
