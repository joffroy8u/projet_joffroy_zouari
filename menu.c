#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>

#include "game.h"
#include "SDL_utils.h"
#include "menu.h"

button_t* init_button(SDL_Renderer* renderer, int x, int y, int width, int height, char* sprite_path, int (*on_click)()){

    sprite_t* sprite = init_sprite(renderer, sprite_path, x - width/2, y - height/2);
    
    button_t* button = (button_t*)malloc(sizeof(button_t));
    button->center_x = x;
    button->center_y = y;
    button->width = width;
    button->height = height;
    button->sprite = sprite;
    button->on_click = on_click;
    
    return button;
}

int handle_button_events(button_t* button, SDL_Event event, game_state_e game_state){

    int x = 0;
    int y = 0;

    x = event.motion.x;
    y = event.motion.y;

    if(is_mouse_over(button, x, y)){
        if(event.type == SDL_MOUSEBUTTONDOWN){
            if(event.button.button == SDL_BUTTON_LEFT){
                return (*button->on_click)();
            }
        }
    }
    return (int)game_state;
}

bool is_mouse_over(button_t* button, int mouse_x, int mouse_y){
    int half_width = button->width >> 1;
    int half_height = button->height >> 1;
    return mouse_x > button->center_x - half_width && mouse_x < button->center_x + half_width
        && mouse_y > button->center_y - half_height && mouse_y < button->center_y + half_height;
}

void free_button(button_t* button){

    free_sprite(button->sprite);
    free(button);
}

int play(){
    return (int)GAME;
}

int settings(){
    return (int)OPTIONS;
}

int increase_render_dst(){
    return 1;
}

int decrease_render_dst(){
    return -1;
}

int menu(){
    return (int)MENU;
}

int quit(){
    return (int)QUIT;
}