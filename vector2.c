#include <stdlib.h>

#include "vector2.h"

vector2_t* init_vector2(float x, float y){

    vector2_t* v = (vector2_t*)malloc(sizeof(vector2_t));
    v->x = x;
    v->y = y;

    return v;
}

void free_vector2(vector2_t* v){

    free(v);
}