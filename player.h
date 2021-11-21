#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <stdbool.h>
#include "vector2.h"

#define ACCELERATION_RATE 0.07
#define BRAKING_FORCE 0.1
#define TURN_RATE 3.14/2.
#define MAX_SPEED 5.
#define WHEEL_SPACING 2.75
#define CAMERA_DISTANCE 3.4
#define CAR_WIDTH .6

struct player_s {
    
    vector2_t* position;
    float turn_angle;
    float move_velocity;
    vector2_t* cam_position;
    float cam_angle;
    bool accelerating;
    bool braking;
    bool turning_left;
    bool turning_right;
    vector2_t* front_wheel_position;
    vector2_t* back_wheel_position;
};

typedef struct player_s player_t;

player_t* init_player(vector2_t* position);
void update_physics(player_t* player, bool collision, float dt);
void free_player(player_t* player);

#endif