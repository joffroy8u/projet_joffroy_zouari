#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <stdlib.h>
#include <stdbool.h>

#include "vector2.h"


struct graph_s {
    
    vector2_t** vertices;
    int** costs;
};
typedef struct graph_s graph_t;

graph_t* load_graph(char* map_file);

#endif