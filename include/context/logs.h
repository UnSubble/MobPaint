#ifndef LOGS_H
#define LOGS_H

#include <stdbool.h>

bool init_logs(void);
void close_logs(void);

void log_error(const char *format, ...);
void log_info(const char *format, ...);

#endif // LOGS_H
