#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include "context/config.h"
typedef struct PaintContext PaintContext;

typedef enum {
    TOOL_BRUSH,
    TOOL_ERASER,
    /* TOOL_LINE,
    TOOL_RECT,
    TOOL_FILL,
    TOOL_COUNT */
} ToolType;

typedef struct Tool {
    ToolType type;
    SDL_Color color;
    int size;
} Tool;

void init_tool(Tool *tool, Config *config);
void set_tool_type(Tool *tool, ToolType type);
void use_tool(PaintContext *context, int prev_x, int prev_y);
char* get_tool_name(Tool *tool);

#endif // TOOLS_H
