#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

SDL_Texture* load_text(const char* message, SDL_Renderer* renderer, TTF_Font *font, SDL_Color color);
SDL_Texture* load_png(const char* nomfichier, SDL_Renderer* renderer);

#endif