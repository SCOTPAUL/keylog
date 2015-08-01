#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#define NUM_EVENTS 128

/**
 * Captures keystrokes by reading from the keyboard resource and writing to
 * the writeout file. Continues reading until SIGINT is recieved. A newline is
 * appended to the end of the file.
 *
 * \param keyboard The file descriptor for the keyboard input file
 * \param writeout The file descriptor to write keystrokes out to
 */
void keylogger(int keyboard, int writeout);

#endif
