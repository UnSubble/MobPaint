#include "sidebar.h"
#include <SDL2/SDL.h>

typedef enum {
    TOPBAR_BTN_UNDO,
    TOPBAR_BTN_REDO,
    TOPBAR_BTN_SIZE_INC,
    TOPBAR_BTN_SIZE_DEC,
    TOPBAR_BTN_COUNT
} TopbarButton;

void draw_topbar(SDL_Renderer *renderer, PaintContext *context, Config *config) {
    SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
    SDL_Rect bar = {0, 0, config->window_width, TOPBAR_HEIGHT};
    SDL_RenderFillRect(renderer, &bar);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    int x = SIDEBAR_WIDTH + TOPBAR_BTN_SPACING;
    for (int i = 0; i < TOPBAR_BTN_COUNT; ++i) {
        SDL_Rect btn = {x, 0, TOPBAR_BTN_WIDTH, TOPBAR_BTN_HEIGHT};
        SDL_RenderDrawRect(renderer, &btn);
        x += TOPBAR_BTN_WIDTH + TOPBAR_BTN_SPACING;
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
                default: break;
            }
            return;
        }
        x += TOPBAR_BTN_WIDTH + TOPBAR_BTN_SPACING;
    }
}

bool in_sidebar_bounds(int x, int y) {
    return x < SIDEBAR_WIDTH || y < TOPBAR_HEIGHT;
}

SDL_Rect *add_new_button(SDL_Renderer *renderer, SDL_Rect *last) {
    int last_y = TOOL_BTN_Y_START + TOPBAR_HEIGHT;
    if (last)
        last_y = last->y + TOOL_BTN_SPACING + TOOL_BTN_HEIGHT;
    SDL_Rect *new_button =  malloc(sizeof(SDL_Rect));
    new_button->x = TOOL_BTN_X;
    new_button->y = last_y;
    new_button->w = TOOL_BTN_WIDTH;
    new_button->h = TOOL_BTN_HEIGHT;
    SDL_RenderFillRect(renderer, new_button);
    return new_button;
}

void draw_left_sidebar(SDL_Renderer *renderer, PaintContext *context, Config* config) {
    SDL_Rect sidebar = {0, 0, SIDEBAR_WIDTH, config->window_height};
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &sidebar);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    SDL_Rect *selected = NULL;
    SDL_Rect *last_btn = NULL;

    for (int i = 0; i < TOOL_COUNT; i++) {
        SDL_Rect *btn = add_new_button(renderer, last_btn);
        if (context->current_tool.type == (ToolType)i) {
            selected = btn;
        }
        last_btn = btn;
    }

    if (selected) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(renderer, selected);
    }
}

void handle_sidebar_click(PaintContext *context, int mouse_x, int mouse_y) {
    if (mouse_x > SIDEBAR_WIDTH || mouse_x < TOOL_BTN_X ||
        mouse_x > TOOL_BTN_X + TOOL_BTN_WIDTH || mouse_y < TOOL_BTN_Y_START + TOPBAR_HEIGHT)
        return;

    int relative_y = mouse_y - (TOOL_BTN_Y_START + TOPBAR_HEIGHT);
    int total_height = TOOL_BTN_HEIGHT + TOOL_BTN_SPACING;

    int clicked_index = relative_y / total_height;

    if (clicked_index >= 0 && clicked_index < TOOL_COUNT) {
        set_tool_type(&context->current_tool, (ToolType)clicked_index);
    }
}
