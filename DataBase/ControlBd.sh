#!/bin/bash
#psql -U postgres (Use in the conteiner)

if [ "$#" -ne 1 ];
then
    echo "ERROR: The number of arguments should be 1 "
    exit 1
fi

comand=$1

if [[ $comand == start ]];
then
    docker-compose -f ./DockerCompose.yaml up -d
elif [[ $comand == down ]];
then
    docker-compose -f ./DockerCompose.yaml down
elif [[ $comand == re ]];
then
    docker-compose -f ./DockerCompose.yaml up --build
elif [[ $comand == stop ]];
then
    docker-compose -f ./DockerCompose.yaml stop
else
    echo "ERROR: The command passed by the argument was not recognized "
fi