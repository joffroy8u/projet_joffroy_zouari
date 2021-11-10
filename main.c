#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <math.h>

#include "renderer.h"
#include "player.h"
#include "map.h"

int main(int argc, char *argv[]){

    SDL_Window* window;
    SDL_Event events;
    SDL_Renderer* renderer;
    bool end = false;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur d’initialisation de la SDL: %s",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    } 
    
    int w = 512;
    int h = 512;

    // Créer la fenêtre
    window = SDL_CreateWindow("Jeu", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_RESIZABLE);
    if(window == NULL)
    {
        printf("Erreur lors de la creation de la fenetre: %s",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();

    SDL_Texture *main_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    uint32_t* img = init_texture(w, h, 0xffffffff);
    uint32_t* wall_tex = init_walltexture("building.bmp");

    int map_width = 16;
    int map_height = 16;

    int size = 0;
    char* map = lire_fichier("map.txt", &size);
    //printf("%s\n", map);
                //"0000000000000000"\
                "0 0            0"\
                "0 0            0"\
                "000    00000   0"\
                "0     0        0"\
                "0     0     0000"\
                "0    000       0"\
                "000  000       0"\
                "0    00000000  0"\
                "0    0000      0"\
                "0       0      0"\
                "0       0  00000"\
                "0     000      0"\
                "0000           0"\
                "0              0"\
                "0000000000000000";

    float player_angle = 3.14/2.;
    float player_x = 3;
    float player_y = 3;
    float turn_speed = 1.2;
    float move_speed = 4.;

    int last_ticks = 0;
    float turn_velocity = 0.;
    float move_velocity = 0.;

    player_t* player = init_player(3,3);

    while(!end){

        float delta_time = (SDL_GetTicks() - last_ticks) / 1000.;
        last_ticks = SDL_GetTicks();
        if(delta_time == 0)
            delta_time = 1.;

        SDL_RenderClear(renderer);
    
        while(SDL_PollEvent(&events) != 0){

            if(events.type == SDL_QUIT){
                end = true;
            }
                
            if(events.type == SDL_KEYDOWN && events.key.repeat == 0){
                switch(events.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        end = true;
                        break;
                    case SDLK_z:
                        player->move_velocity = move_speed * delta_time;
                        break;
                    case SDLK_s:
                        player->move_velocity = -move_speed * delta_time;
                        break;
                    case SDLK_d:
                        player->turn_velocity = turn_speed * delta_time;
                        break;
                    case SDLK_q:
                        player->turn_velocity = -turn_speed * delta_time;
                        break;
                }
            }

            if(events.type == SDL_KEYUP && events.key.repeat == 0){
                switch(events.key.keysym.sym)
                {
                    case SDLK_z:
                        player->move_velocity = 0.;
                        break;
                    case SDLK_s:
                        player->move_velocity = 0.;
                        break;
                    case SDLK_d:
                        player->turn_velocity = 0.;
                        break;
                    case SDLK_q:
                        player->turn_velocity = 0.;
                        break;
                }
            }
        }
        
        player->angle += player->turn_velocity;
        player->pos_x += player->move_velocity * cos(player->angle);
        player->pos_y += player->move_velocity * sin(player->angle);
    
        render(img, wall_tex, map, player, w, h);

        SDL_UpdateTexture(main_texture, NULL, (void*)img, w*4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, main_texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(5);
    }

    free(img);
    free(wall_tex);
    free(player);
    free(map);

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    
    return 0;
    
}