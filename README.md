# c-asip

This is the plain C client for the asip protocol. See [this paper](https://www.sciencedirect.com/science/article/pii/S0140366416300743) for the theoretical background.

**IMPORTANT**: only Linux and Mac clients are supported (currently tested on Linux x64, Mac 10.13, Raspberry Pi with standard Raspbian). Tested on Arduino Uno and Teensy 3.2

## Installation and usage

* First of all, install the ASIP firmware on the microcontroller, as explained here: https://github.com/mdxmase/asip
* Clone this repository and simply type `make` at the command line
* If the compilation goes according to plans, you will find a (static) library in `lib/`
* Check the directory `examples/` for simple examples. After `make`, you will find executable files in `/bin/`. Make sure you check the source of the examples before trying to run them.
* This is a simple blink example:
    
    #include "asip.h"
    #include "asip_log.h"
    /* A very minimal example: open a serial connection, set pin 13 to
       output mode, digital write 0 or 1 for 10 times.
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
 * If you are in the root of c-asip and you have your source file in `/PATH/TO/FILE.C`, compile with: `gcc -I includes/ /PATH/TO/FILE.c -o YOUR-EXECUTABLE-NAME lib/libasip.a -lpthread`
