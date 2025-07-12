#ifndef APP_H
#define APP_H

#include "context/config.h"

// Starts the main application window and event loop.
// Receives the path to the target file (image or canvas).
// Returns 0 on success, non-zero on failure.
int run_app(const char *target_file_path, Config *config);

#endif // APP_H
