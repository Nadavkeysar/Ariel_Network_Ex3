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
#define ID "127.0.0.1"
#define PORT 8080
#define TRUE 1
#define FALSE 0

// write file
void write_file(int sockfd) {

    int n;
    FILE *fp;
    char *filename = "recv.txt", buffer[SIZE];

    // open file
    fp = fopen(filename, "w");

    while(TRUE) {
        n = recv(sockfd, buffer, SIZE, 0);
        if(0 >= n) {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}

int main() {

    // variables
    int n, port = PORT, connection, sockfd, new_sock;
    char *ip = ID;

    // server adder
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // getting socket error
    if(sockfd < 0) {
        perror("socket error");
        exit(1);
    }

    // create socket
    printf("server socket created\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connection = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // error connecting
    if(connection < 0) {
        perror("bind error");
        exit(1);
    }

    // connected
    printf("binding successfull\n");
    if(listen(sockfd, 10) == 0) {
        printf("listening....\n");

        for(int i = 0; i < 5; i++) {

            // getting the cpu time
            clock_t start, end;
            float cpu_time;

            start = clock();
            double time = 0;

            // adder size
            addr_size = sizeof(new_addr);
            new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

            // write file
            write_file(new_sock);
            printf("data written in file\n");
            end = clock();
            cpu_time = ((float)(end - start))/CLOCKS_PER_SEC;
            printf("time it take is: %f \n", cpu_time);
        }
    }

    // getting error
    else {
        perror("listening error");
        exit(1);
    }
    return 0;
}