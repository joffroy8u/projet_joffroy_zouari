#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

#include "SDL_utils.h"

SDL_Texture* load_text(const char* message, SDL_Renderer* renderer, TTF_Font *font, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);

    return SDL_CreateTextureFromSurface(renderer, surface);
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