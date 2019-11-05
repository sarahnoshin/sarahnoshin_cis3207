all: server client

server: tcpechoserver_threaded.c
	gcc tcpechoserver_threaded.c -o server -lpthread

client: tcpechoclient.c
	gcc tcpechoclient.c -o client
