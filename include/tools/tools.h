#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include "context/config.h"

typedef struct PaintContext PaintContext;

// Supported tool types enumeration
typedef enum {
    TOOL_BRUSH,
    TOOL_ERASER,
    // TOOL_LINE,
    // TOOL_RECT,
    // TOOL_FILL,
    // TOOL_COUNT
} ToolType;

// Tool structure holding type, color, and size information
typedef struct {
    ToolType type;   // Tool type (brush, eraser, etc.)
    SDL_Color color; // Tool color
    int size;        // Tool size (e.g., brush size)
} Tool;

/**
 * Initializes the given Tool based on configuration defaults.
 *
 * @param tool   Pointer to the Tool to initialize.
 * @param config Pointer to the Config struct with default settings.
 */
void init_tool(Tool *tool, const Config *config);

/**
 * Changes the Tool's type and adjusts properties accordingly.
 *
 * @param tool Pointer to the Tool to modify.
 * @param type New tool type.
 */
void set_tool_type(Tool *tool, ToolType type);

/**
 * Applies the tool's effect on the PaintContext.
 * Usually called when drawing or erasing between points.
 *
 * @param context Pointer to the PaintContext.
 * @param prev_x  Previous X coordinate (for line interpolation).
 * @param prev_y  Previous Y coordinate.
 */
void use_tool(PaintContext *context, int prev_x, int prev_y);

/**
 * Draws a thick line between two points using the current renderer.
 *
 * @param renderer SDL renderer to use.
 * @param x1       Starting X coordinate.
 * @param y1       Starting Y coordinate.
 * @param x2       Ending X coordinate.
 * @param y2       Ending Y coordinate.
 * @param size     Thickness of the line in pixels.
 */
void draw_thick_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int size);

/**
 * Returns a human-readable name of the tool.
 *
 * @param tool Pointer to the Tool.
 * @return Pointer to a static string with the tool name.
 */
const char* get_tool_name(const Tool *tool);

#endif // TOOLS_H
