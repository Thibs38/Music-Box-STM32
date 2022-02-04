#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "sys/cm4.h"
#include "sys/devices.h"
#include "sys/init.h"
#include "sys/clock.h"
#include "systick.h"
#include "adc.h"
#include "musique.h"

//Retourne ARR qui correspond à la note
int lecture_note(note_t *n){
    printf("%c%c%c ",n->nom[0],n->nom[1],n->nom[2]);
    if(n->nom[0] == '_')
        return 0;
    if(n->nom[0] == 'C'){
        if(n->nom[1] == '#')
            return ARR[1] >> (n->nom[2] - '0' - 2);
        return ARR[0] >> (n->nom[1] - '0' - 2);
    }
    if(n->nom[0] == 'D'){
        if(n->nom[1] == '#')
            return ARR[3] >> (n->nom[2] - '0' - 2);
        if(n->nom[1] == 'b')
            return ARR[1] >> (n->nom[2] - '0' - 2);
        return ARR[2] >> (n->nom[1] - '0' - 2);
    }
    if(n->nom[0] == 'E'){
    	if(n->nom[1] == 'b')
    	    return ARR[1] >> (n->nom[2] - '0' - 2); 
        return ARR[4] >> (n->nom[1] - '0' - 2);
    }
    if(n->nom[0] == 'F'){
        if(n->nom[1] == '#')
            return ARR[6] >> (n->nom[2] - '0' - 2);
        return ARR[5] >> (n->nom[1] - '0' - 2);
    }
    if(n->nom[0] == 'G'){
        if(n->nom[1] == '#')
            return ARR[8] >> (n->nom[2] - '0' - 2);
        if(n->nom[1] == 'b')
            return ARR[6] >> (n->nom[2] - '0' - 2);
        return ARR[7] >> (n->nom[1] - '0' - 2);
    }
    if(n->nom[0] == 'A'){
        if(n->nom[1] == '#')
            return ARR[10] >> (n->nom[2] - '0' - 2);
        if(n->nom[1] == 'b')
            return ARR[8] >> (n->nom[2] - '0' - 2);
        return ARR[9] >> (n->nom[1] - '0' - 2);
    }
    if(n->nom[0] == 'B'){
        if(n->nom[1] == 'b')
            return ARR[10] >> (n->nom[2] - '0' - 2);
        return ARR[11] >> (n->nom[1] - '0' - 2);
    }
}

void lecture_partition(note_t *notes, char *part){
    int i = 0;
    int j = 0;
    char c1,c2,c3;
    while(part[i] != 0){
        //printf("%c%c", part[i],part[i+1]);

        c1 = part[i];
        i++;
        c2 = part[i];
        i++;
        notes[j].nom[0] = c1;
        notes[j].nom[1] = c2;
        if(c2 == '#'){
            //printf("%c", part[i]);

            c3 = part[i];
            i++;
            notes[j].nom[2] = c3;
        }
        //printf("\r\n");
        j++;
    }
}
