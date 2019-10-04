#!/bin/bash
rm -rf CMakeFiles
rm cmake_install.cmake
rm CMakeCache.txt
rm Makefile
cmake -DCMAKE_BUILD_TYPE=Release -DGUI=ON -DVERBOSE=ON -DENERGY=ON -DOMP=ON  ..
make -j 4
