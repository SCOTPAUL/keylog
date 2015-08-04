#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "networking.h"

#define PORT "3491"
#define BACKLOG 15
#define BUFFER_SIZE 1000

int loop = 1;

void sigint_handler(int sig){
    loop = 0;
}

void sigchld_handler(int sig){
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void print_usage_and_quit(char *application_name);

int main(int argc, char *argv[]){
    int sockfd, new_fd;
    struct sigaction sa;
    struct sockaddr_storage their_addr; // Address information of client
    socklen_t sin_size;
    char s[INET_ADDRSTRLEN];
    char buffer[BUFFER_SIZE];
    ssize_t bytes_recieved;
    int i;
    FILE *fp;

    int file = 0, option = 0;
    while((option = getopt(argc, argv,"sf:")) != -1){
        switch(option){
            case 's':
                freopen("/dev/null", "w", stdout);
                freopen("/dev/null", "w", stderr);
                break;
            case 'f':
                file = 1;
                if((fp = fopen(optarg, "a")) == NULL){
                    perror("opening");
                }
                break;
            default: print_usage_and_quit(argv[0]);
        }
    }

    sockfd = get_listener_socket_file_descriptor(PORT);

    if(listen(sockfd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // Reap dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1){
        perror("sigaction");
        exit(1);
    }

    printf("%s\n", "server: waiting for connections");

    while(loop){
        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if(new_fd == -1){
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, &(((struct sockaddr_in *)&their_addr)->sin_addr), s, sizeof(s));
        printf("server: got connection from %s\n", s);

        if(!fork()){ // We are the child process
            close(sockfd);

            bytes_recieved = recv(new_fd, buffer, sizeof(buffer), 0);

            if(bytes_recieved < 0){
                perror("recv");
                return 1;
            }

            while(bytes_recieved > 0){
                for(i = 0; i < bytes_recieved; ++i){
                    if(file) fprintf(fp, "%c", buffer[i]);
                    else printf("%c", buffer[i]);
                }
                bytes_recieved = recv(new_fd, buffer, sizeof(buffer), 0);
            }
            if(file) fflush(fp);

            close(new_fd);
            exit(0);
        }
    }

    close(new_fd);

    if(file) fclose(fp);
    return 0;

}

void print_usage_and_quit(char *application_name){
    printf("Usage: %s [-s] filename\n", application_name);
    exit(1);
}
