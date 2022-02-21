#include <stdint.h>

#define REFRESH_GAP 10
#define ANALOG_RES   4095


/* Active le potentiometre */
void configuration_potentiometre();

/* Réalise une mesure avec le potentiometre */
uint32_t mesure_potentiometre();

/* Active le buzzer */
void configuration_buzzer();

/* Active le timer */
void configuration_timer();

/* Active le carré de leds */
void configuration_leds();

/* Lance le compteur */
void lancer_timer();

/* Coupe le compteur */
void arreter_timer();

/* Active le bouton utilisateur de la carte fille */
void button_init();

/* Active l'interruption sur le bouton utilisateur de la carte fille' */
void button_irq_init();

/* Applique le rapport cyclique en parametre sur le timer */
void rapport_cyclique(uint8_t pourcentage);
