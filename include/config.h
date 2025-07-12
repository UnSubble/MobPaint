#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
    char log_dir[256];
    char default_target_path[512];
    int window_width;
    int window_height;
    char default_tool[32];
    int brush_size;
    SDL_Color brush_color;
    SDL_Color default_background_color;
} Config;


bool load_config(const char *filename, Config *config);

#endif // CONFIG_H
