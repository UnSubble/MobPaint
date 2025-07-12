#include "app.h"
#include "logs.h"
#include "tools/tools.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

int run_app(const char *target_file_path, Config* config) {
    log_info("Running app with target file: %s", target_file_path);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_error("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    int window_width = config->window_width;
    int window_height = config->window_height;

    SDL_Window *window = SDL_CreateWindow("MobPaint",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height, SDL_WINDOW_ALLOW_HIGHDPI);

    if (!window) {
        log_error("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        log_error("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Color background_color = config->default_background_color;

    SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, 
                                    background_color.b, background_color.a);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_ShowWindow(window);

    Tool current_tool;
    init_tool(&current_tool, config);

    bool running = true;
    bool drawing = false;
    bool needs_redraw = false;
    int prev_x = -1, prev_y = -1;  // Track previous position for smooth drawing
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    log_info("Window close event received.");
                    running = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        drawing = true;
                        prev_x = event.button.x;
                        prev_y = event.button.y;

                        use_tool(renderer, &current_tool, prev_x, prev_y, -1, -1);
                        needs_redraw = true;

                        log_info("Drawing started at (%d, %d).", prev_x, prev_y);
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        drawing = false;
                        prev_x = -1;
                        prev_y = -1;
                        log_info("Drawing stopped.");
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (drawing && (event.motion.state & SDL_BUTTON_LMASK)) {
                        int curr_x = event.motion.x;
                        int curr_y = event.motion.y;

                        use_tool(renderer, &current_tool, curr_x, curr_y, prev_x, prev_y);
                        needs_redraw = true;

                        prev_x = curr_x;
                        prev_y = curr_y;
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        log_info("ESC pressed. Exiting.");
                        running = false;
                    } else if (event.key.keysym.sym == SDLK_c && (event.key.keysym.mod & KMOD_CTRL)) {
                        // Clear canvas with Ctrl+C
                        SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, 
                                                        background_color.b, background_color.a);
                        SDL_RenderClear(renderer);
                        needs_redraw = true;
                        log_info("Canvas cleared.");
                    } else if (event.key.keysym.sym == SDLK_1) {
                        set_tool_type(&current_tool, TOOL_BRUSH);
                        log_info("Tool switched to BRUSH.");
                    } else if (event.key.keysym.sym == SDLK_2) {
                        set_tool_type(&current_tool, TOOL_ERASER);
                        log_info("Tool switched to ERASER.");
                    }
                    break;

                default:
                    break;
            }
        }

        if (needs_redraw) {
            SDL_RenderPresent(renderer);
            needs_redraw = false;
        }

        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    log_info("App exited cleanly.");
    return 0;
}