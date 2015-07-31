CC=gcc
CFLAGS= -c -Wall

all: keylog

keylog: main.o keylogger.o
	$(CC) main.o keylogger.o -o keylog

main.o: main.c
	$(CC) $(CFLAGS) main.c

keylogger.o: keylogger.c
	$(CC) $(CFLAGS) keylogger.c

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f keylog *.o
	@echo "Finished cleaning."
