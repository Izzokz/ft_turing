#!/bin/bash

cd cJSON-master
mkdir build
cd build
cmake .. -DENABLE_CJSON_UTILS=On -DENABLE_CJSON_TEST=Off -DCMAKE_INSTALL_PREFIX=/home/huvillat/Documents/ft_turing
make
make DESTDIR=$pkgdir install