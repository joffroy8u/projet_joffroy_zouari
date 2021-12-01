#include <stdlib.h>
#include <stdbool.h>

#include "traffic.h"

graph_t* load_graph(char* map_file){

    FILE* map_file;
    
    map_file = fopen(nomFichier, "r");
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

        

}