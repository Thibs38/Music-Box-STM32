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
