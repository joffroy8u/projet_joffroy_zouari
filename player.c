#include <math.h>

#include "player.h"

player_t* init_player(vector2_t* position){

    player_t* player = (player_t*)malloc(sizeof(player_t));
    player->position = position;
    player->turn_angle = 0;
    player->move_velocity = 0;

    player->cam_position = init_vector2(position->x - CAMERA_DISTANCE, position->y);
    player->cam_angle = 0;

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

void update_physics(player_t* player, bool collision, float dt){

    if(player->turning_left)
        player->turn_angle = -TURN_RATE / (1. + .5 * fabs(player->move_velocity));
    else if(player->turning_right)
        player->turn_angle = TURN_RATE / (1. + .5 * fabs(player->move_velocity));
    else
        player->turn_angle = 0.;

    if(collision){
        player->move_velocity = 0;
        player->accelerating = false;
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
        if(fabs(player->move_velocity) > 0.08){
            player->move_velocity *= 0.96;
        }else{
            player->move_velocity = 0;
        }
    }

    player->front_wheel_position->x = player->position->x + WHEEL_SPACING * .5 * cos(player->cam_angle);
    player->front_wheel_position->y = player->position->y + WHEEL_SPACING * .5 * sin(player->cam_angle);
    player->back_wheel_position->x = player->position->x - WHEEL_SPACING * .5 * cos(player->cam_angle);
    player->back_wheel_position->y = player->position->y - WHEEL_SPACING * .5 * sin(player->cam_angle);

    if(!collision){
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

void free_player(player_t* player){

    free_vector2(player->position);
    free_vector2(player->cam_position);
    free_vector2(player->front_wheel_position);
    free_vector2(player->back_wheel_position);
    free(player);
}