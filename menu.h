#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "sprite.h"
#include "vector2.h"
#include "game.h"

typedef struct {

    int center_x;
    int center_y;
    int width;
    int height;
    sprite_t* sprite;
    int (*on_click)(); // Fonction à exécuter quand le bouton est cliqué.

} button_t;

button_t* init_button(SDL_Renderer* renderer, int x, int y, int width, int height, char* sprite_path, int (*on_click)());
int handle_button_events(button_t* button, SDL_Event event, game_state_e game_state);
bool is_mouse_over(button_t* button, int mouse_x, int mouse_y);
void free_button(button_t* button);
int play();
int settings();
int increase_render_dst();
int decrease_render_dst();
int menu();
int quit();

#endif