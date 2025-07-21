#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture *tool_icons[16];  // Tool icons
    SDL_Texture *undo;
    SDL_Texture *redo;
    SDL_Texture *plus;
    SDL_Texture *minus;
} Assets;

/**
 * Loads all required UI assets.
 */
Assets *load_assets(SDL_Renderer *renderer);

/**
 * Frees all loaded assets.
 */
void free_assets(Assets *assets);

#endif // ASSETS_H
