#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "asip.h"
#include "asip_log.h"

/* A simple example reading pin 10: when it changes, PIN 13 is
 * turned on/off.
*/
int main (int argc, char **argv) {

	// Hard coded, change as appropriate
	char *serialPort = "/dev/ttyACM0";

	asip_open(serialPort);

	short unsigned int ledPin = 13;
	short unsigned int inputPin = 10;

	asip_set_pin_mode(ledPin, OUTPUT_MODE);
	asip_set_pin_mode(inputPin, INPUT_PULLUP_MODE);

	// We need a variable to store the value of the input pin
	short unsigned int oldvalue = 0;


	// Inifinite loop...

	while (1) {

		short unsigned int curValue = asip_digital_read(inputPin);
		if (  curValue != oldvalue ) {
			asip_digital_write(ledPin, curValue);
		}
		oldvalue = curValue;
		usleep(5000); // Sleep a bit, we don't want 100% CPU
	}

	// We'll never get here...
	asip_close();
	return 0;
}

