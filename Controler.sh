#!/bin/bash

if [ "$#" -ne 1 ];
then
    echo "ERROR: The number of arguments should be 1 "
    exit 1
fi

comand=$1

if [[ $comand == start ]];
then
    cd ./DataBase/ && bash ControlBd.sh start
    cd ../SorceCode/ && make
elif [[ $comand == down ]];
then
    cd ./DataBase/ && bash ControlBd.sh down
fi