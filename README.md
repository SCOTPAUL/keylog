#keylog

A simple keylogger for Linux written in C. Currently requires your keyboard device to be `/dev/input/event4`.

## Usage

```bash
cd keylog/
sudo ./keylog "file-to-write-to.txt"
```

## Building

```bash
cd keylog/
make
```
