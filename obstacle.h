#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdbool.h>

#include "sprite.h"
#include "vector2.h"
#include "roads.h"

#define SPEED 1.

typedef struct {

    vector2_t* position;
    sprite_t* sprite;
    int next_vertex;

} obstacle_t;

obstacle_t* init_obstacle(vector2_t* position, sprite_t* sprite);
void move_towards_next_vertex(road_vertex_t** roads, obstacle_t* obstacle);
bool reached_next_vertex(road_vertex_t** roads, obstacle_t* obstacle);
void free_obstacle(obstacle_t* obstacle);

#endif