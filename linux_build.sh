#!/bin/sh
rm -rf cmakebuild
CMaker project:MaratonFramework compiler:clang++ head:*.h src:*.cpp,*.c lib:/usr/lib/libuv.a flag:-std=c++11-pthread out:lib 
cd cmakebuild
cmake ./
make -j 4
cd cmakebuild
