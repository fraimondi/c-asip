#ifndef ASIP_H_
#define ASIP_H_

/**
 * The overall idea is the following: this thing is attached to a serial port.
 * Writing is easy, we simply convert something like asip_set_motor(0,75) to
 * the appropriate ASIP message (in this case: M,m,0,75).
 * Reading is more complicated: there is a thread listening to incoming messages
 * (see serial.c). This thread invokes asip_parse_incoming_message (see below).
 * asip_parse_incoming_message dispatches the message to the appropriate functions
 * and these functions populate static arrays like analog_io_pins etc.
 * When a user requests values, for instance asip_analog_read(5), we simply
 * return the value in the array.
 */

#include "serial.h"

#define MAX_NUM_DIGITAL_PINS  72 // Max number of digital pins
#define MAX_NUM_ANALOG_PINS   16 // Max number of analog pins

// A lot of ASIP constants
#define EVENT_HANDLER '@'
#define ERROR_MESSAGE_HEADER  '~'
#define DEBUG_MESSAGE_HEADER  '!'
#define AUTOEVENT_MESSAGE  'A'
#define ASIP_EVENT  'e'
#define IO_SERVICE  'I'
#define PORT_MAPPING 'M'
#define ANALOG_VALUE  'a'

#define MOTOR_SERVICE  'M'
#define SET_MOTOR_SPEED  'm'

#define IR_SERVICE   'R'

#define BUMPER_SERVICE  'B'

#define LCD_SERVICE  'L'
#define LCD_WRITE  'W'
#define LCD_CLEAR  'C'

void asip_open(char *serialPort);
void asip_close();
void asip_request_port_mapping();
void asip_enable_ir(int interval);
void asip_enable_bumpers(int interval);
int asip_analog_read(int pin);
void asip_set_motor(int motorId, int speed);
int asip_get_bumper(int position);
int asip_get_ir(int irID);
void asip_clear_lcd_screen();
void asip_write_lcd_line(char *msg, unsigned short int line_number);
void asip_parse_incoming_message(char * message);

#endif
