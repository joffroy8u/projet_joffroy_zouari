#include <stdio.h>
#include <stdlib.h>

#include "save.h"

void save_score(float time){

    FILE* save_file;
    int c, n = 0;
    
    save_file = fopen("save.txt", "w");
    if(save_file == NULL){
        
        char* score_str[7] = "000.000";
        sprintf(score_str, "%d.%d", (int)current_lap_timer, (int)((time - (int)time) * 1000));
        fputs(score_str, save_file);

        perror ("Error opening file");
    }
    
    fclose(save_file);
}
