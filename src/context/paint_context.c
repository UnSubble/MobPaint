#include "context/paint_context.h"
#include <stdlib.h>
#include <stdbool.h>

void apply_history_entry(SDL_Renderer *renderer, const HistoryEntry *entry);

static HistoryEntry* create_empty_entry(Tool tool) {
    HistoryEntry *entry = malloc(sizeof(HistoryEntry));
    if (!entry) return NULL;
    entry->points = NULL;
    entry->count = 0;
    entry->capacity = 0;
    
    entry->tool.type = tool.type;
    entry->tool.size = tool.size;
    entry->tool.color = tool.color;

    return entry;
}

void init_paint_context(SDL_Renderer *renderer, PaintContext *paint_context, Config* config, Tool current_tool) {
    if (!paint_context) return;

    paint_context->renderer = renderer;
    paint_context->current_tool = current_tool;
    paint_context->mouse_x = -1;
    paint_context->mouse_y = -1;
    paint_context->committed_stroke_count = 0;

    paint_context->undo_stack = malloc(sizeof(History));
    paint_context->redo_stack = malloc(sizeof(History));
    paint_context->current_stroke = NULL;

    if (paint_context->undo_stack) init_history(paint_context->undo_stack);
    if (paint_context->redo_stack) init_history(paint_context->redo_stack);

    paint_context->bitmap_cache = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        config->window_width, config->window_height
    );

    if (paint_context->bitmap_cache) {
        SDL_SetRenderTarget(renderer, paint_context->bitmap_cache);
        SDL_SetRenderDrawColor(renderer, config->default_background_color.r,
                                          config->default_background_color.g,
                                          config->default_background_color.b,
                                          config->default_background_color.a);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, NULL);
    }
}

void start_stroke(PaintContext *paint_context) {
    if (!paint_context) return;
    paint_context->current_stroke = create_empty_entry(paint_context->current_tool);
}

void add_point_to_current_stroke(PaintContext *paint_context, int x, int y) {
    if (!paint_context || !paint_context->current_stroke) return;

    if (paint_context->current_stroke->count >= paint_context->current_stroke->capacity) {
        int new_capacity = paint_context->current_stroke->capacity ? paint_context->current_stroke->capacity * 2 : 8;
        Point *new_points = realloc(paint_context->current_stroke->points, new_capacity * sizeof(Point));
        if (!new_points) return;
        paint_context->current_stroke->points = new_points;
        paint_context->current_stroke->capacity = new_capacity;
    }

    paint_context->current_stroke->points[paint_context->current_stroke->count++] = (Point){x, y};
}

void end_stroke(PaintContext *paint_context) {
    if (!paint_context || !paint_context->current_stroke) return;

    if (paint_context->current_stroke->count > 0) {
        push_history(paint_context->undo_stack, *(paint_context->current_stroke));
        free_history(paint_context->redo_stack);
        init_history(paint_context->redo_stack);

        int uncommitted = paint_context->undo_stack->count - paint_context->committed_stroke_count;
        if (uncommitted >= CACHE_THRESHOLD && paint_context->bitmap_cache) {
            SDL_SetRenderTarget(paint_context->renderer, paint_context->bitmap_cache);
            for (int i = paint_context->committed_stroke_count; i < paint_context->undo_stack->count; i++) {
                apply_history_entry(paint_context->renderer, &paint_context->undo_stack->entries[i]);
            }
            paint_context->committed_stroke_count = paint_context->undo_stack->count;
            SDL_SetRenderTarget(paint_context->renderer, NULL);
        }
    }

    free(paint_context->current_stroke->points);
    free(paint_context->current_stroke);
    paint_context->current_stroke = NULL;
}

void update_coordinates(PaintContext *paint_context, int x, int y) {
    if (paint_context) {
        paint_context->mouse_x = x;
        paint_context->mouse_y = y;
    }
}

void update_tool(PaintContext *paint_context, Tool tool) {
    if (paint_context) {
        paint_context->current_tool = tool;
    }
}

void apply_history_entry(SDL_Renderer *renderer, const HistoryEntry *entry) {
    if (!renderer || !entry || entry->count < 2) 
        return;

    SDL_SetRenderDrawColor(renderer,
        entry->tool.color.r,
        entry->tool.color.g,
        entry->tool.color.b,
        entry->tool.color.a);

    SDL_Rect rect;
    rect.w = entry->tool.size;
    rect.h = entry->tool.size;

    for (int i = 0; i < entry->count; i++) {
        rect.x = entry->points[i].x - rect.w / 2;
        rect.y = entry->points[i].y - rect.h / 2;
        SDL_RenderFillRect(renderer, &rect);
    }
}

void redraw_canvas(PaintContext *paint_context) {
    if (!paint_context || !paint_context->renderer) 
        return;

    SDL_SetRenderTarget(paint_context->renderer, NULL);
    SDL_SetRenderDrawColor(paint_context->renderer, 255, 255, 255, 255);
    SDL_RenderClear(paint_context->renderer);

    for (int i = 0; i < paint_context->undo_stack->count; i++) {
        apply_history_entry(paint_context->renderer, &paint_context->undo_stack->entries[i]);
    }
}

static bool exchange_history(PaintContext *ctx, History *from, History *to) {
    if (!from || !to || is_history_empty(from)) return false;

    HistoryEntry entry = pop_history(from);
    push_history(to, entry);

    redraw_canvas(ctx);
    return true;
}

bool paint_context_undo(PaintContext *paint_context) {
    return paint_context && exchange_history(paint_context, paint_context->undo_stack, paint_context->redo_stack);
}

bool paint_context_redo(PaintContext *paint_context) {
    return paint_context && exchange_history(paint_context, paint_context->redo_stack, paint_context->undo_stack);
}

void free_paint_context(PaintContext *ctx) {
    if (!ctx) 
        return;

    free_history(ctx->undo_stack);
    free(ctx->undo_stack);

    free_history(ctx->redo_stack);
    free(ctx->redo_stack);

    if (ctx->current_stroke) {
        free(ctx->current_stroke->points);
        free(ctx->current_stroke);
        ctx->current_stroke = NULL;
    }

    if (ctx->bitmap_cache) {
        SDL_DestroyTexture(ctx->bitmap_cache);
        ctx->bitmap_cache = NULL;
    }
}
