// Mettre DEBUG Ã  1 pour activer les messages d'erreur en console
#define DEBUG 0
#define M 20
#define MAX_LEN 128
#define NB_EMPTY_CELL_MIN 50

extern char matrice[M][M];
extern char words[100][20];
extern int MAX_WORDS;
extern int nb_empty_cell;
extern int limite_mots;

// DECLARATIONS FONCTIONS
void initMatrice(void);
void displayMatrice(void);
void fillMatrice(void);
void makePattern(char * pattern, int * x, int * y, int * direction);
void initialiseFrom(FILE * fic);
void writeWord(char * mot, int direction, int x, int y);
void randomFilling(void);
int isAllreadyIn(char * mot);