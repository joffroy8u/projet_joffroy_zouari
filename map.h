#ifndef MAP_H
#define MAP_H

#include "vector2.h"

typedef enum { N,E,S,W,NW,NE,SE,SW } direction_t;

char* load_map(const char* nomFichier, int* size);

#endif
