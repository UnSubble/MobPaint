#include "logs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

#ifndef LOG_DIR
#define LOG_DIR "logs/"
#endif // LOG_DIR

#ifndef ERROR_LOG_PATH
#define ERROR_LOG_PATH "logs/errors.log"
#endif // ERROR_LOG_PATH

static FILE *error_log = NULL;
static FILE *history_log = NULL;

static void get_date_string(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d", t);
}

bool init_logs(void) {
    struct stat st = {0};
    if (stat(LOG_DIR, &st) == -1) {
        if (mkdir(LOG_DIR, 0755) == -1) {
            fprintf(stderr, "Failed to create log directory: %s\n", strerror(errno));
            return false;
        }
    }

    error_log = fopen(ERROR_LOG_PATH, "a");
    if (!error_log) {
        fprintf(stderr, "Failed to open errors.log\n");
        return false;
    }

    char date[32];
    get_date_string(date, sizeof(date));
    char history_path[128];
    snprintf(history_path, sizeof(history_path), "%s%s-history.log", LOG_DIR, date);

    history_log = fopen(history_path, "a");
    if (!history_log) {
        fprintf(stderr, "Failed to open history log file\n");
        fclose(error_log);
        return false;
    }

    fprintf(history_log, "[START] MobPaint session started.\n");
    fflush(history_log);

    return true;
}

void close_logs(void) {
    if (history_log) {
        fprintf(history_log, "[END] MobPaint session ended.\n");
        fclose(history_log);
        history_log = NULL;
    }
    if (error_log) {
        fclose(error_log);
        error_log = NULL;
    }
}

static void vlog(FILE *log_file, const char *prefix, const char *format, va_list args) {
    if (!log_file) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char timebuf[20];
    strftime(timebuf, sizeof(timebuf), "%H:%M:%S", t);

    fprintf(log_file, "[%s] %s: ", timebuf, prefix);
    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");
    fflush(log_file);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vlog(error_log, "ERROR", format, args);
    va_end(args);
}

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vlog(history_log, "INFO", format, args);
    va_end(args);
}
