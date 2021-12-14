#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "roads.h"

road_vertex_t* init_road_tile(vector2_t* position, int edges[2], int bitmask){

    road_vertex_t* road_tile = (road_vertex_t*)malloc(sizeof(road_vertex_t));
    road_tile->position = position;
    road_tile->edges[0] = edges[0];
    road_tile->edges[1] = edges[1];
    road_tile->bitmask = bitmask;
    return road_tile;
}

road_vertex_t** build_roads(char* map, int map_size, float* finish_x, float* finish_y){
    
    int n = map_size * map_size;
    road_vertex_t** vertices = (road_vertex_t**)malloc(sizeof(road_vertex_t*) * n);
    for(int i = 0; i < n; i++){
        vector2_t* position = init_vector2((i % map_size) * 4 + 2, (i / map_size) * 4 + 2);
        int edges[2] = {-1, -1};
        if(map[i] != ' ' && map[i] != '='){ 
            vertices[i] = init_road_tile(position, edges, -1);
            continue;
        }
        
        if(map[i] == '='){
            *finish_x = position->x;
            *finish_y = position->y;
        }

        int bitmask = get_bitmask(map, map_size, i);
        int neighbor = -1;
        // Arete vers le Nord
        if(bitmask == 2 || bitmask == 6){
            if(get_neighbor(map_size, i, N, &neighbor)) edges[0] = neighbor;
        }
        // Arete vers le Sud
        else if(bitmask == 8 || bitmask == 9){
            if(get_neighbor(map_size, i, S, &neighbor)) edges[0] = neighbor;
        }
        // Arete vers l'Est
        else if(bitmask == 4 || bitmask == 12){
            if(get_neighbor(map_size, i, E, &neighbor)) edges[0] = neighbor;
        }
        // Arete vers l'Ouest
        else if(bitmask == 1 || bitmask == 3){
            if(get_neighbor(map_size, i, W, &neighbor)) edges[0] = neighbor;
        }
        // Pas de mur adjacent
        else if(bitmask == 0){
            int diag_bitmask = get_diag_bitmask(map, map_size, i);
            if(diag_bitmask == 1){
                if(get_neighbor(map_size, i, W, &neighbor)) edges[0] = neighbor;
                if(get_neighbor(map_size, i, S, &neighbor)) edges[1] = neighbor;
            }
            else if(diag_bitmask == 2){
                if(get_neighbor(map_size, i, N, &neighbor)) edges[0] = neighbor;
                if(get_neighbor(map_size, i, W, &neighbor)) edges[1] = neighbor;
            }
            else if(diag_bitmask == 4){
                if(get_neighbor(map_size, i, E, &neighbor)) edges[0] = neighbor;
                if(get_neighbor(map_size, i, N, &neighbor)) edges[1] = neighbor;
            }
            else if(diag_bitmask == 8){
                if(get_neighbor(map_size, i, S, &neighbor)) edges[0] = neighbor;
                if(get_neighbor(map_size, i, E, &neighbor)) edges[1] = neighbor;
            }
            else{
                printf("Erreur\n");
            }
        }
        else{
            printf("Erreur\n");
        }
        
        vertices[i] = init_road_tile(position, edges, bitmask);
    }
    return vertices;
}

int get_bitmask(char* map, int map_size, int index){

    int bitmask = 0;
    int n;
    if(get_neighbor(map_size, index, N, &n)){
        if(map[n] >= '0' && map[n] <= '9'){
            bitmask += 1;
        }
    }
    if(get_neighbor(map_size, index, E, &n)){
        if(map[n] >= '0' && map[n] <= '9'){
            bitmask += 2;
        }
    }
    if(get_neighbor(map_size, index, S, &n)){
        if(map[n] >= '0' && map[n] <= '9'){
            bitmask += 4;
        }
    }
    if(get_neighbor(map_size, index, W, &n)){
        if(map[n] >= '0' && map[n] <= '9'){
            bitmask += 8;
        }
    }
    return bitmask;
}

