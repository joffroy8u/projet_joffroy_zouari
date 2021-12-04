#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "traffic.h"

graph_t* load_graph(char* map_file){

    int vertex_count = get_vertex_count(map_file);
    
    vector2_t* vertices = (vector2_t*)malloc(sizeof(vector2_t) * vertex_count);

    FILE* file;
    
    map_file = fopen(map_file, "r");
    if(map_file == NULL){
        
        perror ("Error opening file");
    }
    char str_map_size[2];
    fgets(str_map_size, 2, file);
    int map_size;
    sscanf(str_map_size, "%d", &map_size);

    int n = 0;
    char map[map_size];
    for(int i = 0; i < map_size+1; i++){
        fgets(map, map_size, file);
        for(int j = 0; j < map_size; j++){
            char c = map[j];
            if(c == 'x'){
                
            }
        }
    }
    
    

    free(vertices);


}

int get_vertex_count(char* map_file){

    FILE* file;
    
    map_file = fopen(map_file, "r");
    if(map_file == NULL){
        
        perror ("Error opening file");
    }

    char str_map_size[2];
    fgets(str_map_size, 2, file);
    int map_size;
    sscanf(str_map_size, "%d", &map_size);

    char map[map_size];
    for(int i = 0; i < map_size+1; i++){
        fgets(map, map_size, file);
    }
    
    char str_vertex_count[2];
    fgets(str_vertex_count, 2, file);
    int vertex_count;
    sscanf(str_vertex_count, "%d", &vertex_count);

    return vertex_count;
}