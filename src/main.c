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

#include "IO.h"
#include "musique.h"

#define CSI "\e["
#define CLEARSCREEN "\e[2J\e[1;1H\e[?25l\r"
#define CLEARCURSOR CSI"?25l"CLEARSCREEN




volatile uint32_t flag = 0;
volatile uint32_t compteur = 0;
volatile uint32_t etat = 0; //-1: initialise la musique suivante 0: change note, 1: attend, 2: joue un blanc


void __attribute__((interrupt)) SysTick_Handler() {
    compteur++;
    if(compteur == REFRESH_GAP){
        flag = 1;
        compteur = 0;
    }
}

void __attribute__((interrupt)) EXTI9_5_Handler(){
	EXTI.PR |=(1<<8);
    etat = -1; //On se met dans l'état d'initialisation
}


int main() {

    int potarAvant = 0;

    int compteurPartition = 0; //Compte les cycles de la boucle principale pour les notes
    int compteurNote = 0;      //Compte les notes de la musique en cours
    int compteurPotar = 0;     //Compte les cycles de la boucle principale pour le potar
    int compteurMusique = 0;   //Compte la musique en cours

    uint8_t potar = 0;
    
     
    
    SysTick_init(1000);
    configuration_buzzer();
    configuration_leds();
    configuration_timer();
    configuration_potentiometre();
    button_init();
    button_irq_init();
    printf(CLEARCURSOR);
    printf(CLEARSCREEN);

    init_musique(&musiques[0],partitions[0],180);
    init_musique(&musiques[1],partitions[1],180);
    init_musique(&musiques[2],partitions[2],180);
    init_musique(&musiques[3],partitions[3],180);

    
    
    while(1){
        while(flag == 0)
            __WFI();
        flag = 0;

        compteurPartition++;
        compteurPotar++;
        if(compteurPotar == 20){
            uint32_t mesure = 0;
            mesure = mesure_potentiometre();

            potar = mesure*100 / ANALOG_RES;
            compteurPotar = 0;
        }
        
        //printf("\rpotar: %3d%%  ", potar);

        //GPIOA.ODR = 1 << (n + 4);

        //if(potarAvant != potar)
        rapport_cyclique(potar);
        
        potarAvant = potar;
        //printf("%d",compteurPartition);


        //printf("%lu\r\n",TIM2.CNT);
        switch(etat){
            case -1:  //Initialisation de la prochaine musique
                arreter_timer();
                compteurMusique++;
                if(compteurMusique == 5)
                    compteurMusique = 0;
                GPIOA.ODR = 1 << compteurMusique + 3;
                compteurNote = 0;
                etat = 0;
                break;
            case 0: //On met la note suivante et on allume le compteur
                if(compteurMusique == 0)
                    continue;
                compteurPartition = 0;
                rapport_cyclique(potar);
                if(musiques[compteurMusique-1].notes[compteurNote].arr > 0){ //Si ARR vaut 0 (= un blanc), alors on ne rallume pas le timer pour eviter des "claquements"
                    TIM2.ARR = musiques[compteurMusique-1].notes[compteurNote].arr;
                    lancer_timer();
                }
                printf(" %d = %lu\r\n",compteurNote,musiques[compteurMusique-1].notes[compteurNote].arr);
                                    //printf("Note: %d -> %lu\r\n",compteurNote,musique.notes[compteurNote].arr);


                etat = 1;
                break;
            case 1: //On attend la duree de la note, puis on coupe le compteur
                //printf("Etat 1 \r\n");
                //printf("E 1 n: %d %d\r\n",TIM2.CCR2, TIM2.ARR);
                if(compteurPartition >= musiques[compteurMusique-1].dureeNote * musiques[compteurMusique-1].notes[compteurNote].temps){ 
                    arreter_timer();
                    etat = 2;
                    compteurPartition = 0;
                    //printf("Etat 1 -> 2\r");
                }
                break;
            case 2: //On attend la duree entre 2 notes

                //printf("Etat 2 compteur: %d espaceNote: %f\r\n", compteurPartition,musique.espaceNote);
                if(compteurPartition >= musiques[compteurMusique-1].espaceNote){
                    etat = 0;
                    compteurNote++;
                    if(compteurNote == musiques[compteurMusique-1].nbNotes)
                        compteurNote = 0;
                    //printf("Etat 2 -> 0 \r");
                }
                break;
        }


    }

return 0;
}
