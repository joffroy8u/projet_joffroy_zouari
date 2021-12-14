#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>

#include "SDL_utils.h"
#include "obstacle.h"

obstacle_t* init_obstacle(vector2_t* position, char** sprite_name, int sprite_count, int vertex_count){

    obstacle_t* obstacle = (obstacle_t*)malloc(sizeof(obstacle_t));
    obstacle->position = position;
    obstacle->direction = 0;
    obstacle->length = 2.5;
    obstacle->width = 1.5;
    obstacle->textures = (uint32_t**)malloc(sizeof(uint32_t*) * sprite_count);
    for(int i = 0; i < sprite_count; i++){
        obstacle->textures[i] = load_obstacle_texture(sprite_name[i]);
    }
    obstacle->sprite_count = sprite_count;
    obstacle->next_vertex = 34;

    srand(time(NULL));

    return obstacle;
}

uint32_t* load_obstacle_texture(char* file_name){

    SDL_Surface* surface = load_png_surface(file_name);
    
    int tex_w = surface->w;
    int tex_h = surface->h;

    uint32_t* img = (uint32_t*)malloc(sizeof(uint32_t) * tex_w * tex_h);
    
    for(int i = 0; i < tex_w; i++){
        for(int j = 0; j < tex_h; j++){
           
            uint8_t r = ((uint8_t*)(surface->pixels))[(i+j*tex_w)*4+3];
            uint8_t g = ((uint8_t*)(surface->pixels))[(i+j*tex_w)*4+2];
            uint8_t b = ((uint8_t*)(surface->pixels))[(i+j*tex_w)*4+1];
            uint8_t a = ((uint8_t*)(surface->pixels))[(i+j*tex_w)*4+0];
            
            img[i+j*tex_w] = convert_color(r, g, b, a);
            
        }
    }
    
    SDL_FreeSurface(surface);

    return img;
}

void move_towards_next_vertex(road_vertex_t** roads, obstacle_t* obstacle, float dt){

    vector2_t* next_position = roads[obstacle->next_vertex]->position;
    float move_x = (next_position->x - obstacle->position->x) / fabs(next_position->x - obstacle->position->x) * SPEED * dt;
    float move_y = (next_position->y - obstacle->position->y) / fabs(next_position->y - obstacle->position->y) * SPEED * dt;

    obstacle->position->x += move_x;
    obstacle->position->y += move_y;
    obstacle->direction = atan2(obstacle->position->y - next_position->y, obstacle->position->x - next_position->x);
    
    if(reached_next_vertex(roads, obstacle)){
        int r = rand() % 2;
        
        if(roads[obstacle->next_vertex]->edges[1] == -1)
            r = 0;

        obstacle->next_vertex = roads[obstacle->next_vertex]->edges[0];
        //printf("Next vertex = %d, %f,%f\n", obstacle->next_vertex, roads[obstacle->next_vertex]->position->x, roads[obstacle->next_vertex]->position->y);
    }
}

bool reached_next_vertex(road_vertex_t** roads, obstacle_t* obstacle){

    vector2_t* next_position = roads[obstacle->next_vertex]->position;
    return distance(obstacle->position, next_position) < 0.1;
}

void free_obstacle(obstacle_t* obstacle){

    for(int i = 0; i < obstacle->sprite_count; i++){
        free(obstacle->textures[0]);
    }
    free(obstacle->textures);
    free_vector2(obstacle->position);
    free(obstacle);
}