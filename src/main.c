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

#define CSI "\e["
#define CLEARSCREEN "\e[2J\e[1;1H\e[?25l\r"
#define CLEARCURSOR CSI"?25l"CLEARSCREEN

#define NB_TOURS     12
#define TOTAL_TOURS NB_TOURS * 4
#define REFRESH_RATE 20
#define ANALOG_RES   4095

#define NB_NOTES 100

#define TC1   30
#define TC2   110

typedef struct{
    char nom[3];
    float duree;
    float intensite;
} note_t;



 char *partition = "G3F3E3B3A3";

 int ARR[12] = {59300,55972,52831,49866,47067,44425,41932,39579,37358,35261,33282,31414};
volatile uint32_t flag = 0;
volatile uint32_t compteur = 0;

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

void __attribute__((interrupt)) SysTick_Handler() {
    compteur++;
    if(compteur == REFRESH_RATE){
        flag = 1;
        compteur = 0;
    }
}

void configuration_potentiometre(){

    enable_GPIOB();
    GPIOB.MODER |= 0b11; //Patte PB0 en mode analogique (0b11)
    GPIOB.OTYPER &= ~0b1;
    GPIOB.OSPEEDR &= ~0b10;
    enable_ADC1();
    ADC_init_single_channel(&ADC1,8);
    ADC_set_sampling_time(&ADC1, 8, 480);
}

uint32_t mesure_potentiometre(){
    return ADC_convert_single_channel(&ADC1);
}

void configuration_leds(){

    enable_GPIOA();
    GPIOA.MODER &= 0b11111111111111110101010101111111;
    GPIOA.MODER |= 0b00000000000000000101010100000000;
}

void configuration_temperature(){

    uint32_t sequence[3] = {8,17,18};
    enable_GPIOB();
    GPIOB.MODER   |=  0b11; //Patte PB0 en mode analogique (0b11)
    GPIOB.OTYPER  &= ~0b1;
    GPIOB.OSPEEDR &= ~0b10;
    enable_ADC1();
    ADC_init_sequence(&ADC1,sequence,3);
    ADC_set_sampling_time(&ADC1,  8, 7);
    ADC_set_sampling_time(&ADC1, 17, 7);
    ADC_set_sampling_time(&ADC1, 18, 7);
}

uint32_t mesure_temperature(uint32_t* buffer){

    return ADC_convert_sequence(&ADC1,buffer);
}

void configuration_buzzer(){
    enable_GPIOB();
    GPIOB.MODER   |=  (0x2 << 18);
    //GPIOB.MODER   &= ~(1 << 18);
    GPIOB.OSPEEDR |=  (0x2 << 9);
    //GPIOB.OSPEEDR &= ~(1 << 9);
    GPIOB.AFRH    |=  (0x1 << 4);

}

void configuration_timer(){

    //clock_init();
    enable_TIM2();
    RCC.APB1ENR |= (0x1);
    TIM2.CR1 |= /*(1<<7) | */(1<<2) | 1; //ARPE | URS | CEN
    TIM2.CCMR1 |= (0x1C << 10);

    TIM2.CCER |= (1<<4);
    //TIM2.EGR |= 1;

    TIM2.PSC = 10;
    TIM2.ARR = 0; //
    TIM2.CCR2 = 0;
}

//Met le rapport cyclique a un certain pourcentage
void rapport_cyclique(uint8_t pourcentage){

    if(pourcentage > 100)
        pourcentage = 100;
    if(pourcentage == 1)
        pourcentage = 0;
    TIM2.CCR2 = TIM2.ARR - TIM2.ARR * pourcentage / TIM2.ARR;
}

int main() {

/*printf("\e[2J\e[1;1H\e[?25l\r\n");
  printf("*** Welcome to Nucleo F446 ! ***\r\n");
  printf("   %08lx-%08lx-%08lx\r\n",U_ID[0],U_ID[1],U_ID[2]);
  printf("SYSCLK = %9lu Hz\r\n",get_SYSCLK());
  printf("AHBCLK = %9lu Hz\r\n",get_AHBCLK());
  printf("APB1CLK =%9lu Hz\r\n",get_APB1CLK());
  printf("APB2CLK =%9lu Hz\r\n",get_APB2CLK());*/
    int n = 0;
    int potarAvant = 0;

    int compteurPartition = 0;
    int compteurNote = 0;
    SysTick_init(1000);
    configuration_temperature();
    configuration_leds();
    configuration_buzzer();
    configuration_timer();
    enable_Vrefint();
    printf(CLEARCURSOR);
    printf(CLEARSCREEN);
    printf("\rpotar | VDDA  | temp \r\n");


    note_t notes[NB_NOTES];
    lecture_partition(notes,partition);

    while(1){
        while(flag == 0)
            __WFI();
        compteurPartition++;
        uint32_t mesures[3];
        mesure_temperature(mesures);
        uint8_t potar = mesures[0]*100 / ANALOG_RES;
        float VDDA = VREFIN_CAL * 3.3f / mesures[1];
        float VALC1 = TS_CAL1 * 3.3f / ANALOG_RES;
        float VALC2 = TS_CAL2 * 3.3f / ANALOG_RES;
        float VALTS = mesures[2] * VDDA / ANALOG_RES;
        float temp = (TC2-TC1)*(VALTS-VALC1)/(VALC2-VALC1) + TC1;
        //printf("\r%3d%%  | %3.2fV | %3.2f°C | TIM2.CCR2 %lu", potar, ftod(VDDA), ftod(temp), TIM2.CCR2);
        n = (mesures[0] * TOTAL_TOURS / ANALOG_RES) % 4;

        GPIOA.ODR = 1 << (n + 4);

        if(potarAvant != potar)
        rapport_cyclique(potar);

        potarAvant = potar;

        if(compteurPartition >= 20 && compteurNote < NB_NOTES){
            compteurPartition = 0;
            TIM2.ARR = lecture_note(&notes[compteurNote]);
            printf(" %lu\r\n",TIM2.ARR);

            rapport_cyclique(potar);
            compteurNote++;
        }

        flag = 0;
    }

return 0;
}
