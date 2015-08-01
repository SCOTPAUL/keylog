#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int get_socket_file_descriptor(char *hostname, char *port){
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        freeaddrinfo(servinfo);
        exit(1);
    }

    // Connect to the first possible result
    for(p = servinfo; p != NULL; p = p->ai_next){
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("socket");
            continue;
        }

        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(sockfd);
            perror("connection");
            continue;
        }

        break;
    }

    if(p == NULL){
        fprintf(stderr, "failed to connect\n");
        freeaddrinfo(servinfo);
        exit(1);
    }

    inet_ntop(p->ai_family, p->ai_addr, s, sizeof(s));
    printf("connecting to %s\n", s);

    freeaddrinfo(servinfo);

    return sockfd;
}
