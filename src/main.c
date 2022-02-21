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

    int compteurPartition = 0; //Compte les cycles de la boucle principale pour les notes
    int compteurNote = 0;      //Compte les notes de la musique en cours
    int compteurPotar = 0;     //Compte les cycles de la boucle principale pour le potar
    int compteurMusique = 0;   //Compte la musique en cours

    uint8_t potar = 0;         //Valeur en % du potentiometre
    
     /* Initialisation */
    
    SysTick_init(1000);
    configuration_buzzer();
    configuration_leds();
    configuration_timer();
    configuration_potentiometre();
    button_init();
    button_irq_init();

    /* Efface le contenu de la console */

    //printf(CLEARCURSOR);
    //printf(CLEARSCREEN);

    /* Initialisation des musiques */

    init_musique(&musiques[0],partitions[0],180);
    init_musique(&musiques[1],partitions[1],180);
    init_musique(&musiques[2],partitions[2],600);
    init_musique(&musiques[3],partitions[3],180);

    /* Boucle principale */
    
    while(1){
        while(flag == 0)
            __WFI();
        flag = 0;

        compteurPartition++;
        compteurPotar++;

        if(compteurPotar == 20){ // Délai sur les mesures du potentiometre afin de ne pas surchager la carte
            uint32_t mesure = 0;
            mesure = mesure_potentiometre();

            potar = mesure*100 / ANALOG_RES;
            compteurPotar = 0;
        }
        

        rapport_cyclique(potar); //Changement du volume
        
        potarAvant = potar;

        //Automate: 
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
                if(musiques[compteurMusique-1].notes[compteurNote].arr > 0){ //Si ARR vaut 0 (= un blanc), alors on ne rallume pas le timer pour eviter des "claquements"
                    TIM2.ARR = musiques[compteurMusique-1].notes[compteurNote].arr;
                    rapport_cyclique(potar);
                    lancer_timer();
                }

                etat = 1;
                break;
            case 1: //On attend la duree de la note, puis on coupe le compteur
                if(compteurPartition >= musiques[compteurMusique-1].dureeNote * musiques[compteurMusique-1].notes[compteurNote].temps){ 
                    arreter_timer();
                    etat = 2;
                    compteurPartition = 0;
                }
                break;
            case 2: //On attend la duree entre 2 notes

                if(compteurPartition >= musiques[compteurMusique-1].espaceNote){
                    etat = 0;
                    compteurNote++;
                    if(compteurNote == musiques[compteurMusique-1].nbNotes)
                        compteurNote = 0;
                }
                break;
        }
    }
    return 0;
}
