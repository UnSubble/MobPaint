#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <SDL2/SDL.h>

// Maximum lengths for configurable string fields
#define LOG_DIR_MAX_LEN            256
#define TARGET_PATH_MAX_LEN        512
#define TOOL_NAME_MAX_LEN           32

// Configuration structure for MobPaint
typedef struct {
    // Paths
    char log_dir[LOG_DIR_MAX_LEN];
    char default_target_path[TARGET_PATH_MAX_LEN];

    // Window dimensions
    int window_width;
    int window_height;

    // Tool settings
    char default_tool[TOOL_NAME_MAX_LEN];
    int brush_size;
    SDL_Color brush_color;

    // Background settings
    SDL_Color default_background_color;
} Config;

/**
 * Loads the configuration from the specified file into the given Config struct.
 *
 * @param filename The path to the config file.
 * @param config   Pointer to the Config structure to populate.
 * @return true if loading is successful, false otherwise.
 */
bool load_config(const char *filename, Config *config);

#endif // CONFIG_H
