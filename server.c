#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include "networking.h"

#define PORT "3491"
#define BACKLOG 15

int loop = 1;

void sigint_handler(int sig){
    loop = 0;
}

void sigchld_handler(int sig){
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void print_usage_and_quit(char *application_name);

int main(int argc, char *argv[]){
    int sockfd;
    struct sigaction sa;

    if(argc != 2){
        print_usage_and_quit(argv[0]);
    }

    signal(SIGINT, sigint_handler);
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

    }

    close(sockfd);

    return 0;

}

void print_usage_and_quit(char *application_name){
    printf("Usage: %s filename\n", application_name);
    exit(1);
}
