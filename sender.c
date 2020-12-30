#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

#define SIZE 1024
#define BITS 256
#define ID "127.0.0.1"
#define PORT 8080

void send_file(FILE *file_pointer, int sockfd){
    int n;
    char data[SIZE] = {0};
    while (fgets(data, SIZE, file_pointer) != NULL){
        if (send(sockfd, data, sizeof(data), 0) == -1){
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }

}

// main function
int main() {

    // ip address and port
    char *ip = ID;
    int port = PORT;
    int connection, mysocket;

    // server adder
    struct sockaddr_in server_addr;

    // file pointer and file name
    FILE *file_pointer;
    char *file_name = "1mb.txt";
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    // getting error
    if(mysocket < 0) {
        perror("socket error");
        exit(1);
    }

    // create socket
    printf("socket created\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // trying to connect to server - get -1 if can't
    connection = connect(mysocket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // getting error
    if(connection == -1) {
        perror("socket error");
        exit(1);
    }

    // connect to server
    printf("server conected\n");

    // open file
    file_pointer = fopen(file_name, "r");
    if(file_pointer == NULL){
        perror("file ead error");
        exit(1);
    }
    
    socklen_t len;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("socket");
        return -1;
    }

    // getting current socket
    char buf[BITS];
    len = sizeof(buf);
    if(getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) {
        perror("getsockopt");
        return -1;
    }
    printf("current: %s\n", buf);

    // seding file
    send_file(file_pointer, mysocket);
    printf("data sent\n");

    // close socket
    close(mysocket);
    printf("close connection.\n");

    // change buf to "reno" and reset len
    strcpy(buf, "reno");
    len = strlen(buf);

    if(setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
        perror("setsockopt");
        return -1;
    }

    len = sizeof(buf);

    // getting socket
    if(getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) {
        perror("getsockopt");
        return -1;
    }

    printf("new: %s\n", buf);

    // sending the file
    send_file(file_pointer, sock);
    printf("data sent\n");

    // close socket
    close(sock);
    printf("close connection\n");

    return 0;
}