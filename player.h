#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>

struct player_s {
    
    float pos_x;
    float pos_y;
    float angle;
    float turn_velocity;
    float move_velocity;
};

typedef struct player_s player_t;

player_t* init_player(float x, float y);

#endif