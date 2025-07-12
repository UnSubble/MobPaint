#ifndef PAINT_CONTEXT_H
#define PAINT_CONTEXT_H

#include <SDL2/SDL.h>
#include "tools/tools.h"
#include "context/history.h"

#define CACHE_THRESHOLD 1000

typedef struct PaintContext {
    SDL_Renderer *renderer;
    SDL_Texture *bitmap_cache;
    int mouse_x, mouse_y;
    Tool current_tool;
    History *undo_stack;
    History *redo_stack;
    HistoryEntry *current_stroke;
    int committed_stroke_count;
} PaintContext;

void init_paint_context(SDL_Renderer *renderer, PaintContext *paint_context, Config* config, Tool current_tool);
void start_stroke(PaintContext *paint_context);
void add_point_to_current_stroke(PaintContext *paint_context, int x, int y);
void end_stroke(PaintContext *paint_context);
void update_coordinates(PaintContext *paint_context, int x, int y);
void update_tool(PaintContext *paint_context, Tool tool);
bool paint_context_undo(PaintContext *paint_context);
bool paint_context_redo(PaintContext *paint_context);
void redraw_canvas(PaintContext *paint_context);
void free_paint_context(PaintContext *paint_context);

#endif // PAINT_CONTEXT_H