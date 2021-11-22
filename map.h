#ifndef MAP_H
#define MAP_H

#include "vector2.h"

char* lire_fichier(const char* nomFichier, int* size);
vector2_t* get_player_position(char* map, int size);

#endif
