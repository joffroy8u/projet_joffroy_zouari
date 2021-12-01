#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>

#include "SDL_utils.h"
#include "sprite.h"
#include "renderer.h"
#include "player.h"
#include "vector2.h"
#include "map.h"
#include "building.h"

int main(int argc, char *argv[]){

    SDL_Window* window;
    SDL_Event events;
    SDL_Renderer* renderer;
    bool end = false;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur d’initialisation de la SDL: %s\n",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    } 

    if(IMG_Init(IMG_INIT_PNG) < 0)
    {
        printf( "Erreur d’initialisation de SDL_image: %s\n", IMG_GetError() );
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
        printf("Erreur lors de la creation de la fenetre: %s\n",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialisation TTF
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("arial.ttf", 22);
    if(!font){
        printf("Erreur lors du chargement de la police: %s\n", TTF_GetError());
    }

    SDL_Texture *main_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    uint32_t* img = init_texture(w, h, 0xffffffff);

    building_t* buildings[BUILDINGS_COUNT];
    char building_file_name[20] = "building_0.txt";
    for(int i = 0; i < BUILDINGS_COUNT; i++){
        sprintf(building_file_name, "building_%d.txt", i);
        buildings[i] = load_building(building_file_name);
    }

    sprite_t* car_sprite = init_sprite(renderer, "car.png", 0, 0);
    sprite_t* car_braking_sprite = init_sprite(renderer, "car_braking.png", 0, 0);

    int size = 0;
    char* map = load_map("map.txt", &size);

    int last_ticks = 0;

    player_t* player = init_player(init_vector2(14,28));

    // Initialisation chronometre
    float total_timer = 0;
    float current_lap_timer = 0;
    bool on_finish_line = false;

    SDL_Color black = {0,0,0,255};
    SDL_Texture* text;
    SDL_Rect text_pos;
    char time_str[7] = "000.000";
    text = load_text(time_str, renderer, font, black);
    SDL_QueryTexture(text, NULL, NULL, &text_pos.w, &text_pos.h);
    text_pos.x = 10;
    text_pos.y = h - text_pos.h;

    while(!end){

        SDL_DestroyTexture(text);

        //sprintf(time_str, "%d", (int)(1000/(SDL_GetTicks() - last_ticks)));

        float delta_time = (SDL_GetTicks() - last_ticks) / 1000.;
        last_ticks = SDL_GetTicks();
        if(delta_time == 0)
            delta_time = 1.;

        total_timer += delta_time;
        current_lap_timer += delta_time;

        
        sprintf(time_str, "%d.%d", (int)total_timer, (int)((total_timer - (int)total_timer) * 1000));
        text = load_text(time_str, renderer, font, black);
        SDL_QueryTexture(text, NULL, NULL, &text_pos.w, &text_pos.h);

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
                        player->accelerating = true;
                        break;
                    case SDLK_s:
                        player->braking = true;
                        break;
                    case SDLK_d:
                        player->turning_right = true;
                        break;
                    case SDLK_q:
                        player->turning_left = true;
                        break;
                }
            }

            if(events.type == SDL_KEYUP && events.key.repeat == 0){
                switch(events.key.keysym.sym)
                {
                    case SDLK_z:
                        player->accelerating = false;
                        break;
                    case SDLK_s:
                        player->braking = false;
                        break;
                    case SDLK_d:
                        player->turning_right = false;
                        break;
                    case SDLK_q:
                        player->turning_left = false;
                        break;
                }
            }
        }

        if(check_collision_finish(map, size, player->front_wheel_position->x, player->front_wheel_position->y) && player->move_velocity > 0){
            if(!on_finish_line){
                //printf("Temps dernier tour = %f\n", current_lap_timer);
                current_lap_timer = 0;
            }
            on_finish_line = true;
        }else{
            on_finish_line = false;
        }

        update_physics(map, size, player, delta_time);

        render(img, buildings, map, player, w, h, size);

        SDL_UpdateTexture(main_texture, NULL, (void*)img, w*4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, main_texture, NULL, NULL);

        if((player->braking && player->move_velocity >= 0) || (player->accelerating && player->move_velocity < 0))
            SDL_RenderCopy(renderer, car_braking_sprite->texture, &car_braking_sprite->src_pos, &car_braking_sprite->dst_pos);
        else
            SDL_RenderCopy(renderer, car_sprite->texture, &car_sprite->src_pos, &car_sprite->dst_pos);

        SDL_RenderCopy(renderer, text, NULL, &text_pos);
        SDL_RenderPresent(renderer);

        //SDL_Delay(5);
    }

    free(img);
    free_buildings(buildings, BUILDINGS_COUNT);
    free_player(player);
    free(map);

    TTF_CloseFont(font);
    TTF_Quit();

    if(text != NULL)
        SDL_DestroyTexture(text);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    
    return 0;
    
}