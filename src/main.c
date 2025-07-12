#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logs.h"

#ifndef TMP_DIR
#define TMP_DIR "/tmp/"
#endif

#define TEMP_FILENAME "mobpaint_temp.dat"
#define MAX_PATH_LEN 512

extern int run_app(const char *target_file_path);

int main(int argc, char *argv[]) {
    if (!init_logs()) {
        fprintf(stderr, "Failed to initialize logs.\n");
        return 1;
    }

    char target_file_path[MAX_PATH_LEN] = {0};

    if (argc > 1) {
        snprintf(target_file_path, MAX_PATH_LEN, "%s", argv[1]);
        log_info("User specified target file: %s", target_file_path);
    } else {
        const char *temp_dir = getenv("MOBPAINT_TMPDIR");
        if (!temp_dir) temp_dir = getenv("TMPDIR");
        if (!temp_dir) temp_dir = TMP_DIR;

        size_t len = strlen(temp_dir);
        if (len > 0 && temp_dir[len - 1] == '/') {
            snprintf(target_file_path, MAX_PATH_LEN, "%s%s", temp_dir, TEMP_FILENAME);
        } else {
            snprintf(target_file_path, MAX_PATH_LEN, "%s/%s", temp_dir, TEMP_FILENAME);
        }

        log_info("No target file specified, using temp file: %s", target_file_path);
    }

    int result = run_app(target_file_path);

    close_logs();
    return result;
}
