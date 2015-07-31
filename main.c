#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
    FILE *readin;
    FILE *writeout;

    if(argc != 3){
        printf("Usage: %s [KEYBOARD-FILE] [WRITEOUT-FILE]", argv[0]);
        return 1;
    }

    if((readin = fopen(argv[1], "r")) == NULL){
        printf("Error opening file %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    if((writeout = fopen(argv[2], "a")) == NULL){
        printf("Error opening file %s: %s\n", argv[2], strerror(errno));
        return 1;
    }
    

}
