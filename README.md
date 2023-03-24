# Proxi_server

project build rules:

* The c++ 17 standard must be supported
* The gcc compiler is used, it should be available in the system
* Need support for the version of docker compose 3.9
* Everything should be run from superuser rights to avoid problems with creating sockets
* The system must have the mariadb:10.5 image or there must be an internet connection. (It will pull itself up)
* The program was written for Ubuntu server 20.04, it also works on wsl ubuntu is not inferior to the version. On other systems, the operability was not checked
* The launch is made from the bash script Controler.sh with the start parameter
