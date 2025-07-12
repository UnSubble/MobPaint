#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "context/logs.h"
#include "context/config.h"

#define MAX_PATH_LEN 512

extern int run_app(const char *target_file_path, Config *config);

int main(int argc, char *argv[]) {
    if (!init_logs()) {
        fprintf(stderr, "Failed to initialize logs.\n");
        return 1;
    }

    Config config;
    load_config("config.json", &config);

    char target_file_path[MAX_PATH_LEN] = {0};

    if (argc > 1) {
        snprintf(target_file_path, MAX_PATH_LEN, "%s", argv[1]);
        log_info("User specified target file: %s", target_file_path);
    } else if (strlen(config.default_target_path) > 0) {
        snprintf(target_file_path, MAX_PATH_LEN, "%s", config.default_target_path);
        log_info("Using default target path from config: %s", target_file_path);
    } else {
        const char *temp_dir = getenv("MOBPAINT_TMPDIR");
        if (!temp_dir) temp_dir = getenv("TMPDIR");
        if (!temp_dir) temp_dir = "/tmp/";

        size_t len = strlen(temp_dir);
        if (len > 0 && temp_dir[len - 1] == '/') {
            snprintf(target_file_path, MAX_PATH_LEN, "%smobpaint_temp.dat", temp_dir);
        } else {
            snprintf(target_file_path, MAX_PATH_LEN, "%s/mobpaint_temp.dat", temp_dir);
        }

        log_info("No target or config path, using fallback temp file: %s", target_file_path);
    }

    int result = run_app(target_file_path, &config);

    close_logs();
    return result;
}
