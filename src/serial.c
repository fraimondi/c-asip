#include <pthread.h>
#include "serial.h"
#include "asip.h"
#include "asip_log.h"

static int serialFd = -1; /* file descriptor for the serial port */

static pthread_t tid;
static int read_thread_state = 0; // 0: normal, -1: stop thread: close_port sets to -1;

void *readFromSerial(void *someArgs) {
	asip_log(DEBUG,"readFromSerial: starting reading loop \n");


    unsigned char d;
    ssize_t s;


    /* We need to read a character at a time
     * We accumulate it in an array called buffer, when there is a new line
     * we dispatch to the asip parser.
     * We also check to see if we are required to shut down at every loop.
     */
	char buffer[1024]; // We store here the string being built
	while(1) {
		int build=1;
		buffer[0]='\0';
		while (build) {
			if((s=read(serialFd, &d,1)) != -1)
			{
				if ( d == '\n' ) {
					// If we have a new line, we call the parser
					asip_parse_incoming_message(buffer);
					build=0;

				} else {
					// Otherwise, concatenate
					unsigned int pos = strlen(buffer);
					buffer[pos] = d;
					buffer[pos + 1] = '\0';
				}
				if ( read_thread_state == -1 ) {
					asip_log(DEBUG,"readFromSerial: received request to shut down \n");
					pthread_exit(0); // we exit if someone changed the flag
				}
			}
		}
	}
}

/* FIXME: maybe move some of the constants/flags outside this */
int open_port(char *serialPort) {
	struct termios options;

	asip_log(DEBUG,"open_port: trying to open %s \n",serialPort);
	//serialFd = open(serialPort, O_RDWR | O_NONBLOCK);
	serialFd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY);
	if (serialFd == -1)
	{
		asip_log(ERROR,"open_port: unable to open %s \n",serialPort);
		exit(1);
	}
	else {
		fcntl(serialFd, F_SETFL, 0);

	    if (tcgetattr(serialFd, &options) < 0) {
	    	asip_log(ERROR,"open_port: unable to get options of %s \n",serialPort);
	        return -1;
	    }

		if ( cfsetispeed(&options,B57600)<0 || cfsetospeed(&options,B57600)<0 ) {
                  asip_log(ERROR,"open_port: unable to set BAUD RATE of %s \n",serialPort);
                  exit(1);
		}

        options.c_cflag |= (CLOCAL | CREAD);
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_cflag &= ~CRTSCTS;
        options.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
        options.c_iflag &= ~(IXON | IXOFF | IXANY);
        options.c_iflag |= (INPCK | ISTRIP);
        options.c_oflag &= ~OPOST;

        tcflush(serialFd, TCIOFLUSH);
        options.c_cc[VMIN] = 1;
        options.c_cc[VTIME] = 0;

        tcsetattr(serialFd, TCSADRAIN, &options);

	    tcsetattr(serialFd, TCSANOW, &options);

	    asip_log(DEBUG,"open_port: port %s open and all flags set \n",serialPort);
	}

	/* Starting the thread to read from serial */
	asip_log(DEBUG,"open_port: creating read thread \n");
	pthread_create(&tid, NULL, readFromSerial, NULL);


	return (serialFd);
}

void close_port() {
	asip_log(DEBUG,"close_port: closing port \n");
	read_thread_state = -1; // Set flag to stop reading thread...
	asip_log(DEBUG,"close_port: waiting for read thread to terminate \n");
	pthread_join(tid, NULL); // and wait for it to stop.
	close(serialFd);
	asip_log(DEBUG,"close_port: serial port closed \n");
}



void writeToSerial(char *message) {
	write(serialFd, message, strlen(message));
        usleep(10000);
}
