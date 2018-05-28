#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include <string.h>  /* String function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

int open_port(char *serialPort);
void close_port();
// void readFromSerial();
void writeToSerial(char *message);

#endif
