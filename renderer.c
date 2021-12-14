#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "SDL_utils.h"
#include "renderer.h"

#define SKY_COLOR 0xbce7ffff
#define GROUND_COLOR 0x737373ff
#define FOV 1.0467 // PI / 3
#define MAX_BUILDING_HEIGHT 2.0
#define OBSTACLE_TEXTURE_SIZE 128

uint32_t* init_texture(int width, int height, uint32_t color){

    uint32_t* texture = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
            texture[x + y*width] = color;
        }
    }
    return texture;
}

void draw_column(uint32_t* texture, int width, int height, building_t* building, int tex_coord, int x, int min_y, int max_y, int max_y_ground, float current_height){

    int column_height = abs(max_y_ground - min_y);

    for (int y = 0; y < height; y++) {

        if(y < min_y){
            texture[x + y * width] = SKY_COLOR;
        }
        else if(y >= max_y){
            if(current_height < 1.0)
                texture[x + y * width] = GROUND_COLOR;
        }
        else{
            int pix_x = tex_coord;
            int pix_y = ((y - min_y) * building->texture_height) / column_height;

            texture[x + y * width] = building->texture[pix_x + pix_y * building->texture_width];
        }
    }
}

void clear_texture(uint32_t* texture, int width, int height){

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
            texture[x + y*width] = y > height * 0.1 ? 0x737373ff : 0xbce7ffff;
        }
    }
}

void render(uint32_t* texture, building_t** buildings, obstacle_t** obstacles, char* map, player_t* player, int width, int height, int map_size, int obstacle_count, int render_dst){

    clear_texture(texture, width, height);

    float depth_buffer[width];
    float inv_width = 1. / (float)width;
    float width_half = width >> 1;
    for (int i = 0; i < width; i++) { 
        float angle = (player->cam_angle) - (FOV * 0.5) + FOV * i * inv_width;
        int startIndex = (int)player->cam_position->x + (int)player->cam_position->y * map_size;
        float lod = 0.01;
        float cos_angle = cos(angle);
        float sin_angle = sin(angle);
        float cos_angle_minus_cam_angle = cos(angle - (player->cam_angle));
        float current_height = 0.;
        int current_max_y = 0;
        for (float c = 0; c < render_dst; c+=lod) {
            float x = (player->cam_position->x + c * cos_angle) * .25;
            float y = (player->cam_position->y + c * sin_angle) * .25;

            if(x < 0 || x >= map_size || y < 0 || y >= map_size) break;

            int map_index = (int)x + (int)y * map_size;
            
            if (map[map_index] >= '0' && map[map_index] <= '9'){
                int building_id = map[map_index] - '0';
                float hit_height = buildings[building_id]->building_height;
                
                if(hit_height <= current_height) continue;

                current_height = hit_height;
                

                int texture_width = buildings[building_id]->texture_width;
                float dst = c * cos_angle_minus_cam_angle;
                
                if(current_max_y == 0)
                    depth_buffer[i] = dst;

                int column_half_height = (int)((height / dst) * 2. * hit_height);

                float hitx = x - floor(x+.5);
                float hity = y - floor(y+.5);
                int tex_coord_x = hitx * texture_width;

                if(fabs(hity) > fabs(hitx))
                    tex_coord_x = hity * texture_width;

                if (tex_coord_x < 0)
                    tex_coord_x += texture_width;

                int offset = (int)(height * 0.4) + (hit_height > 1.0 ? (int)(column_half_height - column_half_height / hit_height) : 0);
                int min_y = width_half - column_half_height - offset;
                int max_y = width_half + column_half_height - offset;
                int max_y_ground = max_y;
                if(current_max_y != 0) max_y = current_max_y;
                
                draw_column(texture, width, height, buildings[building_id], tex_coord_x, i, min_y, max_y, max_y_ground, hit_height);
                
                current_max_y = min_y;
                
                if(building_id == 3){
                    current_height = 0;
                    lod = 0.1;
                    current_max_y = 0;
                }

                if(hit_height >= MAX_BUILDING_HEIGHT)
                    break;
                
            }
            
            if(current_height > 0. || c >= 45){
                lod = 0.02;
            }
        }
    }

    for(int i = 1; i < obstacle_count; i++){
        draw_obstacle(texture, width, height, depth_buffer, obstacles[i], player);
    }
}

void draw_obstacle(uint32_t* texture, int width, int height, float depth_buffer[width], obstacle_t* sprite, player_t* player){

    float sprite_dir = atan2(sprite->position->y - player->cam_position->y, sprite->position->x - player->cam_position->x);
    while (sprite_dir - player->cam_angle >  3.14) sprite_dir -= 2*3.14; 
    while (sprite_dir - player->cam_angle < -3.14) sprite_dir += 2*3.14;

    float sprite_dst = distance(sprite->position, player->position);
    int sprite_size = fmin(2000, height / sprite_dst * 4);
    int x_offset = (sprite_dir - player->cam_angle) * width / FOV + (width >> 1) - (sprite_size >> 1);
    int y_offset = (height >> 1) - (sprite_size >> 1) - (height * 0.4);

    float theta = -(sprite->direction - sprite_dir) * 180. / 3.14;
    if(theta < 0) theta = 360 + theta;
    int sprite_index = theta / 30;

    if(sprite->sprite_count == 1) sprite_index = 0;

    for (int i = 0; i < sprite_size; i++) {
        if (x_offset + i < 0 || x_offset + i >= width) continue;
        if (depth_buffer[x_offset + i] < sprite_dst) continue;

        depth_buffer[x_offset + i] = sprite_dst;
        for (int j = 0; j < sprite_size; j++) {
            if (y_offset + j < 0 || y_offset + j >= height) continue;

            uint32_t color = sprite->textures[sprite_index][i * OBSTACLE_TEXTURE_SIZE / sprite_size + (j * OBSTACLE_TEXTURE_SIZE / sprite_size) * OBSTACLE_TEXTURE_SIZE];
            uint8_t r,g,b,a;
            read_color(&color, &r, &g, &b, &a);
            if(a > 4 || r != 0 || g != 0 || b != 0)
                texture[width + x_offset + i + (y_offset+j) * width] = color;
        }
    }
}