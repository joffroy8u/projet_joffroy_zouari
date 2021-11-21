#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "player.h"

uint32_t* init_texture(int width, int height, uint32_t color);
uint32_t* draw_column(uint32_t* img, int texsize, int ntextures, int texid, int texcoord, int column_height);
void clear_texture(uint32_t* texture, int width, int height);
uint32_t convert_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint32_t* init_walltexture(char* filename);
SDL_Texture* load_png(const char* nomfichier, SDL_Renderer* renderer);

void render(uint32_t* texture, uint32_t* wall_tex, char* map, player_t* player, int width, int height, int map_size);

#endif