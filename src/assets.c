#include "assets.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>

static SDL_Texture *load_texture(SDL_Renderer *renderer, const char *path) {
    SDL_Surface *surf = IMG_Load(path);
    if (!surf) return NULL;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return tex;
}

Assets *load_assets(SDL_Renderer *renderer) {
    Assets *assets = malloc(sizeof(Assets));
    memset(assets, 0, sizeof(Assets));

    const char *tool_paths[] = {
        "assets/brush.png",
        "assets/eraser.png",
        "assets/line.png",
        "assets/circle.png",
        "assets/fill.png"
    };

    for (int i = 0; i < 5; i++) {
        assets->tool_icons[i] = load_texture(renderer, tool_paths[i]);
    }

    assets->undo  = load_texture(renderer, "assets/undo.png");
    assets->redo  = load_texture(renderer, "assets/redo.png");
    assets->plus  = load_texture(renderer, "assets/plus.png");
    assets->minus = load_texture(renderer, "assets/minus.png");

    return assets;
}

void free_assets(Assets *assets) {
    for (int i = 0; i < 5; i++) {
        if (assets->tool_icons[i]) SDL_DestroyTexture(assets->tool_icons[i]);
    }

    SDL_DestroyTexture(assets->undo);
    SDL_DestroyTexture(assets->redo);
    SDL_DestroyTexture(assets->plus);
    SDL_DestroyTexture(assets->minus);
    free(assets);
}
