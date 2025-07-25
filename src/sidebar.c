#include "sidebar.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "assets.h"

extern Assets *global_assets;

typedef enum {
    TOPBAR_BTN_UNDO,
    TOPBAR_BTN_REDO,
    TOPBAR_BTN_SIZE_INC,
    TOPBAR_BTN_SIZE_DEC,
    TOPBAR_BTN_COUNT
} TopbarButton;

void draw_topbar(SDL_Renderer *renderer, PaintContext *context, Config *config, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
    SDL_Rect bar = {0, 0, config->window_width, TOPBAR_HEIGHT};
    SDL_RenderFillRect(renderer, &bar);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    int x = SIDEBAR_WIDTH + TOPBAR_BTN_SPACING;

    for (int i = 0; i < TOPBAR_BTN_COUNT; ++i) {
        SDL_Rect btn = {
            .x = x,
            .y = 0,
            .w = TOPBAR_BTN_WIDTH,
            .h = TOPBAR_HEIGHT
        };
        SDL_RenderDrawRect(renderer, &btn);

        SDL_Texture *icon = NULL;
        switch ((TopbarButton)i) {
        case TOPBAR_BTN_UNDO:       
            icon = global_assets->undo;  
            break;
        case TOPBAR_BTN_REDO:       
            icon = global_assets->redo;  
            break;
        case TOPBAR_BTN_SIZE_INC:   
            icon = global_assets->plus;  
            break;
        case TOPBAR_BTN_SIZE_DEC:   
            icon = global_assets->minus; 
            break;
        default: 
            break;
        }
        if (icon) SDL_RenderCopy(renderer, icon, NULL, &btn);
        x += TOPBAR_BTN_WIDTH + TOPBAR_BTN_SPACING;
    }

    char size_text[32];
    snprintf(size_text, sizeof(size_text), "Size: %d", context->current_tool.size);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, size_text, white);
    if (surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect text_rect = {
            .x = config->window_width - surface->w - 10,
            .y = (TOPBAR_HEIGHT - surface->h) / 2,
            .w = surface->w,
            .h = surface->h
        };
        SDL_RenderCopy(renderer, texture, NULL, &text_rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void handle_topbar_click(PaintContext *context, int mouse_x, int mouse_y) {
    if (mouse_y > TOPBAR_HEIGHT) return;

    int x = SIDEBAR_WIDTH + TOPBAR_BTN_SPACING;

    for (int i = 0; i < TOPBAR_BTN_COUNT; ++i) {
        if (mouse_x >= x && mouse_x <= x + TOPBAR_BTN_WIDTH) {
            switch ((TopbarButton)i) {
            case TOPBAR_BTN_UNDO:       
                paint_context_undo(context); 
                break;
            case TOPBAR_BTN_REDO:       
                paint_context_redo(context); 
                break;
            case TOPBAR_BTN_SIZE_INC:   
                if (context->current_tool.size < 50) 
                    context->current_tool.size++; 
                break;
            case TOPBAR_BTN_SIZE_DEC:   
                if (context->current_tool.size > 1)  
                    context->current_tool.size--; 
                break;
            default: 
                break;
            }
            return;
        }
        x += TOPBAR_BTN_WIDTH + TOPBAR_BTN_SPACING;
    }
}

void draw_left_sidebar(SDL_Renderer *renderer, PaintContext *context, Config *config) {
    SDL_Rect sidebar = {0, 0, SIDEBAR_WIDTH, config->window_height};
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &sidebar);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    SDL_Rect selected;
    bool selected_set = false;

    int y = TOOL_BTN_Y_START + TOPBAR_HEIGHT;
    for (int i = 0; i < TOOL_COUNT; ++i) {
        SDL_Rect btn = {
            .x = TOOL_BTN_X,
            .y = y,
            .w = TOOL_BTN_WIDTH,
            .h = TOOL_BTN_HEIGHT
        };
        SDL_RenderDrawRect(renderer, &btn);

        if (global_assets->tool_icons[i]) {
            SDL_RenderCopy(renderer, global_assets->tool_icons[i], NULL, &btn);
        }

        if (context->current_tool.type == (ToolType)i) {
            selected = btn;
            selected_set = true;
        }

        y += TOOL_BTN_HEIGHT + TOOL_BTN_SPACING;
    }

    if (selected_set) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &selected);
    }
}

void handle_sidebar_click(PaintContext *context, int mouse_x, int mouse_y) {
    if (mouse_x > SIDEBAR_WIDTH || mouse_x < TOOL_BTN_X ||
        mouse_x > TOOL_BTN_X + TOOL_BTN_WIDTH || mouse_y < TOOL_BTN_Y_START + TOPBAR_HEIGHT)
        return;

    int relative_y = mouse_y - (TOOL_BTN_Y_START + TOPBAR_HEIGHT);
    int index = relative_y / (TOOL_BTN_HEIGHT + TOOL_BTN_SPACING);

    if (index >= 0 && index < TOOL_COUNT) {
        set_tool_type(&context->current_tool, (ToolType)index);
    }
}

bool in_sidebar_bounds(int x, int y) {
    return x < SIDEBAR_WIDTH || y < TOPBAR_HEIGHT;
}