#include "tools/tools.h"

void init_tool(Tool *tool) {
    tool->type = TOOL_BRUSH;
    tool->color = (SDL_Color){0, 0, 0, 255};
    tool->size = 4;
}

void set_tool_type(Tool *tool, ToolType type) {
    tool->type = type;

    switch (type) {
    case TOOL_BRUSH:
        tool->color = (SDL_Color){0, 0, 0, 255};
        break;
    case TOOL_ERASER:
        tool->color = (SDL_Color){255, 255, 255, 255};
        break;
    default:
        tool->color = (SDL_Color){128, 128, 128, 255};
        break;
    }
}

void use_tool(SDL_Renderer *renderer, Tool *tool, int x, int y, int prev_x, int prev_y) {
    SDL_SetRenderDrawColor(renderer, tool->color.r, tool->color.g, tool->color.b, tool->color.a);

    switch (tool->type) {
    case TOOL_BRUSH:
    case TOOL_ERASER: {
            SDL_Rect brush = {
                x - tool->size / 2,
                y - tool->size / 2,
                tool->size,
                tool->size
            };
            SDL_RenderFillRect(renderer, &brush);
            if (prev_x != -1 && prev_y != -1) {
                SDL_RenderDrawLine(renderer, prev_x, prev_y, x, y);
            }
            break;
    }
    default:
        break;
    }
}

char* get_tool_name(Tool* tool) {
    switch (tool->type)
    {
    case TOOL_BRUSH:
        return "BRUSH";
        break;
    case TOOL_ERASER:
        return "ERASER";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}