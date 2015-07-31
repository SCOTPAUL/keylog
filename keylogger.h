#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#define NUM_EVENTS 128

/**
 * Captures keystrokes by reading from the keyboard resource and writing to
 * the writeout file. Continues reading until SIGINT is recieved, when it will
 * close the passed resources.
 *
 * \param keyboard The file descriptor for the keyboard input file
 * \param writeout A pointer to the file to write keystrokes out to.
 */
void keylogger(int keyboard, FILE *writeout);

#endif
