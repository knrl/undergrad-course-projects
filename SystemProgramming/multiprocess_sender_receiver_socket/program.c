/**
 * @file      multithread_socket_program.c
 * @author    Mehmet Kaan Erol (kaanerol@marun.edu.tr)
 * @brief     System Programming
 * @version   0.1
 * @date      2022-05-24
 * 
 * @copyright Copyright (c) 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define STDIN    0
#define STDOUT   1

struct addrinfo hints;
struct addrinfo *result;
struct sockaddr_in * saddr;
char * hostname;
char in_buf[BUF_SIZE];
char out_buf[BUF_SIZE];
int port;
int sock;
int i, s, r;	

pthread_t send_thread, receive_thread;

void cleanup() {	// Handles CTRL-C
	close(sock);
	exit(0);
}

void * sender() {
	// TODO: While there is data that can be read from stdin, read
	//   it and send it to the socket
	//   Use 's' to count bytes to send
	// Exit if there is a write error

	while ((s = read(STDIN,in_buf,BUF_SIZE)) > 0)
	{
		if (write(sock,in_buf,s) < 0)
		{
			perror("soket yazma hatasi");
			exit(1);
		}
	}
}

void * receiver() {
	// TODO: While there is data that can be read from the socket, read
	//   it and send it to stdout
	//   Use 'r' to count bytes read
	// Exit if there is a write error

	while((r = read(sock,out_buf,BUF_SIZE)) > 0)
	{
		if (write(STDOUT,out_buf,r) < 0)
		{
			perror("stdout yazma hatasi");
			exit(1);
		}
	}
}

int main(int argc, char * argv[]){

	signal(SIGINT, cleanup);

	if( argc < 3 ){
		fprintf(stderr, "ERROR: argüman gerekli\n");
		exit(1);
	}

	if(sscanf(argv[2], "%d", &port) < 1){
		fprintf(stderr, "ERROR: hatalı port\n");
		exit(1);
	}

	hostname = argv[1];
	port 	 = atoi(argv[2]);

	// TODO: Set up address resolution hints
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;

	// TODO: Use getaddrinfo() to resolve hostname.
	if ((s = getaddrinfo(hostname,NULL,&hints,&result)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(s));
		exit(1);
	}

	// TODO: Properly set IP address and port in saddr
	saddr = (struct sockaddr_in *) result->ai_addr;
	saddr->sin_port = htons(port);
  
 	// TODO: Open the socket (sock)
	if ((sock = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("soket acilamadi");
		exit(1);
	}

	// TODO: Connect the socket
	if ((connect(sock,(struct sockaddr *) saddr, sizeof(*saddr))) < 0)
	{
		perror("baglant kurulamadi");
		exit(1);
	}

	// TODO: Create the sender and receiver threads
	pthread_t sender_th,receiver_th;
	if (pthread_create(&sender_th,NULL,sender,NULL) < 0)
	{
		perror("sender thread olusturulamadi");
		exit(1);
	}

	if (pthread_create(&receiver_th,NULL,receiver,NULL) < 0)
	{
		perror("receiver thread olusturulamadi");
		exit(1);
	}

	// TODO: Have the main thread wait on the sender and receiver threads
	pthread_join(sender_th,NULL);
	pthread_join(receiver_th,NULL);

	exit(0);
}