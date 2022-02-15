#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "sys/cm4.h"
#include "sys/devices.h"
#include "sys/init.h"
#include "sys/clock.h"


#include "musique.h"
#include "IO.h"

int ARR[12] = {59300,55972,52831,49866,47067,44425,41932,39579,37358,35261,33282,31414};

char partitions[NB_MUSIQUES][NB_CHAR_MAX] = {"G41 G41 G41 A41 B43 A43 G41 B41 A41 A41 G43 G41 G41 G41 A41 B43 A43 G41 B41 A41 A41 G43",
                                    "E42 E42 G44 E42 E42 G44 E42 G42 C54 B44 A44 A44 G42 D42 E42 F44 D42 D42 E42 F44 D42 F42 B42 A42 G44 B44 C54 C42 C42 C54 C42 A42 F44 G42 E42 C42 F42 G42 A43 E43 G43 C43 C42 C42 C54 A42 F42 G44 E42 C42 F43 G43 F43 E43 D43 C45",
                                    "F#43 D43 D41 E41 F42 E42 D42 C#42 D42 E43 B52 F#43 B43 C#41 D41 E41 D41 C#41 A51 G43 F#43 D43 D41 E41 F42 E42 D42 C#42 D42 E43 B52 B51 C#51 D51 G41 F#41 D41 D51 E51",
                                    "C41 G41 D#51 G41 D#51 G41 C41 G41 F31 C41 G#41 C41 G#41 C41 F31 C41 G41 D#51 G41 D#51 G41 C41 G41 C43 _ _ C41 G41 D#51 G41 D#51 G41 C41 G41 F31 C41 G#41 C41 G#41 C41 F31 C41 G41 D#51 G41 D#51 G41 C41 G41 C43 _ _ C41 G41 D#51 G41 D#51 G41 C41 G41 D41 G41 B41 G41 B43 _ A#31 F41 G#41 F41 G#41 F41 A#31 F41 C41 G41 C51 G41 C54 _ _ C53 _ C53 _ C53 A#42 G#42 G42 C53 _ C53 _ C53 B42 A42 G42 C53 _ C54 _"};

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
    while(i < length){
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

void init_musique( musique_t *musique, char* partition,int tempo){
    musique->tempo = tempo;
    musique->dureeNote =  60 * 1000 / tempo / REFRESH_GAP;
    musique->espaceNote = 60 * 200 / tempo / REFRESH_GAP;
    lecture_partition(musique,partition);
}
