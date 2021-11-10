#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#endif

uint32_t* init_texture(int width, int height, uint32_t color);
uint32_t* draw_column(uint32_t* img, int texsize, int ntextures, int texid, int texcoord, int column_height);
void clear_texture(uint32_t* texture, int width, int height);
uint32_t convert_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint32_t* init_walltexture(char* filename);

void render(uint32_t* texture, uint32_t* wall_tex, char* map, int width, int height, float player_x, float player_y, float player_angle);