int get_diag_bitmask(char* map, int map_size, int index){

    int diag_bitmask = 0;
    int n;
    if(get_neighbor(map_size, index, NW, &n)){
        if(map[n] >= '0' && map[n] <= '9'){
            diag_bitmask += 1;
        }
    }
    if(get_neighbor(map_size, index, NE, &n)){
        if(map[n] >= '0' && map[n] <= '9'){
            diag_bitmask += 2;
        }
    }
    if(get_neighbor(map_size, index, SE, &n)){
        if(map[n] >= '0' && map[n] <= '9'){
            diag_bitmask += 4;
        }
    }
    if(get_neighbor(map_size, index, SW, &n)){
        if(map[n] >= '0' && map[n] <= '9'){
            diag_bitmask += 8;
        }
    }
    return diag_bitmask;
}

bool get_neighbor(int map_size, int index, direction_t dir, int* neighbor){

    switch(dir){
        case N:
            *neighbor = index - map_size;
            return *neighbor >= 0;
        case S:
            *neighbor = index + map_size;
            return *neighbor < map_size * map_size;
        case E:
            *neighbor = index + 1;
            return *neighbor / map_size == index / map_size;
        case W:
            *neighbor = index - 1;
            return *neighbor / map_size == index / map_size && *neighbor >= 0;
        case NW:
            *neighbor = index - map_size - 1;
            return *neighbor >= 0;
        case NE:
            *neighbor = index - map_size + 1;
            return *neighbor >= 0;
        case SE:
            *neighbor = index + map_size + 1;
            return *neighbor < map_size * map_size;
        case SW:
            *neighbor = index + map_size - 1;
            return *neighbor < map_size * map_size;
    }
}

void render_roads_map(uint32_t* texture, int width, int height, road_vertex_t** roads, int map_size){

    int n = map_size * map_size;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < 2; j++){
            if(roads[i]->bitmask == -1 || roads[i]->edges[j] == -1)
                continue;
            else{
                draw_edge(texture, width, roads[i]->position, roads[roads[i]->edges[j]]->position, map_size);
                draw_vertex(texture, width, roads[i]->position, map_size);
            }
        }
    }
}

void draw_edge(uint32_t* texture, int width, vector2_t* a, vector2_t* b, int map_size){

    int start_x = fmin(a->x, b->x) * width / (4 * map_size);
    int end_x = fmax(a->x, b->x) * width / (4 * map_size);
    int start_y = fmin(a->y, b->y) * width / (4 * map_size);
    int end_y = fmax(a->y, b->y) * width / (4 * map_size);

    if(start_x == end_x){
        for(int y = start_y; y < end_y; y++){
            texture[start_x + y * width] = 0x00000000;
        }
    }else{
        for(int x = start_x; x < end_x; x++){
            texture[x + start_y * width] = 0x00000000;
        }
    }
}

void draw_vertex(uint32_t* texture, int width, vector2_t* a, int map_size){

    int cx = a->x * width / (4 * map_size);
    int cy = a->y * width / (4 * map_size);

    for(int x = cx-2; x <= cx+2; x++){
        for(int y = cy-2; y <= cy+2; y++){
            if(x < 0 || y < 0 || x >= width || y >= width) continue;
            texture[x + y * width] = 0x00000000;
        }
    }
}

void draw_wall(uint32_t* texture, int width, vector2_t* a, vector2_t* b, int map_size){

    int start_x = fmin(a->x, b->x) * width / (4 * map_size);
    int end_x = fmax(a->x, b->x) * width / (4 * map_size);
    int start_y = fmin(a->y, b->y) * width / (4 * map_size);
    int end_y = fmax(a->y, b->y) * width / (4 * map_size);
    //printf("%d,%d,%d,%d\n", start_x, end_x, start_y, end_y);

    if(start_x == end_x){
        for(int x = start_x - 4; x <= start_x + 4; x++){
            for(int y = start_y; y <= end_y; y++){
                if(x < 0 || x >= width * width || y < 0 || y >= width * width) continue;
                texture[x + y * width] = 0xff000000;
            }
        }
        
    }else{
        for(int x = start_x; x <= end_x; x++){
            for(int y = start_y - 4; y <= end_y + 4; y++){
                if(x < 0 || x >= width * width || y < 0 || y >= width * width) continue;
                texture[x + y * width] = 0xff000000;
            }
        }
    }
}

void free_roads(road_vertex_t** roads, int map_size){

    int n = map_size * map_size;
    for(int i = 0; i < n; i++){
        free_vector2(roads[i]->position);
        free(roads[i]);
    }
    free(roads);
}