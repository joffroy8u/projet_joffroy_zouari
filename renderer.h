#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "player.h"
#include "building.h"
#include "obstacle.h"

uint32_t* init_texture(int width, int height, uint32_t color);
void draw_column(uint32_t* texture, int width, int height, building_t* building, int tex_coord, int x, int min_y, int max_y, int max_y_ground, float current_height);
void clear_texture(uint32_t* texture, int width, int height);

void render(uint32_t* texture, building_t** buildings, obstacle_t** obstacles, char* map, player_t* player, int width, int height, int map_size, int obstacle_count, int render_dst);
void draw_obstacle(uint32_t* texture, int width, int height, float depth_buffer[width], obstacle_t* sprite, player_t* player);

#endif