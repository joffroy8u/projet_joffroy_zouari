#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "obstacle.h"

obstacle_t* init_obstacle(vector2_t* position, sprite_t* sprite, int vertex_count){

    obstacle_t* obstacle = (obstacle_t*)malloc(sizeof(obstacle_t));
    obstacle->position = position;
    obstacle->sprite = sprite;
    obstacle->next_vertex = 34;

    return obstacle;
}

void move_towards_next_vertex(road_vertex_t** roads, obstacle_t* obstacle, float dt){

    vector2_t* next_position = roads[obstacle->next_vertex]->position;
    float move_x = (next_position->x - obstacle->position->x) / fabs(next_position->x - obstacle->position->x) * SPEED * dt;
    float move_y = (next_position->y - obstacle->position->y) / fabs(next_position->y - obstacle->position->y) * SPEED * dt;

    obstacle->position->x += move_x;
    obstacle->position->y += move_y;

    if(reached_next_vertex(roads, obstacle)){
        obstacle->next_vertex = roads[obstacle->next_vertex]->edges[0];
        //printf("Next vertex = %d, %f,%f\n", obstacle->next_vertex, roads[obstacle->next_vertex]->position->x, roads[obstacle->next_vertex]->position->y);
    }
}

bool reached_next_vertex(road_vertex_t** roads, obstacle_t* obstacle){

    vector2_t* next_position = roads[obstacle->next_vertex]->position;
    return distance(obstacle->position, next_position) < 1;
}

void free_obstacle(obstacle_t* obstacle){

    free_sprite(obstacle->sprite);
    free_vector2(obstacle->position);
    free(obstacle);
}