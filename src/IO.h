
#define REFRESH_GAP 10
#define ANALOG_RES   4095


void configuration_potentiometre();

uint32_t mesure_potentiometre();

void configuration_buzzer();

void configuration_timer();
void configuration_leds();

void lancer_timer();
void arreter_timer();

void button_init();
void button_irq_init();
uint32_t button_poll();


void rapport_cyclique(uint8_t pourcentage);
