#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>

#include "renderer.h"
#include "player.h"
#include "vector2.h"
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
        printf("Erreur lors de la creation de la fenetre: %s",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();

    SDL_Texture *main_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    uint32_t* img = init_texture(w, h, 0xffffffff);
    uint32_t* wall_tex = init_walltexture("building.bmp");

    SDL_Texture *car_texture = load_png("car.png", renderer);
    SDL_Rect src;
    SDL_Rect dst;
    SDL_QueryTexture(car_texture, NULL, NULL, &src.w, &src.h);
    src.x = 0;
    src.y = 0;
    dst.x = 0;
    dst.y = 0;
    dst.w = src.w;
    dst.h = src.h;

    int size = 0;
    char* map = lire_fichier("map.txt", &size);

    int last_ticks = 0;

    player_t* player = init_player(init_vector2(7,14));

    float total_timer = 0;
    float current_lap_timer = 0;
    bool on_finish_line = false;

    while(!end){

        float delta_time = (SDL_GetTicks() - last_ticks) / 1000.;
        last_ticks = SDL_GetTicks();
        if(delta_time == 0)
            delta_time = 1.;

        total_timer += delta_time;
        current_lap_timer += delta_time;

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

        if(check_collision_finish(map, size, player->position->x, player->position->y) && player->move_velocity > 0){
            if(!on_finish_line){
                printf("Whole time = %f\n", total_timer);
                printf("Last lap = %f\n", current_lap_timer);
                current_lap_timer = 0;
            }
            on_finish_line = true;
        }else{
            on_finish_line = false;
        }

        update_physics(map, size, player, delta_time);

        render(img, wall_tex, map, player, w, h, size);

        SDL_UpdateTexture(main_texture, NULL, (void*)img, w*4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, main_texture, NULL, NULL);
        SDL_RenderCopy(renderer, car_texture, &src, &dst);
        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    free(img);
    free(wall_tex);
    free_player(player);
    free(map);

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    
    return 0;
    
}