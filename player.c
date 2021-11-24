#include <math.h>

#include "player.h"

player_t* init_player(vector2_t* position){

    player_t* player = (player_t*)malloc(sizeof(player_t));
    player->position = position;
    player->turn_angle = 0;
    player->move_velocity = 0;

    player->cam_position = init_vector2(position->x - CAMERA_DISTANCE, position->y);
    player->cam_angle = -3.14/2.;

    player->accelerating = false;
    player->braking = false;
    player->turning_left = false;
    player->turning_right = false;

    player->front_wheel_position = init_vector2(position->x + WHEEL_SPACING * .5 * cos(player->cam_angle),
                                                position->y + WHEEL_SPACING * .5 * sin(player->cam_angle));
    player->back_wheel_position = init_vector2(position->x - WHEEL_SPACING * .5 * cos(player->cam_angle),
                                                position->y - WHEEL_SPACING * .5 * sin(player->cam_angle));
    
    return player;
}

void update_physics(char* map, int map_size, player_t* player, float dt){

    if(player->turning_left)
        player->turn_angle = -TURN_RATE / (1. + .4 * fabs(player->move_velocity));
    else if(player->turning_right)
        player->turn_angle = TURN_RATE / (1. + .4 * fabs(player->move_velocity));
    else
        player->turn_angle = 0.;

    float left_wheel_x = player->front_wheel_position->x - CAR_WIDTH * cos(player->cam_angle + 3.14/2.) * .5;
    float left_wheel_y = player->front_wheel_position->y - CAR_WIDTH * sin(player->cam_angle + 3.14/2.) * .5;
    float right_wheel_x = player->front_wheel_position->x + CAR_WIDTH * cos(player->cam_angle + 3.14/2.) * .5;
    float right_wheel_y = player->front_wheel_position->y + CAR_WIDTH * sin(player->cam_angle + 3.14/2.) * .5;

    bool collision_front = (check_collision_wall(map, map_size, left_wheel_x, left_wheel_y)
                         || check_collision_wall(map, map_size, right_wheel_x, right_wheel_y))
                         && player->move_velocity >= 0.;

    left_wheel_x = player->cam_position->x - CAR_WIDTH * cos(player->cam_angle + 3.14/2.1) * .5;
    left_wheel_y = player->cam_position->y - CAR_WIDTH * sin(player->cam_angle + 3.14/2.1) * .5;
    right_wheel_x = player->cam_position->x + CAR_WIDTH * cos(player->cam_angle + 3.14/1.9) * .5;
    right_wheel_y = player->cam_position->y + CAR_WIDTH * sin(player->cam_angle + 3.14/1.9) * .5;

    bool collision_back = (check_collision_wall(map, map_size, left_wheel_x, left_wheel_y)
                        || check_collision_wall(map, map_size, right_wheel_x, right_wheel_y))
                        && player->move_velocity <= 0.;

    if(collision_front){
        player->move_velocity = 0;
        player->accelerating = false;
    }
    if(collision_back){
        player->move_velocity = 0;
        player->braking = false;
    }

    if(player->accelerating)
        player->move_velocity += ACCELERATION_RATE;
    else if(player->braking)
        player->move_velocity -= BRAKING_FORCE;

    if(player->move_velocity > MAX_SPEED)
        player->move_velocity = MAX_SPEED;
    if(player->move_velocity < -1)
        player->move_velocity = -1;

    if(!player->accelerating && !player->braking){
        if(fabs(player->move_velocity) > MIN_SPEED){
            player->move_velocity *= FRICTION;
        }else{
            player->move_velocity = 0;
        }
    }

    player->front_wheel_position->x = player->position->x + WHEEL_SPACING * .5 * cos(player->cam_angle);
    player->front_wheel_position->y = player->position->y + WHEEL_SPACING * .5 * sin(player->cam_angle);
    player->back_wheel_position->x = player->position->x - WHEEL_SPACING * .5 * cos(player->cam_angle);
    player->back_wheel_position->y = player->position->y - WHEEL_SPACING * .5 * sin(player->cam_angle);

    if(!collision_front && !collision_back){
        player->back_wheel_position->x += player->move_velocity * cos(player->cam_angle) * dt;
        player->back_wheel_position->y += player->move_velocity * sin(player->cam_angle) * dt;
        player->front_wheel_position->x += player->move_velocity * cos(player->cam_angle + player->turn_angle) * dt;
        player->front_wheel_position->y += player->move_velocity * sin(player->cam_angle + player->turn_angle) * dt;
    }

    player->position->x = (player->front_wheel_position->x + player->back_wheel_position->x) * 0.5;
    player->position->y = (player->front_wheel_position->y + player->back_wheel_position->y) * 0.5;
    player->cam_angle = atan2(player->front_wheel_position->y - player->back_wheel_position->y,
                              player->front_wheel_position->x - player->back_wheel_position->x);
    player->cam_position->x = player->position->x - CAMERA_DISTANCE * cos(player->cam_angle);
    player->cam_position->y = player->position->y - CAMERA_DISTANCE * sin(player->cam_angle);
}

bool check_collision_wall(char* map, int map_size, float pos_x, float pos_y){

    int index = (int)(pos_x * .25) + (int)(pos_y * .25) * map_size;
    return map[index] != ' ' && map[index] != '=';
}

bool check_collision_finish(char* map, int map_size, float pos_x, float pos_y){

    int index = (int)(pos_x * .25) + (int)(pos_y * .25) * map_size;
    return map[index] == '=';
}

void free_player(player_t* player){

    free_vector2(player->position);
    free_vector2(player->cam_position);
    free_vector2(player->front_wheel_position);
    free_vector2(player->back_wheel_position);
    free(player);
}