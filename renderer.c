#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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

void render(uint32_t* texture, uint32_t* wall_tex, char* map, int width, int height, float player_x, float player_y, float player_angle){

    clear_texture(texture, width, height);

    int texture_size = 256;
    float fov = 3.14 / 3.;
 
    for (int i = 0; i < width; i++) { 
        float angle = player_angle-fov/2 + fov * i / (float)width;
        for (float c = 0; c<30; c+=.01) {
            float x = player_x + c*cos(angle);
            float y = player_y + c*sin(angle);

            int map_index = (int)x + (int)y * 16;
            if (map[map_index] != ' '){
                int texid = map[map_index] - '0';
                float dst = c*cos(angle-player_angle);
                int column_height = (int)(height / dst);
                float hitx = x - floor(x+.5);
                float hity = y - floor(y+.5);
                int texcoord_x = hitx * texture_size;

                if(fabs(hity) > fabs(hitx))
                    texcoord_x = hity * texture_size;

                if (texcoord_x<0)
                    texcoord_x += texture_size;

                uint32_t* column = draw_column(wall_tex, texture_size, 1, 0, texcoord_x, column_height);

                int offset = (int)(height * 0.4);
                for (int j=0; j<column_height; j++) {
                    int pix_y = j + width/2-column_height/2 - offset;
                    if (pix_y<0 || pix_y>=(int)height) 
                        continue;
                    texture[i + pix_y*width] = column[j];
                }
                free(column);
                break;
            }
        }
    }
}