#include <stdlib.h>
#include <stdio.h>*
#include "map.h"

char* lire_fichier(const char* nomFichier){
    
    FILE* map;
    int c, n = 0;
    
    map = fopen("map.txt", "r");
    
    if(map = NULL){
        
        perror ("Error opening file");
    }
    
    else{
        
        do{
            
            c = fgetc(map);
            printf("%c", c);
            n++;
            
        }while (c != EOF);
        
        fclose(map);
        printf("File contains %d caracteres \n", n);
        
    }
    
    return 0;
    
}
