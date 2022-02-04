#define NB_NOTES 100

char *partition = "G3F3E3B3A3";

int ARR[12] = {59300,55972,52831,49866,47067,44425,41932,39579,37358,35261,33282,31414};
volatile uint32_t compteur = 0;

struct musique_t{
    char *partition = “C4 C#5 __ ”;
    note_t *notes;
    int tempo;
    float dureeNote = 1 / tempo * CST1; //Duree d’une note et duree des blancs
    float espaceNote = 1 / tempo * CST2; //Duree entre 2 notes CST2 < CST1
}


struct note_t{
    int arr;      //Valeur de arr pour la note
    int temps;//Nombre de fois que l’on attend dureeNote
}


int lecture_note(note_t *n);

void lecture_partition(note_t *notes, char *part);


