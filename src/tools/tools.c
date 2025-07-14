#include "tools/tools.h"
#include "context/paint_context.h"
#include "context/logs.h"
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
    } else if (strcasecmp(tool_name, "FILL") == 0) {
        return TOOL_FILL;
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
    case TOOL_FILL:
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

bool colors_equal(SDL_Color *a, SDL_Color *b) {
    return a->a == b->a && a->r == b->r && a->g == b->g && a->b == b->b;
}   

static SDL_Color get_pixel(SDL_Surface* surface, int x, int y) {
    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 pixel = pixels[(y * surface->w) + x];
    SDL_Color color;
    SDL_GetRGBA(pixel, surface->format, &color.r, &color.g, &color.b, &color.a);
    return color;
}

static void set_pixel(SDL_Surface* surface, int x, int y, SDL_Color color) {
    Uint32 pixel = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y * surface->w) + x] = pixel;
}

int flood_fill(SDL_Renderer *renderer, int start_x, int start_y, 
               SDL_Color target_color, SDL_Color fill_color, Point** out_points) {
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);
    
    if (start_x < 0 || start_y < 0 || start_x >= width || start_y >= height) {
        return -1;
    }
    
    if (colors_equal(&target_color, &fill_color)) {
        return -1;
    }
    
    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!surface) {
        log_error("Failed to create surface for flood fill");
        return -1;
    }
    
    SDL_Texture *prev_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    
    if (SDL_RenderReadPixels(renderer, NULL, surface->format->format,
                           surface->pixels, surface->pitch) != 0) {
        log_error("Failed to read pixels: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return -1;
    }
    
    SDL_Color start_pixel = get_pixel(surface, start_x, start_y);
    if (!colors_equal(&target_color, &start_pixel)) {
        SDL_FreeSurface(surface);
        return 0;
    }
    
    Point* stack = malloc(sizeof(Point) * width * height);
    if (!stack) {
        SDL_FreeSurface(surface);
        return -1;
    }
    
    Point* filled_points = malloc(sizeof(Point) * width * height);
    if (!filled_points) {
        free(stack);
        SDL_FreeSurface(surface);
        return -1;
    }
    
    int stack_top = 0;
    int filled_count = 0;
    
    stack[stack_top++] = (Point){start_x, start_y};
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    while (stack_top > 0) {
        Point current = stack[--stack_top];
        int x = current.x;
        int y = current.y;
        
        if (x < 0 || x >= width || y < 0 || y >= height) {
            continue;
        }
        
        SDL_Color pixel = get_pixel(surface, x, y);
        if (!colors_equal(&target_color, &pixel)) {
            continue;
        }
        
        set_pixel(surface, x, y, fill_color);
        filled_points[filled_count++] = (Point){x, y};
        
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                SDL_Color next_pixel = get_pixel(surface, nx, ny);
                if (colors_equal(&target_color, &next_pixel)) {
                    stack[stack_top++] = (Point){nx, ny};
                }
            }
        }
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        SDL_SetRenderTarget(renderer, prev_target);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_DestroyTexture(texture);
    }
    
    free(stack);
    SDL_FreeSurface(surface);
    
    if (out_points) {
        *out_points = filled_points;
    } else {
        free(filled_points);
    }
    
    return filled_count;
}

static SDL_Color get_pixel_with_renderer(SDL_Renderer *renderer, int x, int y) {
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    SDL_Color default_color = {0, 0, 0, 0};
    
    if (!surface) {
        log_error("Failed to create surface for getting target color");
        return default_color;
    }
    
    if (SDL_RenderReadPixels(renderer, NULL, surface->format->format,
                        surface->pixels, surface->pitch) != 0) {
        log_error("Failed to read pixels: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return default_color;
    }
    
    SDL_Color target_color = get_pixel(surface, x, y);
    SDL_FreeSurface(surface);
    
    return target_color; 
}

void use_tool(PaintContext* context, int prev_x, int prev_y) {
    Tool *tool = &context->current_tool; 
    SDL_SetRenderDrawColor(context->renderer, tool->color.r, tool->color.g, tool->color.b, tool->color.a);

    switch (tool->type) {
    case TOOL_BRUSH:
    case TOOL_ERASER: {
        add_point_to_current_stroke(context, context->mouse_x, context->mouse_y);
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
        add_point_to_current_stroke(context, context->mouse_x, context->mouse_y);
        if (prev_x != -1 && prev_y != -1 && context->mouse_x != -1 && context->mouse_y != -1) {
            draw_thick_line(context->renderer, prev_x, prev_y, context->mouse_x, context->mouse_y, tool->size);
        }
        break;
    }
    case TOOL_CIRCLE: {
        add_point_to_current_stroke(context, context->mouse_x, context->mouse_y);
        if (prev_x != -1 && prev_y != -1 && context->mouse_x != -1 && context->mouse_y != -1) {
            draw_thick_circle(context->renderer, prev_x, prev_y, context->mouse_x, context->mouse_y, tool->size);
        }
        break;
    }
    case TOOL_FILL: {
        Point* filled_points = NULL;
        SDL_Color target_color = get_pixel_with_renderer(context->renderer, context->mouse_x, context->mouse_y);
        SDL_Color fill_color = tool->color;
        int count = flood_fill(context->renderer, context->mouse_x, context->mouse_y, 
                            target_color, fill_color, &filled_points);

        if (count > 0 && filled_points) {
            for (int i = 0; i < count; i++) {
                add_point_to_current_stroke(context, filled_points[i].x, filled_points[i].y);
            }
            free(filled_points);
        } else if (count == -1) {
            log_error("Flood fill failed");
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
    case TOOL_FILL:
        return "FILL";
    default:
        return "UNKNOWN";
    }
}
