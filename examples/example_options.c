#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "asip.h"
#include "asip_log.h"


/* This example shows how to read the serial port from the command line.
 * It also shows how to enable logging, writing to file.
 */

int main (int argc, char **argv) {

	/* Here we parse the command line */
	char *serialPort = NULL;
	int index;
	int c;
	opterr = 0;
	int serialFlag = -1;

	while ((c = getopt (argc, argv, "p:")) != -1) {
		switch (c) {
		case 'p':
			serialPort = optarg;
			serialFlag = 0;
			break;
		case '?':
			if (optopt == 'p')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr,
						"Unknown option character `\\x%x'.\n",
						optopt);
			return 1;
		default:
			abort ();
		}
	}

	if (serialFlag < 0) {
		fprintf(stderr,"You need to specify a port with -p\n");
		exit(1);
	}

	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);

	/* end of command line parsing */

	// You can set the log level with the following function:
	// Levels are: OFF, ERROR, WARN, INFO, DEBUG, TRACE.
	// The default level is ERROR.
	// CAREFUL: TRACE is going to output a *lot* of information
	asip_set_log_level(TRACE);


	/* If you want to forward to a file, do the following
	 * (don't forget to close, see below!)
	 */
	FILE *fptr;
	fptr = fopen("asip.log","w"); // declare and open the file
	asip_set_output_stream(fptr); // and set output stream for log

	// Let's open the serial port:
	asip_open(serialPort);
	for (int i=0; i<10; i++) {
	// And now a loop to print on screen the value of pin 0 for 10 times
		printf("Analog pin 0 is: %d\n",asip_analog_read(0));
		usleep(1000000);
	}

	// Be gentle and close the serial port
	asip_close();

	/* Don't forget to close the file, if you opened it! */
	fclose(fptr);

	return 0;
}

