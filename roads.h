#ifndef ROAD_H
#define ROAD_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vector2.h"
#include "map.h"

typedef struct {

    vector2_t* position;
    int edges[2];
    int bitmask;

} road_vertex_t;

road_vertex_t* init_road_tile(vector2_t* position, int edges[2], int bitmask);
road_vertex_t** build_roads(char* map, int map_size, float* finish_x, float* finish_y);
int get_bitmask(char* map, int map_size, int index);
int get_diag_bitmask(char* map, int map_size, int index);
bool get_neighbor(int map_size, int index, direction_t dir, int* neighbor);
void render_roads_map(uint32_t* texture, int width, int height, road_vertex_t** roads, int map_size);
void draw_edge(uint32_t* texture, int width, vector2_t* a, vector2_t* b, int map_size);
void draw_vertex(uint32_t* texture, int width, vector2_t* a, int map_size);
void draw_wall(uint32_t* texture, int width, vector2_t* a, vector2_t* b, int map_size);
void free_roads(road_vertex_t** roads, int map_size);

#endif