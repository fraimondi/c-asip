#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "asip.h"
#include "asip_log.h"


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
		fprintf(stderr,"You need to specify a port with -p");
		exit(1);
	}

	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);

	/* end of command line parsing */

	asip_set_log_level(TRACE); // Levels are: OFF, ERROR, WARN, INFO, DEBUG, TRACE

	/* if you want to forward to a file, do the following
	 * (don't forget to close, see below!)
	 */
	FILE *fptr;
	fptr = fopen("asip.log","w"); // declare and open the file
	//asip_set_output_stream(fptr); // and set output stream for log

	// Let's open the serial port:
	asip_open(serialPort);

	//asip_clear_lcd_screen();
	usleep(500000);
	// And now a loop to print on screen the value of the 7th analog pin
	for (int i=0; i<10; i++) {
		printf("Analog pin 0 is: %d\n",asip_analog_read(0));
		char lcd_message[50];
		sprintf(lcd_message,"Pin 0 = %d",asip_analog_read(0));
		//asip_write_lcd_line(lcd_message,0);
                //usleep(100000);
		//asip_write_lcd_line("Test line 1\n",1);
		usleep(1000000);
	}

	asip_close();

	/* Don't forget to close the file, if you opened it! */
	fclose(fptr);

	return 0;
}

