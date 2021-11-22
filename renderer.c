#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "renderer.h"

uint32_t* init_texture(int width, int height, uint32_t color){

    uint32_t* texture = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
            texture[x + y*width] = color;
        }
    }
    return texture;
}

uint32_t* draw_column(uint32_t* img, int texsize, int ntextures, int texid, int texcoord, int column_height){

    int img_w = texsize*ntextures;
    int img_h = texsize;

    uint32_t* column = (uint32_t*)malloc(sizeof(uint32_t) * column_height);
    for (int y=0; y<column_height; y++) {
        int pix_x = texid*texsize + texcoord;
        int pix_y = (y*texsize)/column_height;
        column[y] = img[pix_x + pix_y*img_w];
    }
    return column;
}

void clear_texture(uint32_t* texture, int width, int height){

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
            texture[x + y*width] = y > height * 0.1 ? 0x737373ff : 0xbce7ffff;
        }
    }
}

uint32_t convert_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (r<<24) + (g<<16) + (b<<8) + a;
}

uint32_t* init_walltexture(char* filename){

    SDL_Surface* bmpSurface = SDL_LoadBMP(filename);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(bmpSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(bmpSurface);

    int tex_w = surface->w;
    int tex_h = surface->h;

    uint32_t* img = (uint32_t*)malloc(sizeof(uint32_t) * tex_w * tex_h);

    for(int i = 0; i < tex_w; i++){
        for(int j = 0; j < tex_h; j++){
            uint8_t r = ((uint8_t*)(surface->pixels))[(i+j*tex_w)*4+3];
            uint8_t g = ((uint8_t*)(surface->pixels))[(i+j*tex_w)*4+2];
            uint8_t b = ((uint8_t*)(surface->pixels))[(i+j*tex_w)*4+1];
            uint8_t a = ((uint8_t*)(surface->pixels))[(i+j*tex_w)*4+0];
            img[i+j*tex_w] = convert_color(r, g, b, a);
        }
    }
    
    SDL_FreeSurface(surface);

    return img;
}

SDL_Texture* load_png(const char* nomfichier, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(nomfichier);
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surface);

    if(formattedSurface == NULL)
    {
        printf( "Erreur lors du chargement de l'image %s! Erreur: %s\n", nomfichier, IMG_GetError());
    }
  
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, formattedSurface);
  SDL_FreeSurface(formattedSurface);
  return texture;
}

void render(uint32_t* texture, uint32_t* wall_tex, char* map, player_t* player, int width, int height, int map_size){

    clear_texture(texture, width, height);

    int texture_size = 64;
    float fov = 3.14 / 3.;

    for (int i = 0; i < width; i++) { 
        float angle = (player->cam_angle)-fov/2 + fov * i / (float)width;
        int startIndex = (int)player->cam_position->x + (int)player->cam_position->y * map_size;
        float lod = .01;
        float cos_angle = cos(angle);
        float sin_angle = sin(angle);
        for (float c = 0; c<40; c+=lod) {
            float x = (player->cam_position->x + c * cos_angle) * .5;
            float y = (player->cam_position->y + c * sin_angle) * .5;

            int map_index = (int)x + (int)y * map_size;
            if (map[map_index] == '0'){
                int texid = map[map_index] - '0';
                float dst = c * cos(angle - (player->cam_angle));
                int column_height = (int)(height / dst) * 2.;
                
                float hitx = x - floor(x+.5);
                float hity = y - floor(y+.5);
                int texcoord_x = hitx * texture_size;

                if(fabs(hity) > fabs(hitx))
                    texcoord_x = hity * texture_size;

                if (texcoord_x<0)
                    texcoord_x += texture_size;

                uint32_t* column = draw_column(wall_tex, texture_size, 1, 0, texcoord_x, column_height);

                int offset = (int)(height * 0.45);
                int min_y = width/2-column_height/2 - offset;
                int max_y = width/2+column_height/2 - offset;
                for (int j=0; j<height; j++) {                    
                    if(j < min_y)
                        texture[i + j*width] = 0xbce7ffff;
                    else if(j >= max_y)
                        texture[i + j*width] = 0x737373ff;
                    else
                        texture[i + j*width] = column[j-min_y];
                }
                
                free(column);
                break;
            }
            if(c > 25)
                lod += 0.02;
        }
    }
}