#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#include "SDL_utils.h"

SDL_Texture* load_text(const char* message, SDL_Renderer* renderer, TTF_Font *font, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* load_png(const char* nomfichier, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(nomfichier);
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surface);

    if(formattedSurface == NULL)
    {
        printf( "Erreur lors du chargement de l'image %s! Erreur: %s\n", nomfichier, IMG_GetError());
    }
  
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, formattedSurface);
    SDL_FreeSurface(formattedSurface);
    return texture;
}

SDL_Surface* load_png_surface(char* file_name)
{
    SDL_Surface* surface = IMG_Load(file_name);
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surface);

    if(formattedSurface == NULL)
    {
        printf( "Erreur lors du chargement de l'image %s! Erreur: %s\n", file_name, IMG_GetError());
    }
    return formattedSurface;
}

uint32_t convert_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (r<<24) + (g<<16) + (b<<8) + a;
}

void read_color(uint32_t* color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) {
    *r = (*color >>  0) & 255;
    *g = (*color >>  8) & 255;
    *b = (*color >> 16) & 255;
    *a = (*color >> 24) & 255;
}