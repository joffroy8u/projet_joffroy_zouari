#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <stdbool.h>
#include "vector2.h"

#define ACCELERATION_RATE 0.06
#define BRAKING_FORCE 0.15
#define FRICTION 0.98
#define TURN_RATE 3.14/3.
#define MIN_SPEED 0.1
#define MAX_SPEED 10.
#define WHEEL_SPACING 2.7
#define CAMERA_DISTANCE 3.4
#define CAR_WIDTH 1.25

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
void update_physics(char* map, int map_size, player_t* player, float dt);
bool check_collision_wall(char* map, int map_size, float pos_x, float pos_y);
bool check_collision_finish(char* map, int map_size, float pos_x, float pos_y);
void free_player(player_t* player);

#endif