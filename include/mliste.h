/*
	En-tête de mliste.c
*/

#define ERREUR -1
#define MAX_TAMPON 100000

typedef struct {
	
	int wordid;
	int * pos;
	int nb_occ;
	

} t_index;

typedef struct {
	
	char letter;
	t_index * index;
	int indexSize;
	

} t_alphabet;

typedef struct {
	
	char word[100];
	

} t_dic;

extern int nb_unique_char;
extern t_dic * allData;
extern long nb_words;

int ajouterEnFin(int wordid, char letter, int pos, int * indexId);
char * rechercherElement(char * pattern, int indice);
void effacerListe();
int isrowExist(char letter);
int rowCreate(char letter);
int rowId(char letter);
char * getrandpatternMatch(char * pattern);
void flushIndex();