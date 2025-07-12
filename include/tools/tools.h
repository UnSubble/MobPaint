#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>

typedef enum {
    TOOL_BRUSH,
    TOOL_ERASER,
    /* TOOL_LINE,
    TOOL_RECT,
    TOOL_FILL,
    TOOL_COUNT */
} ToolType;

typedef struct {
    ToolType type;
    SDL_Color color;
    int size;
} Tool;

void init_tool(Tool *tool);
void set_tool_type(Tool *tool, ToolType type);
void use_tool(SDL_Renderer *renderer, Tool *tool, int x, int y, int prev_x, int prev_y);
char* get_tool_name(Tool *tool);

#endif // TOOLS_H
