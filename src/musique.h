#define NB_NOTES 100

char *partition = "G3F3E3B3A3";

int ARR[12] = {59300,55972,52831,49866,47067,44425,41932,39579,37358,35261,33282,31414};
volatile uint32_t compteur = 0;

#define CST1 1
#define CST2 0.1

typedef struct {
    int arr;      //Valeur de arr pour la note
    int temps;//Nombre de fois que l’on attend dureeNote
}note_t;

typedef struct{
    char *partition;
    note_t *notes;
    int tempo;
    float dureeNote; //1 / tempo * CST1; //Duree d’une note et duree des blancs
    float espaceNote; // 1 / tempo * CST2; //Duree entre 2 notes CST2 < CST1
}musique_t;



int lecture_note(note_t *n);

void lecture_partition(note_t *notes, char *part);


