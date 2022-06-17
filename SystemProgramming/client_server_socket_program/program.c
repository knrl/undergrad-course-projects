/**
 * @file program.c
 * @author Mehmet Kaan Erol (kaanerol@marun.edu.tr)
 * @brief Multi process server client program example
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE   2048
#define STDOUT_FILENO 1
#define STDIN_FILENO  0

struct sockaddr_in saddr_in;  
int saddr_len;
char buffer[BUFFER_SIZE];

/**
 * @brief Client function that called by child process  
 * 
 */
int client()
{
    const char hostname[] = "127.0.0.1";
    const char request[]  = "request message to server";
    char response[BUFFER_SIZE];
    const short port = 1845;
    int client_socket;

    // set the address
    saddr_in.sin_family = AF_INET;  // IPv4
    inet_aton(hostname, &saddr_in.sin_addr);
    saddr_in.sin_port = htons(port);      // change byte order by htons()
    saddr_len = sizeof(struct sockaddr_in);

    // open a socket
    if ((client_socket = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("CLIENT: Cilent socket cannot open.");
        return EXIT_FAILURE;
    }

    // connect to the (address:port) server
    if (connect(client_socket, (struct sockaddr *) &saddr_in, sizeof(saddr_in)) < 0)
    {
        perror("CLIENT: Connection error");
        return EXIT_FAILURE;
    }

    // send the request
    if (write(client_socket, request, sizeof(request)) < 0)
    {
        perror("CLIENT: Error raised while sending to request message.");
        return EXIT_FAILURE;
    }
    printf("CLIENT: Write: %s\n", request);

    int num_byte = 0;
    while ((num_byte = read(client_socket, response, BUFFER_SIZE)) > 0)
    {
        printf("CLIENT: Read: %s\n", response);
    }

    if (num_byte < 0)
    {
        perror("Error raised while reading from client socket.");
        return EXIT_FAILURE;
    }

    close(client_socket);
    return EXIT_SUCCESS;
}

/**
 * @brief Server function that called by parent process  
 * 
 */
int server()
{
    const char hostname[] = "127.0.0.1";
    char response[BUFFER_SIZE] = "Server respond message";
    const short port    = 1845;
    const short backlog = 5;
    int server_socket, peer_socket;

    struct sockaddr_in client_saddr_in;

    // set the address
    saddr_in.sin_family = AF_INET;  // IPv4
    inet_aton(hostname, &saddr_in.sin_addr);
    saddr_in.sin_port = htons(port);      // change byte order by htons()
    saddr_len = sizeof(struct sockaddr_in);

    // open a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("SERVER: Socket cannot open.");
        return EXIT_FAILURE;
    }

    // bind to the (address:port)
    if (bind(server_socket, (struct sockaddr *) &saddr_in, sizeof(saddr_in)) < 0)
    {
        perror("SERVER: Connection error");
        return EXIT_FAILURE;
    }

    // listen
    if (listen(server_socket, backlog) < 0)
    {
        printf("SERVER: Error occured during listening on %s:%d",
            inet_ntoa(saddr_in.sin_addr), ntohs(saddr_in.sin_port));
        return EXIT_FAILURE;
    }

    // accept
    if ((peer_socket = accept(server_socket, (struct sockaddr*) &client_saddr_in, &saddr_len)) < 0)
    {
        perror("SERVER: Error occured during accept().");
        return EXIT_FAILURE;
    }

    printf("SERVER: Connection from: %s:%d (%d)\n", 
        inet_ntoa(client_saddr_in.sin_addr),
        ntohs(client_saddr_in.sin_port),
        peer_socket);

    printf("SERVER: Connection to: %s:%d\n", 
        inet_ntoa(client_saddr_in.sin_addr),
        ntohs(client_saddr_in.sin_port));

    // read data from client
    int num_byte = read(peer_socket, response, BUFFER_SIZE);
    response[num_byte] = '\0';

    printf("SERVER: Read: %s\n", response);
    snprintf(response, BUFFER_SIZE, "Second response message from server");

    if (write(peer_socket, response, strlen(response)))
    {
        return EXIT_FAILURE;
    }
    printf("SERVER: Write: %s\n", response);

    close(server_socket);
    close(peer_socket);
    return EXIT_SUCCESS;
}

int main()
{
    pid_t cpid;
    int status = EXIT_SUCCESS;

    // create process
    cpid = fork();
    if (cpid == 0) // child process: Client
    {
        status = client();
    }
    else           // parent process: Server
    {
        status = server();
        if (status == EXIT_SUCCESS)
            wait(NULL);
    }

    exit(status);
}
