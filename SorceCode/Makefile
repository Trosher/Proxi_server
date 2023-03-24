ALL_SERV_FILE=Server/Server.cc NetProcessing/NetProcessing.cc CreationLog/CreationLog.cc
CFLAGS=-lstdc++ -std=c++17 -lm -lpthread -Wall -Werror -Wextra 
CC=gcc

PROXI_PORT=85
DB_PORT=3306
DB_IP=127.0.0.1

.PHONY: clean Executable_server start

all: clean Executable_server start

Executable_server: clean
	$(CC) $(ALL_SERV_FILE) $(CFLAGS) -o ProxyServer

start:
	./ProxyServer $(PROXI_PORT) $(DB_PORT) $(DB_IP)

clean: 
	rm -rf ProxyServer