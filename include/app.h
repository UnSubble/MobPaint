#ifndef APP_H
#define APP_H

#include "context/config.h"

/**
 * Starts the main application window and event loop.
 *
 * @param target_file_path Path to the target file (image or canvas) to load.
 * @param config          Pointer to the Config structure with application settings.
 * @return                0 on success, non-zero on failure.
 */
int run_app(const char *target_file_path, Config *config);

#endif // APP_H
