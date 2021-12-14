#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>

#include "game.h"
#include "SDL_utils.h"
#include "sprite.h"
#include "renderer.h"
#include "player.h"
#include "vector2.h"
#include "map.h"
#include "building.h"
#include "roads.h"
#include "menu.h"
#include "obstacle.h"

int main(int argc, char *argv[]){

    bool debug = false;

    SDL_Window* window;
    SDL_Event event;
    SDL_Renderer* renderer;
    bool end = false;
    game_state_e game_state = MENU;
    int render_dst = 60;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur d’initialisation de la SDL: %s\n",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    } 

    if(IMG_Init(IMG_INIT_PNG) < 0)
    {
        printf("Erreur d’initialisation de SDL_image: %s\n", IMG_GetError() );
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

    float finish_x, finish_y;
    road_vertex_t** roads = build_roads(map, size, &finish_x, &finish_y);
    

    int last_ticks = 0;

    player_t* player = init_player(init_vector2(finish_x - 1,finish_y+1));

    // Initialisation chronometre
    float total_timer = 0;
    float current_lap_timer = 0;
    bool on_finish_line = false;

    SDL_Color black = {0,0,0,255};
    SDL_Color white = {255,255,255,255};
    SDL_Texture* text;
    SDL_Rect text_pos;
    char time_str[7] = "000.000";
    text = load_text(time_str, renderer, font, black);
    SDL_QueryTexture(text, NULL, NULL, &text_pos.w, &text_pos.h);
    text_pos.x = 10;
    text_pos.y = h - text_pos.h;

    SDL_Texture* text_render_dst;
    SDL_Rect text_render_dst_pos;
    char render_dst_str[3] = "000";
    text_render_dst = load_text(render_dst_str, renderer, font, white);
    SDL_QueryTexture(text, NULL, NULL, &text_render_dst_pos.w, &text_render_dst_pos.h);
    text_render_dst_pos.x = w/2 - 10;
    text_render_dst_pos.y = h/2 - text_render_dst_pos.h/2;

    // Initialisation des boutons
    // Menu principal
    button_t* button_play = init_button(renderer, w/2, h/2 - 120, 140, 70, "menu/button_play.png", play);
    button_t* button_settings = init_button(renderer, w/2, h/2 - 40, 140, 70, "menu/button_options.png", settings);
    button_t* button_quit = init_button(renderer, w/2, h/2 + 40, 140, 70, "menu/button_quit.png", quit);

    // Menu options
    button_t* button_increase_render_dst =  init_button(renderer, w/2 + 70, h/2, 70, 70, "menu/button_right.png", increase_render_dst);
    button_t* button_decrease_render_dst =  init_button(renderer, w/2 - 70, h/2, 70, 70, "menu/button_left.png", decrease_render_dst);
    button_t* button_back = init_button(renderer, w/2, h - 75, 140, 70, "menu/button_back.png", menu);

    //Initialisation des obstacles
    int obstacle_count = 2;
    obstacle_t* obstacles[obstacle_count];
    char** sprite_names = (char**)malloc(sizeof(char*) * 12);
    for(int i = 0; i < 12; i++){
        char name[32] = "obstacle/car_obstacle_XXX.png";
        sprintf(name, "obstacle/car_obstacle_%d.png", i * 30);
        sprite_names[i] = (char*)malloc(sizeof(char) * 32);
        strcpy(sprite_names[i], name);
    }
    obstacles[0] = init_obstacle(init_vector2(8, 20), sprite_names, 12, size*size);

    free(sprite_names);
    sprite_names = (char**)malloc(sizeof(char*) * 1);
    sprite_names[0] = "finish_line.png";
    obstacles[1] = init_obstacle(init_vector2(finish_x,finish_y), sprite_names, 1, size*size);

    while(!end){

        SDL_DestroyTexture(text);

        //sprintf(time_str, "%d", (int)(1000/(SDL_GetTicks() - last_ticks)));
        sprintf(render_dst_str, "%d", render_dst);

        float delta_time = (SDL_GetTicks() - last_ticks) / 1000.;
        last_ticks = SDL_GetTicks();
        if(delta_time == 0)
            delta_time = 1.;

        if(game_state == GAME){
            total_timer += delta_time;
            current_lap_timer += delta_time;
        }

        
        sprintf(time_str, "%d.%d", (int)current_lap_timer, (int)((current_lap_timer - (int)current_lap_timer) * 1000));
        text = load_text(time_str, renderer, font, black);
        SDL_QueryTexture(text, NULL, NULL, &text_pos.w, &text_pos.h);

        text_render_dst = load_text(render_dst_str, renderer, font, white);
        SDL_QueryTexture(text_render_dst, NULL, NULL, &text_render_dst_pos.w, &text_render_dst_pos.h);
        //printf("%d\n", render_dst);

        while(SDL_PollEvent(&event) != 0){

            if(event.type == SDL_QUIT){
                end = true;
                break;
            }

            if(game_state == MENU){
                game_state = handle_button_events(button_play, event, game_state);
                game_state = handle_button_events(button_settings, event, game_state);
                end = (handle_button_events(button_quit, event, game_state) == 3);
            }
            else if(game_state == OPTIONS){
                game_state = handle_button_events(button_back, event, game_state);
                render_dst += (handle_button_events(button_increase_render_dst, event, game_state) == 5 ? 5 : 0);
                render_dst += (handle_button_events(button_decrease_render_dst, event, game_state) == -5 ? -5 : 0);
                if(render_dst < 30) render_dst = 30;
                if(render_dst > 100) render_dst = 100;
            }
            else if(game_state == GAME){
                if(event.type == SDL_KEYDOWN && event.key.repeat == 0){
                    switch(event.key.keysym.sym)
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

                if(event.type == SDL_KEYUP && event.key.repeat == 0){
                    switch(event.key.keysym.sym)
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
        }

        SDL_RenderClear(renderer);

        if(game_state == GAME){
            if(check_collision_finish(map, size, player->front_wheel_position->x, player->front_wheel_position->y) && player->move_velocity > 0){
                if(!on_finish_line){
                    printf("Temps dernier tour = %f\n", current_lap_timer);
                    current_lap_timer = 0;
                }
                on_finish_line = true;
            }else{
                on_finish_line = false;
            }

            update_physics(map, size, player, delta_time);
            for(int i = 0; i < obstacle_count-1; i++){
                move_towards_next_vertex(roads, obstacles[i], delta_time);
                if(check_collision_obstacle(player, obstacles[i])){
                    //printf("collision\n");
                }
            }

            if(debug)
                render_roads_map(img, w, h, roads, size);
            else
                render(img, buildings, obstacles, map, player, w, h, size, obstacle_count, render_dst);

            SDL_UpdateTexture(main_texture, NULL, (void*)img, w*4);
            SDL_RenderCopy(renderer, main_texture, NULL, NULL);
            
            if(!debug){
                SDL_RenderCopy(renderer, text, NULL, &text_pos);
                if((player->braking && player->move_velocity >= 0) || (player->accelerating && player->move_velocity < 0))
                    SDL_RenderCopy(renderer, car_braking_sprite->texture, &car_braking_sprite->src_pos, &car_braking_sprite->dst_pos);
                else
                    SDL_RenderCopy(renderer, car_sprite->texture, &car_sprite->src_pos, &car_sprite->dst_pos);
            }
        }
        else if(game_state == MENU){
            SDL_RenderCopy(renderer, button_play->sprite->texture, NULL, &button_play->sprite->dst_pos);
            SDL_RenderCopy(renderer, button_settings->sprite->texture, NULL, &button_settings->sprite->dst_pos);
            SDL_RenderCopy(renderer, button_quit->sprite->texture, NULL, &button_quit->sprite->dst_pos);
        }
        else if(game_state == OPTIONS){
            SDL_RenderCopy(renderer, button_back->sprite->texture, NULL, &button_back->sprite->dst_pos);
            SDL_RenderCopy(renderer, button_increase_render_dst->sprite->texture, NULL, &button_increase_render_dst->sprite->dst_pos);
            SDL_RenderCopy(renderer, button_decrease_render_dst->sprite->texture, NULL, &button_decrease_render_dst->sprite->dst_pos);
            SDL_RenderCopy(renderer, text_render_dst, NULL, &text_render_dst_pos);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(2);
    }

    for(int i = 0; i < 12; i++){
        free(sprite_names[i]);
    }
    free(sprite_names);
    for(int i = 0; i < obstacle_count; i++){
        free_obstacle(obstacles[i]);
    }
    free(car_sprite);
    free(car_braking_sprite);
    free_button(button_play);
    free_button(button_settings);
    free_button(button_quit);
    free_button(button_back);
    free_button(button_increase_render_dst);
    free_button(button_decrease_render_dst);
    free_roads(roads, size);
    free(img);
    free_buildings(buildings, BUILDINGS_COUNT);
    free_player(player);
    free(map);

    TTF_CloseFont(font);
    TTF_Quit();

    if(text != NULL)
        SDL_DestroyTexture(text);
    if(text_render_dst != NULL)
        SDL_DestroyTexture(text_render_dst);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    
    return 0;
}