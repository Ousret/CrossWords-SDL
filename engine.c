#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "include/engine.h"
#include "include/mliste.h"

// VARIABLES GLOBALES
char matrice[M][M];
char words[100][20];
int id_matched[40] = {0};
int MAX_WORDS = 0;
t_dic * allData = NULL;
long nb_words = 0;
int nb_empty_cell = M*M;

int limite_mots = 0;

// Indexation du dictionnaire
void initialiseFrom(FILE * fic) {
	if (fic == NULL) return;
	char tmp_word[100];
	long i;
	int * tmp_index = malloc(sizeof(int));
	
	memset(tmp_word, 0, sizeof(tmp_word));
	
	while (fscanf(fic,"%s",tmp_word) == 1) {
		
		//Ajout en mÈmoire
		if (allData == NULL) {
			
			allData = malloc(sizeof(t_dic));
			
		}else{
			
			allData = realloc(allData, sizeof(t_dic)*(nb_words+1));
			
		}
		
		memset(allData[nb_words].word, 0, sizeof(allData[nb_words].word));
		
		for (i = 0; i <  (int) strlen(tmp_word); i++) {
			
			allData[nb_words].word[i] = tmp_word[i];
			*tmp_index = ajouterEnFin(nb_words, tmp_word[i], i, tmp_index);
			
		}
		
		nb_words++;
	}
}

// Initialise la matrice avec des espaces
void initMatrice(void){
	int i, j;
	for(i=0 ; i<M ; i++){
		for(j=0 ; j<M ; j++){
			matrice[i][j]=' ';
		}
	}
}

//Affichage de la matrice en mode console (debug)
void displayMatrice(void){
	int i, j;
	for(i=0 ; i < MAX_WORDS ; i++){
		fprintf(stderr,"\n%s",words[i]);
	}
	fprintf(stderr,"\n");
	for(j=0 ; j<M ; j++){
		for(i=0 ; i<M ; i++){
			fprintf(stderr," %c",matrice[i][j]);
		}
		fprintf(stderr,"\n");
	}
}

// Remplis la matrice avec des mots choisis aléatoirement
// correspondant à un patron rédigé à partir d'une position
// et direction aléatoires puis remplis la matrice
void *fillMatrice(void *arg){
	int direction=-1, x, y, i=0;
	char patrontest[20];
	char * wordPTR = NULL;
	
	do {
		if(DEBUG)fprintf(stderr,"\n\n-------------\nPATTERN IN PROGRESS");
		makePattern(patrontest,&x,&y,&direction);
		if(DEBUG)fprintf(stderr,"\nPATTERN : %s",patrontest);
		wordPTR=getrandpatternMatch(patrontest);	
		if(DEBUG)fprintf(stderr,"\nWORD : %s",wordPTR);
		
		if(wordPTR!=NULL){
			if(isAllreadyIn(wordPTR) == -1){
				strcpy(words[i],wordPTR);
				writeWord(wordPTR,direction,x,y);
				if(DEBUG)fprintf(stderr,"\nREMAINING CELL(S) : %i",nb_empty_cell-NB_EMPTY_CELL_MIN);
				i++;
				MAX_WORDS++;
			}
		}
		if(DEBUG)fprintf(stderr,"\n-------------");
	}while ((nb_empty_cell > NB_EMPTY_CELL_MIN) && MAX_WORDS < limite_mots);
	
	pthread_exit(NULL);
	
}

// Remplis les espaces vides de la matrice par des lettres aléatoires
void randomFilling(void){
	int i, j;
	char charrand = (char)(rand() % 26 + 97);
	for (i=0 ; i<M ; i++){
		for (j=0 ; j<M ; j++){
			charrand = (char)(rand() % 26 + 97);
			if(matrice[i][j]==' ')matrice[i][j]=charrand;
		}
	}	
}

// Verifie si le mot n'est pas deja dans la matrice
int isAllreadyIn(char * mot){
	int i;
	
	for (i=0 ; i<MAX_WORDS ; i++){
		
		if(strcmp(words[i],mot) == 0) {
			return i;
		}
		
	}
	
	return -1;
}

// Ecrit le mot dans la matrice
void writeWord(char * mot, int direction, int x, int y){
	int i;
	int length = strlen(mot);
	for(i=0 ; i<length ; i++){
		if (direction == 0){
			if(matrice[y][x+i]==' '){
				nb_empty_cell--;
				matrice[y][x+i]=mot[i];
			}
		}
		else {
			if(matrice[y+i][x]==' '){
				nb_empty_cell--;
				matrice[y+i][x]=mot[i];
			}
		}
	}
}

// Ecrit le patron afin de trouver les mots correspondants
void makePattern(char * pattern, int * x, int * y, int * direction){
	// Gestion de la longueur du pattern à fournir pour optimiser
	// le temps d'exection de l'application
	int lene, i, max_len = 17, min_len = 11;
	if(nb_empty_cell < 250) max_len = 13, min_len = 8;
	if(nb_empty_cell < 200) max_len = 9, min_len = 5;
	if(nb_empty_cell < 150) max_len = 5, min_len = 2;
	do{
		*x = rand()%M;
		*y = rand()%M;
		*direction = rand()%2;
		if (*direction==1){
			lene = rand()%(M-*y);
		}
		else{
			lene = rand()%(M-*x);
		}
	}
	while (lene<min_len || lene>max_len || matrice[*y][*x]!=' ');
	
	// Générer le pattern à partir des mots déjà présents
	// dans la matrice 
	for (i=0 ; i<lene ; i++){
		if (!*direction) {
			if(matrice[*y][*x+i]==' ')pattern[i]='*';
			else pattern[i]=matrice[*y][*x+i];
		}
		else {
			if(matrice[*y+i][*x]==' ')pattern[i]='*';
			else pattern[i]=matrice[*y+i][*x];
		}
	}
	pattern[i]='\0';
}
