#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "sys/cm4.h"
#include "sys/devices.h"
#include "sys/init.h"
#include "sys/clock.h"


#include "musique.h"

int ARR[12] = {59300,55972,52831,49866,47067,44425,41932,39579,37358,35261,33282,31414};


//Retourne ARR qui correspond à la note
int lecture_note(char c1, char c2, char c3){
    //printf("%c%c%c ",c1,c2,c3);
    if(c1 == '_')
        return 0;
    if(c1 == 'C'){
        if(c2 == '#')
            return ARR[1] >> (c3 - '0' - 2);
        return ARR[0] >> (c2 - '0' - 2);
    }
    if(c1 == 'D'){
        if(c2 == '#')
            return ARR[3] >> (c3 - '0' - 2);
        if(c2 == 'b')
            return ARR[1] >> (c3 - '0' - 2);
        return ARR[2] >> (c2 - '0' - 2);
    }
    if(c1 == 'E'){
    	if(c2 == 'b')
    	    return ARR[1] >> (c3 - '0' - 2); 
        return ARR[4] >> (c2 - '0' - 2);
    }
    if(c1 == 'F'){
        if(c2 == '#')
            return ARR[6] >> (c3 - '0' - 2);
        return ARR[5] >> (c2 - '0' - 2);
    }
    if(c1 == 'G'){
        if(c2 == '#')
            return ARR[8] >> (c3 - '0' - 2);
        if(c2 == 'b')
            return ARR[6] >> (c3 - '0' - 2);
        return ARR[7] >> (c2 - '0' - 2);
    }
    if(c1 == 'A'){
        if(c2 == '#')
            return ARR[10] >> (c3 - '0' - 2);
        if(c2 == 'b')
            return ARR[8] >> (c3 - '0' - 2);
        return ARR[9] >> (c2 - '0' - 2);
    }
    if(c1 == 'B'){
        if(c2 == 'b')
            return ARR[10] >> (c3 - '0' - 2);
        return ARR[11] >> (c2 - '0' - 2);
    }
    return 0;
}


void lecture_partition(musique_t *musique, char *part){
    int i = 0;
    int j = 0; 
    int length = strlen(part);
    while(i < strlen(part)){
        //printf("%c%c", part[i],part[i+1]);
        char c1 = 0,c2 = 0,c3 = 0,c4 = '1';

        c1 = part[i];
        i++;
        if(c1 != '_'){
            c2 = part[i];
            i++;

            if(c2 == '#'){
                //printf("%c", part[i]);

                c3 = part[i];
                i++;

            }
            c4 = part[i];
            i++;
        }
        i++;
        musique->notes[j].arr = lecture_note(c1,c2,c3);
        musique->notes[j].temps =  c4 - '0';
        printf(" arr: %d temps: %d i: %d\r\n",lecture_note(c1,c2,c3), c4 - '0', i);

        //printf("\r\n");
        j++;
    }
    musique->nbNotes = j;
    printf("nombre notes: %d\r\n",j);
    
}

void init_musique(int tempo, musique_t *musique, char* partition){
    
}
