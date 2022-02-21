#include <string.h>

#include "musique.h"
#include "IO.h"

int ARR[12] = {59300,55972,52831,49866,47067,44425,41932,39579,37358,35261,33282,31414};

char partitions[NB_MUSIQUES][NB_CHAR_MAX] = {"G41 G41 G41 A41 B43 A43 G41 B41 A41 A41 G43 G41 G41 G41 A41 B43 A43 G41 B41 A41 A41 G43 A41 A41 A41 A41 E43 A41 G41 F41 E41 D43 G41 G41 G41 A41 B43 A43 G41 B41 A41 A41 G43 _ _",
     "E42 E42 G44 E42 E42 G44 E42 G42 C54 B44 A44 A44 G42 D42 E42 F44 D42 D42 E42 F44 D42 F42 B42 A42 G44 B44 C54 C42 C42 C54 C42 A42 F44 G42 E42 C42 F42 G42 A43 E43 G43 C43 C42 C42 C54 A42 F42 G44 E42 C42 F43 G43 F43 E43 D43 C45 _ _",                                                                                  //
    "F#48 D49 D41 E41 F43 E43 D42 C#43 D43 E42 F#48 B58 B42 C#42 D43 E43 D42 C#43 A53 G42 F#48 D49 D41 E41 F43 E43 D42 C#43 D43 E42 F#48 B48 B42 C#52 D53 G43 F#42 F43 D53 E52",
    "C41 G41 D#51 G41 D#51 G41 C41 G41 F31 C41 G#41 C41 G#41 C41 F31 C41 G41 D#51 G41 D#51 G41 C41 G41 C43 _ _ C41 G41 D#51 G41 D#51 G41 C41 G41 F31 C41 G#41 C41 G#41 C41 F31 C41 G41 D#51 G41 D#51 G41 C41 G41 C43 _ _ C41 G41 D#51 G41 D#51 G41 C41 G41 D41 G41 B41 G41 B43 _ A#31 F41 G#41 F41 G#41 F41 A#31 F41 C41 G41 C51 G41 C54 _ _ C53 _ C53 _ C53 A#42 G#42 G42 C53 _ C53 _ C53 B42 A42 G42 C53 _ C54 _"};

//Retourne ARR qui correspond à la note
int lecture_note(char c1, char c2, char c3){
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
        char c1 = 0,c2 = 0,c3 = 0,c4 = '1';

        c1 = part[i];
        i++;
        if(c1 != '_'){
            c2 = part[i];
            i++;

            if(c2 == '#'){

                c3 = part[i];
                i++;

            }
            c4 = part[i];
            i++;
        }
        i++;
        musique->notes[j].arr = lecture_note(c1,c2,c3);
        musique->notes[j].temps =  c4 - '0';

        j++;
    }
    musique->nbNotes = j;    
}

void init_musique( musique_t *musique, char* partition,int tempo){
    musique->tempo = tempo;
    musique->dureeNote =  60 * 1000 / tempo / REFRESH_GAP;
    musique->espaceNote = 60 * 200 / tempo / REFRESH_GAP;
    lecture_partition(musique,partition);
}
