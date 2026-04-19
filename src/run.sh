#!/bin/bash

JDK_PATH="/usr/lib/jvm/default"

JAVA_INC="$JDK_PATH/include"
JAVA_INC_LINUX="$JDK_PATH/include/linux"

echo "1/3: Compiling Java..."
javac Mathx.java

echo "2/3: Compiling C++ library..."
g++ -shared -fPIC -I"$JAVA_INC" -I"$JAVA_INC_LINUX" Mathx.cpp -o libmathx.so

if [ $? -eq 0 ]; then
    echo "3/3: Running program..."
    echo "--------------------------------"
    java -Djava.library.path=. Test
else
    echo "Error: An issue occurred during C++ compilation!"
fi