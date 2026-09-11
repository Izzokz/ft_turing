#!/bin/bash

# script to launch cJSON build
# cd cJSON-master
# mkdir build
# cd build
# cmake .. -DENABLE_CJSON_UTILS=On -DENABLE_CJSON_TEST=Off -DCMAKE_INSTALL_PREFIX=/home/huvillat/Documents/ft_turing
# make
# make DESTDIR=$pkgdir install

cc -Wall -Wextra -Werror -g t_cjson.c ../../lib/libcjson.a ../../lib/libcjson_utils.a -I../../include/cjson
./a.out
# rm ./a.out