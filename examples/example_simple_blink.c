#include "asip.h"
#include "asip_log.h"

/* A very minimal example: open a serial connection, set pin 13 to
 * output mode, digital write 0 or 1 for 10 times.
 */
int main (int argc, char **argv) {

	// Hard coded, change as appropriate
	char *serialPort = "/dev/ttyACM0";

	asip_open(serialPort);

	asip_set_pin_mode(13, OUTPUT_MODE);

	// Loop, 10 times
	for (int i=0; i<10; i++) {
		asip_digital_write(13, 1);
		usleep(1000000);
		asip_digital_write(13, 0);
		usleep(1000000);
	}

	asip_close();

	return 0;
}

