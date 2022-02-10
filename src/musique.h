#define NB_NOTES_MAX 100


#define CST1 60
#define CST2 0.05

typedef struct {
    int arr;      //Valeur de arr pour la note
    int temps;//Nombre de fois que l’on attend dureeNote
}note_t;

typedef struct{
    note_t notes[NB_NOTES_MAX];
    int nbNotes;
    int tempo;
    float dureeNote; 
    float espaceNote;
}musique_t;




void lecture_partition(musique_t *musique, char *part);


void init_musique(int tempo, musique_t *musique, char* partition);
