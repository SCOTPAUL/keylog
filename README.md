#keylog

A simple keylogger for Linux written in C. Currently requires your keyboard input event file to be `/dev/input/by-path/platform-i8042-serio-0-event-kbd`.

## Usage

```bash
cd keylog/
sudo ./keylog "file-to-write-to.txt"
```

This will log all keystrokes to the specified file while the program is running.

## Building

```bash
cd keylog/
make
```
