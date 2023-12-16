#!/bin/bash

set -e

cd ..

# 如果没有build目录，创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/lzj_muduo  so库拷贝到 /usr/lib    PATH
if [ ! -d /usr/include/lzj_muduo ]; then 
    mkdir /usr/include/lzj_muduo
fi

cd `pwd`/include

# 拷贝hpp文件
for header in `ls *.h`
do
    cp $header /usr/include/lzj_muduo
done

cd ..
cp `pwd`/lib/liblzj_muduo.so /usr/lib

ldconfig