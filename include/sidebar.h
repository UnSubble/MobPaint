#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "tools/tools.h"
#include "context/paint_context.h"
#include "context/config.h"

// Sidebar dimensions
#define SIDEBAR_WIDTH         80
#define TOPBAR_HEIGHT         30

// Tool button layout in sidebar
#define TOOL_BTN_X            10
#define TOOL_BTN_Y_START      10
#define TOOL_BTN_WIDTH        60
#define TOOL_BTN_HEIGHT       60
#define TOOL_BTN_SPACING      10

/**
 * Checks if the given (x, y) position is within the left sidebar area.
 *
 * @param x Mouse X coordinate.
 * @param y Mouse Y coordinate.
 * @return true if the point is within the sidebar, false otherwise.
 */
bool in_sidebar_bounds(int x, int y);

/**
 * Draws the left sidebar including tool buttons.
 *
 * @param renderer SDL renderer used for drawing.
 * @param context  Pointer to the current PaintContext.
 * @param config   Pointer to the current configuration.
 */
void draw_left_sidebar(SDL_Renderer *renderer, PaintContext *context, Config *config);

/**
 * Handles click events that occur inside the left sidebar.
 * Updates tool selection based on clicked button.
 *
 * @param context Pointer to the PaintContext.
 * @param mouse_x Mouse X coordinate.
 * @param mouse_y Mouse Y coordinate.
 */
void handle_sidebar_click(PaintContext *context, int mouse_x, int mouse_y);

#endif // SIDEBAR_H
