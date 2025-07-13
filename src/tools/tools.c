#include "tools/tools.h"
#include "context/paint_context.h"
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#define M_PI_2 1.57079632679489661923f
#endif

ToolType get_tooltype_from_string(const char *tool_name) {
    if (strcasecmp(tool_name, "BRUSH") == 0) {
        return TOOL_BRUSH;
    } else if (strcasecmp(tool_name, "ERASER") == 0) {
        return TOOL_ERASER;
    } else if (strcasecmp(tool_name, "LINE") == 0) {
        return TOOL_LINE;
    } else if (strcasecmp(tool_name, "CIRCLE") == 0) {
        return TOOL_CIRCLE;
    } else {
        return -1;
    }
}

void init_tool(Tool *tool, const Config *config) {
    if (config) {
        tool->type = get_tooltype_from_string(config->default_tool);
        tool->size = config->brush_size;
        tool->color = config->brush_color;
    } else {
        tool->type = TOOL_BRUSH;                 // Default type
        tool->color = (SDL_Color){0, 0, 0, 255}; // Default black
        tool->size = 4;                          // Default size
    }
}

void set_tool_type(Tool *tool, ToolType type) {
    tool->type = type;

    switch (type) {
    case TOOL_CIRCLE:
    case TOOL_LINE:
    case TOOL_BRUSH:
        tool->color = (SDL_Color){0, 0, 0, 255};  // Black
        break;
    case TOOL_ERASER:
        tool->color = (SDL_Color){255, 255, 255, 255};  // White (erase)
        break;
    default:
        tool->color = (SDL_Color){128, 128, 128, 255}; // Unknown
        break;
    }
}

void draw_thick_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int size) {
    const float dx = x2 - x1;
    const float dy = y2 - y1;
    const float distance = SDL_sqrtf(dx * dx + dy * dy);

    if (distance == 0) 
        return;

    const float step_x = dx / distance;
    const float step_y = dy / distance;

    for (float i = 0; i <= distance; i += 1.0f) {
        int px = (int)(x1 + step_x * i);
        int py = (int)(y1 + step_y * i);

        SDL_Rect brush = {
            px - size / 2,
            py - size / 2,
            size,
            size
        };
        SDL_RenderFillRect(renderer, &brush);
    }
}

void draw_thick_circle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int size) {
    const float dx = x2 - x1;
    const float dy = y2 - y1;
    const float distance = SDL_sqrtf(dx * dx + dy * dy);

    if (distance == 0) 
        return;

    const float cx = (x2 + x1) / 2.0f;
    const float cy = (y2 + y1) / 2.0f;
    const float radius = distance / 2.0f;
    
    const int segments = (int)SDL_powf(2, M_PI * SDL_logf(radius));
    const float step = 2.0f * (float)M_PI / segments;

    for (int r = 0; r < size; r++) {
        float current_radius = radius - size / 2 + r;
        for (float theta = 0.0f; theta < 2.0f * (float)M_PI; theta += step) {
            int x = (int)(cx + current_radius * SDL_cosf(theta));
            int y = (int)(cy + current_radius * SDL_sinf(theta));
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void use_tool(PaintContext* context, int prev_x, int prev_y) {
    Tool *tool = &context->current_tool; 
    SDL_SetRenderDrawColor(context->renderer, tool->color.r, tool->color.g, tool->color.b, tool->color.a);

    switch (tool->type) {
    case TOOL_BRUSH:
    case TOOL_ERASER: {
        if (prev_x != -1 && prev_y != -1) {
            draw_thick_line(context->renderer, prev_x, prev_y, context->mouse_x, context->mouse_y, tool->size);
        } else {
            SDL_Rect brush = {
                context->mouse_x - tool->size / 2,
                context->mouse_y - tool->size / 2,
                tool->size,
                tool->size
            };
            SDL_RenderFillRect(context->renderer, &brush);
        }
        break;
    }
    case TOOL_LINE: {
        if (prev_x != -1 && prev_y != -1 && context->mouse_x != -1 && context->mouse_y != -1) {
            draw_thick_line(context->renderer, prev_x, prev_y, context->mouse_x, context->mouse_y, tool->size);
        }
        break;
    }
    case TOOL_CIRCLE: {
        if (prev_x != -1 && prev_y != -1 && context->mouse_x != -1 && context->mouse_y != -1) {
            draw_thick_circle(context->renderer, prev_x, prev_y, context->mouse_x, context->mouse_y, tool->size);
        }
        break;
    }
    default:
        break;
    }
}

const char* get_tool_name(const Tool* tool) {
    switch (tool->type) {
    case TOOL_BRUSH:
        return "BRUSH";
    case TOOL_ERASER:
        return "ERASER";
    case TOOL_LINE:
        return "LINE";
    case TOOL_CIRCLE:
        return "CIRCLE";
    default:
        return "UNKNOWN";
    }
}
