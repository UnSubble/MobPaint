#include "app.h"
#include "context/logs.h"
#include "context/paint_context.h"
#include "tools/tools.h"
#include "sidebar.h"
#include "assets.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

Assets *global_assets = NULL;

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

    if (TTF_Init() == -1) {
        log_error("TTF_Init error: %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("assets/OpenSans.ttf", 16);
    if (!font) {
        log_error("Font load error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Color background_color = config->default_background_color;

    SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g,
                                      background_color.b, background_color.a);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_ShowWindow(window);

    global_assets = load_assets(renderer);
    
    PaintContext context;
    Tool current_tool;
    init_tool(&current_tool, config);
    init_paint_context(renderer, &context, config, current_tool);

    bool running = true;
    bool drawing = false;
    bool needs_redraw = true;
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
                        if (in_sidebar_bounds(event.button.x, event.button.y)) {
                            if (event.button.x < SIDEBAR_WIDTH)
                                handle_sidebar_click(&context, event.button.x, event.button.y);
                            if (event.button.y < TOPBAR_HEIGHT)
                                handle_topbar_click(&context, event.button.x, event.button.y);
                        } else {
                            context.text_placed = false;
                            
                            drawing = true;
                            context.mouse_x = event.button.x;
                            context.mouse_y = event.button.y;

                            if (context.current_tool.type == TOOL_LINE || context.current_tool.type == TOOL_CIRCLE) {
                                start_stroke(&context);
                                add_point_to_current_stroke(&context, context.mouse_x, context.mouse_y);
                            } else {
                                start_stroke(&context);
                                use_tool(&context, -1, -1);
                            }

                            log_info("Drawing started at (%d, %d).", context.mouse_x, context.mouse_y);
                        }
                        needs_redraw = true;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT && drawing) {
                        drawing = false;
                        int prev_x = -1, prev_y = -1;

                        if (context.current_tool.type == TOOL_LINE || context.current_tool.type == TOOL_CIRCLE) {
                            prev_x = context.current_stroke->points[0].x;
                            prev_y = context.current_stroke->points[0].y;

                            context.mouse_x = event.button.x;
                            context.mouse_y = event.button.y;

                            use_tool(&context, prev_x, prev_y);
                        } else if (context.current_tool.type != TOOL_FILL && context.current_tool.type != TOOL_TEXT) {
                            prev_x = context.mouse_x;
                            prev_y = context.mouse_y;
                            context.mouse_x = event.button.x;
                            context.mouse_y = event.button.y;
                            use_tool(&context, prev_x, prev_y);
                        }

                        context.mouse_x = -1;
                        context.mouse_y = -1;
                        
                        if (context.current_tool.type != TOOL_TEXT || !context.text_input_active) {
                            end_stroke(&context);
                        }
                        
                        needs_redraw = true;
                        log_info("Drawing stopped.");
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (drawing && (event.motion.state & SDL_BUTTON_LMASK)) {
                        int prev_x = context.mouse_x;
                        int prev_y = context.mouse_y;
                        context.mouse_x = event.motion.x;
                        context.mouse_y = event.motion.y;

                        if (context.current_tool.type != TOOL_LINE && context.current_tool.type != TOOL_CIRCLE 
                                    && context.current_tool.type != TOOL_FILL && context.current_tool.type != TOOL_TEXT) {
                            use_tool(&context, prev_x, prev_y);
                            needs_redraw = true;
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    if (context.text_input_active) {
                        if (!handle_text_key(&context, event.key.keysym.sym)) {
                            finalize_text_input(&context);
                            end_stroke(&context);
                            redraw_canvas(&context);
                        } 
                        needs_redraw = true;
                    } else {
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            log_info("ESC pressed. Exiting.");
                            running = false;
                        } else if (event.key.keysym.sym == SDLK_c && (event.key.keysym.mod & KMOD_CTRL)) {
                            SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g,
                                                              background_color.b, background_color.a);
                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, context.bitmap_cache, NULL, NULL);
                            needs_redraw = true;
                            log_info("Canvas cleared.");
                        } else if (event.key.keysym.sym == SDLK_z && (event.key.keysym.mod & KMOD_CTRL)) {
                            bool changed = false;
                            if (event.key.keysym.mod & KMOD_SHIFT) {
                                changed = paint_context_redo(&context);
                            } else {
                                changed = paint_context_undo(&context);
                            }
                            if (changed) {
                                redraw_canvas(&context);
                                needs_redraw = true;
                            }
                        } else if (event.key.keysym.sym == SDLK_i && (event.key.keysym.mod & KMOD_CTRL)) {
                            if (context.current_tool.size < 50)
                                ++context.current_tool.size;
                            needs_redraw = true;
                        } else if (event.key.keysym.sym == SDLK_o && (event.key.keysym.mod & KMOD_CTRL)) {
                            if (context.current_tool.size > 1)
                                --context.current_tool.size;
                            needs_redraw = true;
                        } else if (event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym < SDLK_1 + TOOL_COUNT) {
                            ToolType tool_type = event.key.keysym.sym - SDLK_1;
                            set_tool_type(&context.current_tool, tool_type);
                            log_info("Tool switched to %s.", get_tool_name(&context.current_tool));
                            needs_redraw = true;
                        }
                    }
                    break;

                case SDL_TEXTINPUT:
                    if (context.text_input_active) {
                        handle_text_input(&context, event.text.text);
                        needs_redraw = true;
                    }
                    break;

                default:
                    break;
            }
        }

        if (needs_redraw) {
            if (context.text_input_active) {
                redraw_canvas(&context);
            }
            
            draw_topbar(renderer, &context, config, font);
            draw_left_sidebar(renderer, &context, config);
            
            if (context.text_input_active) {
                TTF_Font *preview_font = TTF_OpenFont("assets/OpenSans.ttf", context.current_tool.size + 12);
                if (preview_font && strlen(context.text_input_buffer) > 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
                    
                    int text_w, text_h;
                    TTF_SizeText(preview_font, context.text_input_buffer, &text_w, &text_h);
                    
                    SDL_Rect bg_rect = {
                        context.text_input_x - 2,
                        context.text_input_y - 2,
                        text_w + 4,
                        text_h + 4
                    };
                    SDL_RenderFillRect(renderer, &bg_rect);
                    
                    render_text(renderer, preview_font, context.text_input_buffer, 
                               context.text_input_x, context.text_input_y, context.current_tool.color);
                }
                
                int text_w = 0;
                if (preview_font && strlen(context.text_input_buffer) > 0) {
                    TTF_SizeText(preview_font, context.text_input_buffer, &text_w, NULL);
                }
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                int cursor_height;
                if (preview_font) {
                    TTF_SizeText(preview_font, "I", NULL, &cursor_height);
                } else {
                    cursor_height = 16;
                }
                if (preview_font) {
                    TTF_CloseFont(preview_font);
                }
                SDL_RenderDrawLine(renderer, 
                                  context.text_input_x + text_w, context.text_input_y,
                                  context.text_input_x + text_w, context.text_input_y + cursor_height);
            }
            
            SDL_RenderPresent(renderer);
            needs_redraw = false;
        }

        SDL_Delay(1);
    }

    free_paint_context(&context);
    free_assets(global_assets);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    log_info("App exited cleanly.");
    return 0;
}
