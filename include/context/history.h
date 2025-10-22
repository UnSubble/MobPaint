#ifndef HISTORY_H
#define HISTORY_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "tools/tools.h"

// Represents a 2D coordinate on the canvas
typedef struct Point {
    int x;
    int y;
} Point;

// Represents a single drawing action (e.g., brush stroke)
typedef struct {
    Point *points;      // Dynamic array of drawn points
    int count;          // Number of points currently stored
    int capacity;       // Allocated capacity of points array
    Tool tool;          // Tool used for this entry
    char *text_data;    // Text content for TOOL_TEXT (NULL for other tools)
} HistoryEntry;

// Represents the history of drawing actions
typedef struct {
    HistoryEntry *entries; // Dynamic array of entries
    int count;             // Number of entries currently stored
    int capacity;          // Allocated capacity of entries array
} History;

/**
 * Initializes a History structure.
 *
 * @param history Pointer to the History structure to initialize.
 */
void init_history(History *history);

/**
 * Frees all memory used by the history.
 *
 * @param history Pointer to the History structure to free.
 */
void free_history(History *history);

/**
 * Pushes a new history entry onto the history stack.
 *
 * @param history Pointer to the History structure.
 * @param entry   The HistoryEntry to add.
 */
void push_history(History *history, HistoryEntry entry);

/**
 * Pops the most recent history entry from the stack.
 * If history is empty, the returned HistoryEntry may have count == 0.
 *
 * @param history Pointer to the History structure.
 * @return The last HistoryEntry.
 */
HistoryEntry pop_history(History *history);

/**
 * Checks if the history is empty.
 *
 * @param history Pointer to the History structure.
 * @return true if empty, false otherwise.
 */
bool is_history_empty(const History *history);

/**
 * Adds a point to a given HistoryEntry (auto-expands if needed).
 *
 * @param entry Pointer to the HistoryEntry.
 * @param x     X coordinate of the point.
 * @param y     Y coordinate of the point.
 * @return true on success, false on failure (e.g., memory allocation failure).
 */
bool add_point_to_entry(HistoryEntry *entry, int x, int y);

#endif // HISTORY_H
