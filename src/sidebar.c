#include "sidebar.h"
#include <SDL2/SDL.h>

bool in_sidebar_bounds(int x, int y) {
    return x < SIDEBAR_WIDTH;
}

void draw_left_sidebar(SDL_Renderer *renderer, PaintContext *context, Config* config) {
    SDL_Rect sidebar = {0, 0, SIDEBAR_WIDTH, config->window_height};
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &sidebar);

    SDL_Rect brush_btn = {TOOL_BTN_X, TOOL_BTN_Y_START, TOOL_BTN_WIDTH, TOOL_BTN_HEIGHT};
    SDL_Rect eraser_btn = {TOOL_BTN_X, TOOL_BTN_Y_START + TOOL_BTN_HEIGHT + TOOL_BTN_SPACING, TOOL_BTN_WIDTH, TOOL_BTN_HEIGHT};

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &brush_btn);
    SDL_RenderFillRect(renderer, &eraser_btn);

    SDL_Rect *selected = context->current_tool.type == TOOL_BRUSH ? &brush_btn : &eraser_btn;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(renderer, selected);
}

void handle_sidebar_click(PaintContext *context, int mouse_x, int mouse_y) {
    if (mouse_x > SIDEBAR_WIDTH)
        return;

    if (mouse_x >= TOOL_BTN_X && mouse_x <= TOOL_BTN_X + TOOL_BTN_WIDTH) {
        if (mouse_y >= TOOL_BTN_Y_START && mouse_y <= TOOL_BTN_Y_START + TOOL_BTN_HEIGHT) {
            set_tool_type(&context->current_tool, TOOL_BRUSH);
        } else if (mouse_y >= TOOL_BTN_Y_START + TOOL_BTN_HEIGHT + TOOL_BTN_SPACING &&
                   mouse_y <= TOOL_BTN_Y_START + 2 * TOOL_BTN_HEIGHT + TOOL_BTN_SPACING) {
            set_tool_type(&context->current_tool, TOOL_ERASER);
        }
    }
}