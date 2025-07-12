#include "context/history.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 8

void init_history(History *history) {
    history->entries = NULL;
    history->count = 0;
    history->capacity = 0;
}

void free_history(History *history) {
    if (!history) 
        return;
    for (int i = 0; i < history->count; i++) {
        free(history->entries[i].points);
    }
    free(history->entries);
    history->entries = NULL;
    history->count = 0;
    history->capacity = 0;
}

void ensure_history_capacity(History *history) {
    if (history->count >= history->capacity) {
        int new_capacity = history->capacity ? history->capacity * 2 : INITIAL_CAPACITY;
        history->entries = realloc(history->entries, new_capacity * sizeof(HistoryEntry));
        history->capacity = new_capacity;
    }
}

void ensure_entry_capacity(HistoryEntry *entry) {
    if (entry->count >= entry->capacity) {
        int new_capacity = entry->capacity ? entry->capacity * 2 : INITIAL_CAPACITY;
        entry->points = realloc(entry->points, new_capacity * sizeof(Point));
        entry->capacity = new_capacity;
    }
}

void push_history(History *history, HistoryEntry entry) {
    ensure_history_capacity(history);

    HistoryEntry *target = &history->entries[history->count];

    target->tool = entry.tool;
    target->count = entry.count;
    target->capacity = entry.capacity;

    if (entry.count > 0) {
        target->points = malloc(entry.capacity * sizeof(Point));
        if (!target->points) {
            return;
        }
        memcpy(target->points, entry.points, entry.count * sizeof(Point));
    } else {
        target->points = NULL;
    }

    history->count++;
}

HistoryEntry pop_history(History *history) {
    if (history->count == 0) {
        HistoryEntry empty = {0};
        return empty;
    }
    HistoryEntry entry = history->entries[--history->count];
    return entry;
}

bool is_history_empty(const History *history) {
    return history->count == 0;
}

bool add_point_to_entry(HistoryEntry *entry, int x, int y) {
    if (!entry) 
        return false;
    ensure_entry_capacity(entry);
    entry->points[entry->count].x = x;
    entry->points[entry->count].y = y;
    entry->count++;
    return true;
}
