#define NB_NOTES_MAX 100
#define NB_CHAR_MAX NB_NOTES_MAX * 5
#define NB_MUSIQUES 4


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


char partitions[NB_MUSIQUES][NB_CHAR_MAX];
musique_t musiques[NB_MUSIQUES];

void lecture_partition(musique_t *musique, char *part);


void init_musique( musique_t *musique, char* partition,int tempo);
