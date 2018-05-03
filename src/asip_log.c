#include <stdio.h>
#include "asip_log.h"

static asip_log_level LEVEL = ERROR;
static FILE* asip_output_file = NULL;

void asip_log(asip_log_level l, char *fmt, ...) {

	if (asip_output_file == NULL) {
		asip_output_file = stderr;
	}
	va_list argp;
	va_start(argp, fmt);

	if (l<=LEVEL) {
		vfprintf(asip_output_file,fmt,argp);
	}
}

void asip_set_log_level(asip_log_level l) {
	LEVEL=l;
}

asip_log_level asip_get_log_level() {
	return LEVEL;
}

void asip_set_output_stream(FILE* s) {
	asip_output_file = s;
}
