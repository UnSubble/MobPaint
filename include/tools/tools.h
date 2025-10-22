#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "context/config.h"

typedef struct PaintContext PaintContext;

typedef struct Point Point;

// Supported drawing tool types
typedef enum {
    TOOL_BRUSH = 0,
    TOOL_ERASER = 1,
    TOOL_LINE = 2,
    TOOL_CIRCLE = 3,
    TOOL_FILL = 4,
    TOOL_TEXT = 5,
    TOOL_COUNT = 6, // Always at the bottom
    // TOOL_RECT,
    // TOOL_COUNT
} ToolType;

// Represents the current drawing tool's configuration
typedef struct Tool {
    ToolType type;      // Tool type (e.g., brush, eraser)
    SDL_Color color;    // Tool color
    int size;           // Tool size or thickness
} Tool;

/**
 * Initializes the given Tool using default values from configuration.
 *
 * @param tool   Pointer to the Tool to initialize.
 * @param config Pointer to the configuration struct.
 */
void init_tool(Tool *tool, const Config *config);

/**
 * Updates the tool type and adjusts internal state if needed.
 *
 * @param tool Pointer to the Tool to modify.
 * @param type New ToolType value to apply.
 */
void set_tool_type(Tool *tool, ToolType type);

/**
 * Applies the current tool’s effect to the canvas via PaintContext.
 * Used for real-time drawing actions.
 *
 * @param context Pointer to the PaintContext.
 * @param prev_x  Previous X coordinate (used for interpolation).
 * @param prev_y  Previous Y coordinate.
 */
void use_tool(PaintContext *context, int prev_x, int prev_y);

/**
 * Draws a thick line between two points with the specified size.
 *
 * @param renderer SDL renderer to use.
 * @param x1       Start X position.
 * @param y1       Start Y position.
 * @param x2       End X position.
 * @param y2       End Y position.
 * @param size     Line thickness.
 */
void draw_thick_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int size);

/**
 * Draws a thick circle outline between two points using bounding box logic.
 *
 * @param renderer SDL renderer to use.
 * @param x1       First point (defines center/radius).
 * @param y1       First point.
 * @param x2       Second point (defines radius).
 * @param y2       Second point.
 * @param size     Circle outline thickness.
 */
void draw_thick_circle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int size);

/**
 * Performs flood fill (bucket fill) operation starting at a given pixel.
 * Optionally outputs the filled pixels list via `out_points`.
 *
 * @param renderer     SDL renderer (target canvas).
 * @param start_x      X coordinate to start filling.
 * @param start_y      Y coordinate to start filling.
 * @param target_color Color to be replaced.
 * @param fill_color   Fill color to apply.
 * @param out_points   Optional output: pointer to an array of filled points (can be NULL).
 * @return             Number of pixels filled, or -1 on error.
 */
int flood_fill(SDL_Renderer *renderer, int start_x, int start_y,
               SDL_Color target_color, SDL_Color fill_color, Point **out_points);

/**
 * Returns a human-readable name string for the given tool.
 *
 * @param tool Pointer to the Tool.
 * @return     Static string describing the tool (e.g., "Brush").
 */
const char* get_tool_name(const Tool *tool);

/**
 * Renders text at the specified position using the given font and color.
 *
 * @param renderer SDL renderer to use for drawing.
 * @param font     TTF font to use for text rendering.
 * @param text     Text string to render.
 * @param x        X position to render the text.
 * @param y        Y position to render the text.
 * @param color    Color to use for the text.
 */
void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color);

#endif // TOOLS_H
