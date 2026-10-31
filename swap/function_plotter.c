#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 600
#define MIN_SPAN 0.001
#define PI 3.14159265358979323846

typedef enum { FUNC_SIN, FUNC_COS, FUNC_TAN, FUNC_EXP, FUNC_LOG } FuncType;

static const char *func_name(FuncType type) {
    static const char *names[] = { "sin(x)", "cos(x)", "tan(x)",
                                  "exp(x/3)-1", "log(x)" };
    return names[type];
}

static double evaluate(FuncType type, double x) {
    switch (type) {
        case FUNC_SIN: return sin(x);
        case FUNC_COS: return cos(x);
        case FUNC_TAN: return tan(x);
        case FUNC_EXP: return exp(x / 3.0) - 1.0;
        case FUNC_LOG: return x > 0.0 ? log(x) : NAN;
    }
    return NAN;
}

static int map_x(double x, double min, double max, int width) {
    return (int)lround((x - min) * (double)width / (max - min));
}

static int map_y(double y, double min, double max, int height) {
    return (int)lround(height - (y - min) * (double)height / (max - min));
}

static void draw_label(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                       int x, int y, SDL_Color color, SDL_Color shadow) {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, shadow);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect destination = {x + 2, y + 2, surface->w, surface->h};
    SDL_FreeSurface(surface);
    if (!texture) return;
    SDL_RenderCopy(renderer, texture, NULL, &destination);
    SDL_DestroyTexture(texture);

    surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) return;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    destination = (SDL_Rect){x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    if (!texture) return;
    SDL_RenderCopy(renderer, texture, NULL, &destination);
    SDL_DestroyTexture(texture);
}

static void zoom(double *min, double *max, double factor) {
    double centre = (*min + *max) / 2.0;
    double half_span = fmax((*max - *min) * factor / 2.0, MIN_SPAN / 2.0);
    *min = centre - half_span;
    *max = centre + half_span;
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        fprintf(stderr, "SDL_ttf initialization error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Funciones graficas © arauxodev 2026 ", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = window ? SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) : NULL;
    if (!window || !renderer) {
        fprintf(stderr, "Window/renderer error: %s\n", SDL_GetError());
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        TTF_Quit(); SDL_Quit();
        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 20);
    if (!font) font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 20);
    if (!font) {
        fprintf(stderr, "Font error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); TTF_Quit(); SDL_Quit();
        return 1;
    }

    FuncType current = FUNC_SIN;
    double x_min = -2.0 * PI, x_max = 2.0 * PI, y_min = -2.5, y_max = 2.5;
    double target_x_min = x_min, target_x_max = x_max, target_y_min = y_min, target_y_max = y_max;
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type != SDL_KEYDOWN) continue;
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: running = false; break;
                case SDLK_1: current = FUNC_SIN; break; case SDLK_2: current = FUNC_COS; break;
                case SDLK_3: current = FUNC_TAN; break; case SDLK_4: current = FUNC_EXP; break;
                case SDLK_5: current = FUNC_LOG; break;
                case SDLK_MINUS: zoom(&target_x_min, &target_x_max, 1.25); zoom(&target_y_min, &target_y_max, 1.25); break;
                case SDLK_COMMA: zoom(&target_x_min, &target_x_max, 0.8); zoom(&target_y_min, &target_y_max, 0.8); break;
                case SDLK_LEFT: target_x_min -= 0.5; target_x_max -= 0.5; break;
                case SDLK_RIGHT: target_x_min += 0.5; target_x_max += 0.5; break;
                case SDLK_UP: target_y_min += 0.5; target_y_max += 0.5; break;
                case SDLK_DOWN: target_y_min -= 0.5; target_y_max -= 0.5; break;
            }
        }
        x_min += (target_x_min - x_min) * 0.15; x_max += (target_x_max - x_max) * 0.15;
        y_min += (target_y_min - y_min) * 0.15; y_max += (target_y_max - y_max) * 0.15;

        int width, height;
        SDL_GetRendererOutputSize(renderer, &width, &height);
        SDL_SetRenderDrawColor(renderer, 10, 20, 40, 255); SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        int zero_x = map_x(0, x_min, x_max, width), zero_y = map_y(0, y_min, y_max, height);
        if (zero_y >= 0 && zero_y < height) SDL_RenderDrawLine(renderer, 0, zero_y, width, zero_y);
        if (zero_x >= 0 && zero_x < width) SDL_RenderDrawLine(renderer, zero_x, 0, zero_x, height);

        SDL_SetRenderDrawColor(renderer, 0, 255, 220, 255);
        const double step = (x_max - x_min) / (double)width;
        double previous_y = NAN;
        int previous_x_pixel = 0, previous_y_pixel = 0;
        for (int pixel = 0; pixel <= width; ++pixel) {
            double x = x_min + pixel * step, y = evaluate(current, x);
            int y_pixel = map_y(y, y_min, y_max, height);
            /* A large screen jump means an asymptote or an off-screen discontinuity. */
            if (isfinite(y) && isfinite(previous_y) && abs(y_pixel - previous_y_pixel) < height) {
                SDL_RenderDrawLine(renderer, previous_x_pixel, previous_y_pixel, pixel, y_pixel);
            }
            previous_y = y; previous_x_pixel = pixel; previous_y_pixel = y_pixel;
        }

        SDL_Rect panel = {10, 10, 315, 210};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 140); SDL_RenderFillRect(renderer, &panel);
        SDL_Color turquoise = {64, 224, 208, 255}, shadow = {0, 0, 0, 180};
        char buffer[128];
        snprintf(buffer, sizeof buffer, "Funcion: %s", func_name(current)); draw_label(renderer, font, buffer, 20, 20, turquoise, shadow);
        snprintf(buffer, sizeof buffer, "X: %.2f to %.2f", x_min, x_max); draw_label(renderer, font, buffer, 20, 50, turquoise, shadow);
        snprintf(buffer, sizeof buffer, "Y: %.2f to %.2f", y_min, y_max); draw_label(renderer, font, buffer, 20, 80, turquoise, shadow);
        draw_label(renderer, font, "1-5 teclas | , zoom in | - zoom out", 20, 110, turquoise, shadow);
        draw_label(renderer, font, "Cursores : Desliza | Esc: SALIR", 20, 140, turquoise, shadow);
        draw_label(renderer, font, "1 sin, 2 cos, 3 tan, 4 exp, 5 log", 20, 170, turquoise, shadow);
        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font); SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); TTF_Quit(); SDL_Quit();
    return 0;
}
