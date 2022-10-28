#!/usr/bin/bash

echo "building chomik for windows!!!"

echo "only src and src2 should be built"

mingw32-configure --disable-chomik-servers --enable-windows-build
make clean
make
