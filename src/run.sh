#!/bin/bash

rm -rf bin/*
mkdir -p bin

JDK_PATH="/usr/lib/jvm/default"
JAVA_INC="$JDK_PATH/include"
JAVA_INC_LINUX="$JDK_PATH/include/linux"

echo "1/3: Compiling Java..."
javac -d ./bin ./src/Mathx.java

echo "2/3: Compiling C++ library..."
g++ -O3 -march=native -fopenmp-simd -shared -fPIC -I"$JAVA_INC" -I"$JAVA_INC_LINUX" ./src/Mathx.cpp -o ./bin/libmathx.so

if [ $? -eq 0 ]; then
    echo "3/3: Running program..."
    echo "--------------------------------"
    java -cp bin -Djava.library.path=bin Test
else
    echo "Error: An issue occurred during C++ compilation!"
fi