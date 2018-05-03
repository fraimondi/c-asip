#ifndef ASIP_LOG_H_
#define ASIP_LOG_H_

#include <stdarg.h>

typedef enum {
	OFF,
	ERROR,
	WARN,
	INFO,
	DEBUG,
	TRACE
} asip_log_level;

void asip_log(asip_log_level level, char *fmt, ...);
void asip_set_log_level(asip_log_level level);
asip_log_level asip_get_log_level();
void asip_set_output_stream(FILE* s);
#endif
