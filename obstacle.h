#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "sprite.h"
#include "vector2.h"
#include "roads.h"

#define SPEED 2.5

typedef struct {

    vector2_t* position;
    float direction;
    float length;
    float width;
    uint32_t** textures;
    int sprite_count;
    int next_vertex;

} obstacle_t;

obstacle_t* init_obstacle(vector2_t* position, char** sprite_name, int sprite_count, int vertex_count);
uint32_t* load_obstacle_texture(char* file_name);
void move_towards_next_vertex(road_vertex_t** roads, obstacle_t* obstacle, float dt);
bool reached_next_vertex(road_vertex_t** roads, obstacle_t* obstacle);
void free_obstacle(obstacle_t* obstacle);

#endif