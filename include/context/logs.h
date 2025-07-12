#ifndef LOGS_H
#define LOGS_H

#include <stdbool.h>

/**
 * Initializes the logging system.
 * Creates or opens necessary log files.
 *
 * @return true if initialization is successful, false otherwise.
 */
bool init_logs(void);

/**
 * Closes and flushes all open log files.
 * Should be called before program termination.
 */
void close_logs(void);

/**
 * Logs an error message to the error log file.
 * Accepts printf-style formatting.
 *
 * @param format Format string.
 * @param ...    Format arguments.
 */
void log_error(const char *format, ...);

/**
 * Logs an informational message to the history log file.
 * Accepts printf-style formatting.
 *
 * @param format Format string.
 * @param ...    Format arguments.
 */
void log_info(const char *format, ...);

#endif // LOGS_H
