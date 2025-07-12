#ifndef HISTORY_H
#define HISTORY_H

#include <SDL2/SDL.h>
#include "tools/tools.h"

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point *points;
    int count;
    int capacity;
    Tool tool;
} HistoryEntry;

typedef struct {
    HistoryEntry *entries;
    int count;
    int capacity;
} History;

void init_history(History *history);
void free_history(History *history);

void push_history(History *history, HistoryEntry entry); 
HistoryEntry pop_history(History *history); 
int is_history_empty(History *history);

bool add_point_to_entry(HistoryEntry *entry, int x, int y);

#endif // HISTORY_H
