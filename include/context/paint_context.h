#ifndef PAINT_CONTEXT_H
#define PAINT_CONTEXT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "tools/tools.h"
#include "context/history.h"
#include "config.h"

// Threshold to decide when to cache the bitmap for performance
#define CACHE_THRESHOLD 1000

typedef struct PaintContext {
    SDL_Renderer *renderer;         // SDL renderer for drawing
    SDL_Texture *bitmap_cache;      // Cached texture for optimized redraws
    int mouse_x;                    // Current mouse X position
    int mouse_y;                    // Current mouse Y position
    Tool current_tool;              // Currently selected drawing tool
    History *undo_stack;            // Stack holding undo history entries
    History *redo_stack;            // Stack holding redo history entries
    HistoryEntry *current_stroke;   // Points collected in the current stroke
    int committed_stroke_count;    // Count of finalized strokes
    
    // Text input state
    bool text_input_active;         // Whether text input is active
    char text_input_buffer[256];    // Buffer for text input
    int text_input_x;               // X position for text input
    int text_input_y;               // Y position for text input
    bool text_placed;               // Whether text has been placed in current click
} PaintContext;

/**
 * Initializes the PaintContext structure.
 *
 * @param renderer      SDL_Renderer pointer to draw on.
 * @param paint_context Pointer to the PaintContext to initialize.
 * @param config        Pointer to Config with initial settings.
 * @param current_tool  Initial tool to select.
 */
void init_paint_context(SDL_Renderer *renderer, PaintContext *paint_context, Config *config, Tool current_tool);

/**
 * Starts a new stroke (drawing action).
 *
 * @param paint_context Pointer to PaintContext.
 */
void start_stroke(PaintContext *paint_context);

/**
 * Adds a point to the current stroke.
 *
 * @param paint_context Pointer to PaintContext.
 * @param x             X coordinate.
 * @param y             Y coordinate.
 */
void add_point_to_current_stroke(PaintContext *paint_context, int x, int y);

/**
 * Ends the current stroke and commits it to undo history.
 *
 * @param paint_context Pointer to PaintContext.
 */
void end_stroke(PaintContext *paint_context);

/**
 * Updates the current mouse coordinates.
 *
 * @param paint_context Pointer to PaintContext.
 * @param x             New mouse X coordinate.
 * @param y             New mouse Y coordinate.
 */
void update_coordinates(PaintContext *paint_context, int x, int y);

/**
 * Changes the currently selected tool.
 *
 * @param paint_context Pointer to PaintContext.
 * @param tool          Tool to select.
 */
void update_tool(PaintContext *paint_context, Tool tool);

/**
 * Performs an undo operation if possible.
 *
 * @param paint_context Pointer to PaintContext.
 * @return true if undo was successful, false if no undo available.
 */
bool paint_context_undo(PaintContext *paint_context);

/**
 * Performs a redo operation if possible.
 *
 * @param paint_context Pointer to PaintContext.
 * @return true if redo was successful, false if no redo available.
 */
bool paint_context_redo(PaintContext *paint_context);

/**
 * Redraws the entire canvas, including committed strokes.
 *
 * @param paint_context Pointer to PaintContext.
 */
void redraw_canvas(PaintContext *paint_context);

/**
 * Frees all allocated resources in PaintContext.
 *
 * @param paint_context Pointer to PaintContext.
 */
void free_paint_context(PaintContext *paint_context);

/**
 * Starts text input mode at the specified coordinates.
 *
 * @param paint_context Pointer to PaintContext.
 * @param x             X coordinate for text input.
 * @param y             Y coordinate for text input.
 */
void start_text_input(PaintContext *paint_context, int x, int y);

/**
 * Handles text input events and updates the input buffer.
 *
 * @param paint_context Pointer to PaintContext.
 * @param text          Text to add to the buffer.
 * @return              true if text input is still active, false if completed.
 */
bool handle_text_input(PaintContext *paint_context, const char *text);

/**
 * Handles key events for text input (like backspace, enter).
 *
 * @param paint_context Pointer to PaintContext.
 * @param key           SDL key code.
 * @return              true if text input is still active, false if completed.
 */
bool handle_text_key(PaintContext *paint_context, SDL_Keycode key);

/**
 * Finalizes text input and renders the text to the canvas.
 *
 * @param paint_context Pointer to PaintContext.
 */
void finalize_text_input(PaintContext *paint_context);

/**
 * Cancels text input without rendering.
 *
 * @param paint_context Pointer to PaintContext.
 */
void cancel_text_input(PaintContext *paint_context);

#endif // PAINT_CONTEXT_H
