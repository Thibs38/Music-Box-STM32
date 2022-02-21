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

void button_init() {
/* Configure PB3-PB6, PB8 as inputs, no pull */
	GPIOB.MODER = GPIOB.MODER & ~(0xFF<<6 | 3<<16);
	GPIOB.PUPDR = GPIOB.PUPDR & ~(0xFF<<6 | 3<<16);
}

void button_irq_init () {
  /* set PB8 as EXTI8 input */
  SYSCFG.EXTICR3 = (SYSCFG.EXTICR3 & ~(0xf<<0)) | (0x1<<0);

  /* Setup interrupt for EXTI8, falling edge */
  EXTI.IMR  |= (1<<8);
  EXTI.RTSR &= ~(1<<8);
  EXTI.FTSR |= (1<<8);

  /* enable EXTI15_10 IRQ */
  NVIC.ISER[23/32]=(1<<(23%32)); 
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

    enable_TIM2();
    RCC.APB1ENR |= (0x1);
    
    // Configuration de CR1 Control Register 1
    
    TIM2.CR1 &= ~(0b11<<8); //CKD = 0 Clock division
    TIM2.CR1 |=  (1<<7);    //ARPE = 1 Auto-reload preload enabled
    TIM2.CR1 &= ~(0b11<<5); //CMS = 0 Edge Aligned Mode
    TIM2.CR1 &= ~(1<<4);    //DIR = 0 upcounter
    TIM2.CR1 &= ~(1<<3);    //OPM = 0 One-pulse mode
    TIM2.CR1 &= ~(1<<2);    //URS = 0 Update request source
    TIM2.CR1 &= ~(1<<1);    //UDIS = 0 Update enabled
    TIM2.CR1 &= ~(1<<0);    //CEN = 0 Counter disabled
    
    //On activera le compteur lorsque l'on en aura besoin (lecture d'une musique)
    

    
    TIM2.CCMR1 |=  (0b11<<13); //OC2M = 110 PWM mode
    TIM2.CCMR1 &= ~(1<<12); 
    TIM2.CCMR1 |=  (1<<11);    //OC2PE = 1 Output compare 2 preload enable

    TIM2.CCER |= (1<<4); //CC2E = 1 Capture/Compare 2 output enable
    
    TIM2.PSC = 10;   //On met une faible valeur au prescaler afin d'avoir de la précision dans les fréquences
    TIM2.ARR = 1000; //On initialise ARR a une valeur != 0
    TIM2.CCR2 = 0;   //On initialise CCR2 a zero -> rapport cyclique = 0
    
    TIM2.EGR |= 1; //UG = 1 Update Generation

}

void lancer_timer(){
    TIM2.CNT = 0;
    TIM2.CR1 |= 1;
}

void arreter_timer(){
    TIM2.CR1 &= ~1;
    TIM2.CNT = 0;
}

void rapport_cyclique(uint8_t pourcentage){

    if(pourcentage > 100) //On ne dépasse pas 100% du rapport cyclique
        pourcentage = 100;
    if(pourcentage == 1) //Afin d'être sur que le son se coupe
        pourcentage = 0;
    TIM2.CCR2 = TIM2.ARR - TIM2.ARR * pourcentage / 1000;
}
