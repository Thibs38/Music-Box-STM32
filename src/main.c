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

#define REFRESH_RATE 10
#define ANALOG_RES   4095




char *partition = "C31 D31 E31 F31 G31 A31 B31";
char *partition2 = "C41 D41 E41 F41 G41 A41 B41";
char *partition3 = "C51 D51 E51 F51 G51 A51 B51";
char *partition4 = "C61 D61 E61 F61 G61 A61 B61";


volatile uint32_t flag = 0;
volatile uint32_t compteur = 0;
volatile uint32_t compteurMusique = 0;


void __attribute__((interrupt)) SysTick_Handler() {
    compteur++;
    if(compteur == REFRESH_RATE){
        flag = 1;
        compteur = 0;
    }
}

void __attribute__((interrupt)) EXTI9_5_Handler(){
	EXTI.PR |=(1<<8);
        //printf("\nD = %5lu",TIM2.ARR);
    compteurMusique++;
    if(compteurMusique == 5)
        compteurMusique = 0;
    TIM2.ARR = 0;
}


int main() {

    int potarAvant = 0;

    int compteurPartition = 0;
    int compteurNote = 0;
    int compteurPotar = 0;
    
    musique_t musique;
    musique.tempo = 180;
    musique.dureeNote =  60 * 1000 / musique.tempo / REFRESH_RATE;
    musique.espaceNote = 60 * 200 / musique.tempo / REFRESH_RATE;    
    musique.nbNotes = 7;
    
    SysTick_init(1000);
    configuration_leds();
    configuration_timer();
    configuration_buzzer();
    configuration_potentiometre();
    button_init();
    button_irq_init();
    printf(CLEARCURSOR);
    printf(CLEARSCREEN);
    //printf("\rpotar \r\n");


    lecture_partition(&musique,partition);
    lancer_timer();
    int etat = 0; //0: change note, 1: attend, 2: joue un blanc
    uint8_t potar = 0;
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
        
        //printf("\rpotar: %3d%%  ", mesure);

        //GPIOA.ODR = 1 << (n + 4);

        //if(potarAvant != potar)
        rapport_cyclique(potar);
        
        potarAvant = potar;
        //printf("%d",compteurPartition);

        if(compteurMusique == 0)
            continue;
        printf("%lu\r\n",TIM2.CNT);
        switch(etat){
            
            case 0:
                compteurPartition = 0;
                //rapport_cyclique(potar);
                TIM2.ARR = musique.notes[compteurNote].arr;
                //printf(" %d = %lu\r\n",compteurNote,musique.notes[compteurNote].arr);
                                    //printf("Note: %d -> %lu\r\n",compteurNote,musique.notes[compteurNote].arr);


                etat = 1;
                break;
            case 1:
                //printf("Etat 1 \r\n");
                //printf("E 1 n: %d %d\r\n",TIM2.CCR2, TIM2.ARR);
                if(compteurPartition >= musique.dureeNote * musique.notes[compteurNote].temps){ 
                    etat = 2;
                    TIM2.ARR = 0;             //Le claquement se produit ici
                    compteurPartition = 0;
                    //printf("Etat 1 -> 2\r");
                }
                break;
            case 2:

                //printf("Etat 2 compteur: %d espaceNote: %f\r\n", compteurPartition,musique.espaceNote);
                if(compteurPartition >= musique.espaceNote){
                    etat = 0;
                    compteurNote++;
                    if(compteurNote == musique.nbNotes)
                        compteurNote = 0;
                    //printf("Etat 2 -> 0 \r");
                }
                break;
        }


    }

return 0;
}
