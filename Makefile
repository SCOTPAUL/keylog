CC=gcc
CFLAGS= -c -Wall

all: keylog server

keylog: main.o keylogger.o networking.o
	$(CC) main.o keylogger.o networking.o -o keylog

server: server.o networking.o
	$(CC) server.o networking.o -o server

main.o: main.c
	$(CC) $(CFLAGS) main.c

keylogger.o: keylogger.c
	$(CC) $(CFLAGS) keylogger.c

networking.o: networking.c
	$(CC) $(CFLAGS) networking.c

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f keylog server *.o
	@echo "Finished cleaning."
