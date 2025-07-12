#include "context/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

static void set_default_config(Config *config) {
    strncpy(config->log_dir, "logs", sizeof(config->log_dir));
    config->window_width = 800;
    config->window_height = 600;
    strncpy(config->default_tool, "brush", sizeof(config->default_tool));
    config->brush_size = 4;
    config->brush_color.r = 0;
    config->brush_color.g = 0;
    config->brush_color.b = 0;
    config->brush_color.a = 255;
}

bool load_config(const char *filename, Config *config) {
    set_default_config(config);

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Config file '%s' not found, using defaults.\n", filename);
        return false;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(filesize + 1);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for config file.\n");
        fclose(file);
        return false;
    }

    fread(buffer, 1, filesize, file);
    buffer[filesize] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (!json) {
        fprintf(stderr, "Failed to parse JSON config: %s\n", cJSON_GetErrorPtr());
        return false;
    }

    cJSON *log_dir = cJSON_GetObjectItemCaseSensitive(json, "log_dir");
    if (cJSON_IsString(log_dir) && (log_dir->valuestring != NULL)) {
        strncpy(config->log_dir, log_dir->valuestring, sizeof(config->log_dir));
    }

    cJSON *target_path = cJSON_GetObjectItemCaseSensitive(json, "default_target_path");
    if (cJSON_IsString(target_path) && (target_path->valuestring != NULL)) {
        strncpy(config->default_target_path, target_path->valuestring, sizeof(config->default_target_path));
    }

    cJSON *window = cJSON_GetObjectItemCaseSensitive(json, "window");
    if (cJSON_IsObject(window)) {
        cJSON *width = cJSON_GetObjectItemCaseSensitive(window, "width");
        cJSON *height = cJSON_GetObjectItemCaseSensitive(window, "height");
        cJSON *default_background = cJSON_GetObjectItemCaseSensitive(window, "default_background");
        if (cJSON_IsArray(default_background) && cJSON_GetArraySize(default_background) == 4) {
            cJSON *r = cJSON_GetArrayItem(default_background, 0);
            cJSON *g = cJSON_GetArrayItem(default_background, 1);
            cJSON *b = cJSON_GetArrayItem(default_background, 2);
            cJSON *a = cJSON_GetArrayItem(default_background, 3);
            if (cJSON_IsNumber(r) && cJSON_IsNumber(g) && cJSON_IsNumber(b) && cJSON_IsNumber(a)) {
                config->default_background_color.r = (Uint8)r->valueint;
                config->default_background_color.g = (Uint8)g->valueint;
                config->default_background_color.b = (Uint8)b->valueint;
                config->default_background_color.a = (Uint8)a->valueint;
            }
        }
        if (cJSON_IsNumber(width)) config->window_width = width->valueint;
        if (cJSON_IsNumber(height)) config->window_height = height->valueint;
    }

    cJSON *default_tool = cJSON_GetObjectItemCaseSensitive(json, "default_tool");
    if (cJSON_IsString(default_tool) && (default_tool->valuestring != NULL)) {
        strncpy(config->default_tool, default_tool->valuestring, sizeof(config->default_tool));
    }

    cJSON *brush = cJSON_GetObjectItemCaseSensitive(json, "brush");
    if (cJSON_IsObject(brush)) {
        cJSON *size = cJSON_GetObjectItemCaseSensitive(brush, "size");
        if (cJSON_IsNumber(size)) config->brush_size = size->valueint;

        cJSON *color = cJSON_GetObjectItemCaseSensitive(brush, "color");
        if (cJSON_IsArray(color) && cJSON_GetArraySize(color) == 4) {
            cJSON *r = cJSON_GetArrayItem(color, 0);
            cJSON *g = cJSON_GetArrayItem(color, 1);
            cJSON *b = cJSON_GetArrayItem(color, 2);
            cJSON *a = cJSON_GetArrayItem(color, 3);
            if (cJSON_IsNumber(r) && cJSON_IsNumber(g) && cJSON_IsNumber(b) && cJSON_IsNumber(a)) {
                config->brush_color.r = (Uint8)r->valueint;
                config->brush_color.g = (Uint8)g->valueint;
                config->brush_color.b = (Uint8)b->valueint;
                config->brush_color.a = (Uint8)a->valueint;
            }
        }
    }

    cJSON_Delete(json);
    return true;
}
