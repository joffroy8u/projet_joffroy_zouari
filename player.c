#include "player.h"

player_t* init_player(float x, float y){

    player_t* player = (player_t*)malloc(sizeof(player_t));
    player->pos_x = x;
    player->pos_y = y;
    player->angle = 0;
    player->turn_velocity = 0;
    player->move_velocity = 0;

    return player;
}