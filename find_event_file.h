#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/**
 * Tries to find the filepath of a connected keyboard
 * \return malloc allocated filepath of keyboard, or NULL if none could be 
 *         detected
 */
char *get_keyboard_event_file(void);

