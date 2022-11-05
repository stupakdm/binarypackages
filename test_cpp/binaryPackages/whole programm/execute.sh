#!/bin/bash
if [ $# = 2 ]
then
sudo apt update
sudo apt install curl
g++ -o main main.cpp
./main $1 $2
else
echo "Нужно задать 2 параметра"
fi
