#include "asip.h"
#include "serial.h"
#include "asip_log.h"

// two arrays to hold data received
static short analog_io_pins[MAX_NUM_ANALOG_PINS]; // we only need values 0-1023, let's use a short
static char digital_io_pins[MAX_NUM_DIGITAL_PINS]; // these can only be 0 or 1, let's use a char

void asip_open(char *serialPort) {
	open_port(serialPort); // call to serial port to open
}

void asip_close() {
	close_port(); // call to serial port to close
}

void asip_enable_ir(int interval) {
}

void asip_enable_bumpers(int interval) {
}

int asip_analog_read(int pin) {
	if ((pin >= 0) || (pin<MAX_NUM_ANALOG_PINS)) {
		asip_log(TRACE,"asip_analog_read: returning value %d for pin %d\n",analog_io_pins[pin],pin);
		return analog_io_pins[pin];
	} else {
		asip_log(ERROR,"asip_analog_read: pin value %d not valid\n",pin);
	}
	return -1;
}

void asip_set_motor(int motorId, int speed) {
}

int asip_get_bumper(int position) {
  return 0;
}

int asip_get_ir(int irID) {
  return 0;
}


/** A function to parse incoming messages containing information about
 * analog pins. The message is of the form: @I,a,6,{7:1,8:116,9:133,11:288,12:197,13:211}
 * We extract the data between { and }, and then populate analog_io_pins accordingly
 * (in the case of the example: pin 7 is 1, pin 8 is 116, etc)
 */
void asip_process_analog_values(char *message) {
	char *target = NULL;
	char *start, *end;

	asip_log(DEBUG,"asip_process_analog_values: entering\n");
	start = strstr( message, "{" );
	if ( start )	{
		start += 1; // Ignore the "{" character
		end = strstr( start, "}" );
		if ( end ) {
			target = ( char * )malloc( end - start + 1 );
			if ( target == 0 ) {
				asip_log(ERROR,"asip_process_analog_values: OUT OF MEMORY\n");
			} else {
				memcpy( target, start, end - start );
				target[end - start] = '\0';
				asip_log(TRACE,"asip_process_analog_values: the substring is %s\n",target);
			}
		} else {
			asip_log(WARN,"asip_process_analog_values: I couldn't find a closing }.\n");
		}
	} else {
		asip_log(WARN,"asip_process_analog_values: I couldn't find an opening {.\n");
	}

	// target contains the substring "7:1,8:116...": let's split it and populate the array
	char *pairs = strtok (target, ",");
	while (pairs != NULL) {
		char *p;
		// FIXME: ADD A FEW MEMORY CHECKS!
		int position; // the position of ":";
		char *key;
		p = strchr(pairs, ':');

		position = strlen(pairs)-strlen(p);
		key = (char *)malloc(position+1);
		memcpy(key,pairs,position);
		key[position] = '\0';
		asip_log(TRACE,"asip_process_analog_values: setting pin %d to %d.\n",atoi(key),atoi(p+1));
		analog_io_pins[atoi(key)] = atoi(p+1);
		pairs = strtok (NULL, ",");
		free(key);
	}

	free(target);
	asip_log(DEBUG,"asip_process_analog_values: exiting\n");


}

/** A function to parse incoming event messages (those starting with @).
 * We need to look at the first character after @ to find out the appropriate
 * service and dispatch the message to the handler for that service.
 * If you want to extend with additional services, add your new tags here
 */
void asip_handle_input_event(char *message) {
	asip_log(DEBUG,"asip_handle_input_event: entering\n");
	if (strlen(message)>3) {
		switch(message[1]) {

			case IO_SERVICE:
				// it's an IO service, let's call the appropriate handler
				switch (message[3]) {
					case ANALOG_VALUE:
						asip_process_analog_values(message);
						break;

					default:
						asip_log(DEBUG,"asip_handle_input_event: I don't know tag %c, I'm ignoring it.\n",message[3]);
				}
				break;
				// end of IO_SERVICE switch;

			default:
				asip_log(DEBUG,"asip_handle_input_event: I don't know tag %c, I'm ignoring it.\n",message[1]);

		}
	} else {
		asip_log(WARN,"asip_handle_input_event: I've received an incoming message of length < 2\n");
	}
	asip_log(DEBUG,"asip_handle_input_event: exiting\n");
}

/** Entry point for asip messages received.
 * The messages are then passed to other functions to be managed:
 * they could be events, errors, debug.
 */
void asip_parse_incoming_message(char * message) {
	if (strlen(message) > 0) { // Just in case...
		asip_log(DEBUG,"asip_parse_incoming_message: received message of length %ld\n", strlen(message));
		switch(message[0]) {
			case EVENT_HANDLER:
				asip_log(TRACE,"asip_parse_incoming_message for event: %s\n",message);
				asip_handle_input_event(message);
				break;
			case ERROR_MESSAGE_HEADER:
				asip_log(WARN,"asip_parse_incoming_message error message: %s\n",message);
				break;
			case DEBUG_MESSAGE_HEADER:
				asip_log(DEBUG,"Received debug message %s\n", message);
				break;
			default:
				asip_log(WARN,"I've received something strange of length %ld: %s\n",strlen(message),message);
		}
	} else {
		asip_log(WARN,"I've received an incoming message of length 0\n");
	}
}

/* We just need to send the message L,C, but we use
 * the built-in constants and malloc*/
void asip_clear_lcd_screen() {
	char *toSend;
	toSend = (char *)malloc(sizeof(LCD_SERVICE)+1+sizeof(LCD_CLEAR)+1);
	if ( toSend == 0 ) {
		asip_log(ERROR,"asip_clear_lcd_screen: OUT OF MEMORY\n");
	} else {
		sprintf(toSend,"%c,%c\n",LCD_SERVICE,LCD_CLEAR);
		asip_log(TRACE,"asip_clear_lcd_screen: sending %s\n",toSend);
		writeToSerial(toSend);
		free(toSend);
	}
}

/* We get a string message and a line and we print it */
void asip_write_lcd_line(char *msg, unsigned short int line_number) {
	char *toSend;
	if ( (line_number < 5) && (line_number>=0)) {
		toSend = (char *)malloc(sizeof(LCD_SERVICE)+1+sizeof(LCD_WRITE)+1+1+1+strlen(msg)+10);
		if ( toSend == 0 ) {
			asip_log(ERROR,"asip_write_lcd_line: OUT OF MEMORY\n");
		} else {
			sprintf(toSend,"%c,%c,%d,%s\n",LCD_SERVICE,LCD_WRITE,line_number,msg);
			writeToSerial(toSend);
			asip_log(TRACE,"asip_write_lcd_line: sending %s\n",toSend);
			free(toSend);
		}
	} else {
		asip_log(ERROR,"asip_write_lcd_line: invalid line number %d\n",line_number);
	}

}


