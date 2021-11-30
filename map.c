#include <stdlib.h>
#include <stdio.h>
#include "map.h"

char* load_map(const char* nomFichier, int* size){
    
    FILE* map_file;
    int c, n = 0;
    
    map_file = fopen(nomFichier, "r");
    if(map_file == NULL){
        
        perror ("Error opening file");
    }
    
    c = fgetc(map_file);
    *size += (int)(c - '0') * 10;
    c = fgetc(map_file);
    *size += (int)(c - '0');

    char* map = (char*)malloc(sizeof(char) * (*size) * (*size));
    do{
        c = fgetc(map_file);
        if((c >= '0' && c <= '9') || c == ' ' || c == '='){
            map[n] = c;
            n++;
        }
    }while (c != EOF);
        
    fclose(map_file);
    return map;
}