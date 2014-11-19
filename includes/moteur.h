/**
 * \file def.h
 * \brief Ensemble des définitions et prototypes de fonctions / variables.
 * \author TAHRI Ahmed, SIMON Jeremy
 * \version 0.1
 * \date 27 Octobre 2014
 */

/* Pour la matrice du mot-meles */
#define X_MAX 40
#define Y_MAX 40
/* Pour la matrice du sommaire */
#define N 5
#define M 50
/* Pour la longueur maximale d'un mot */
#define MAXLEN 30
#define MAX_MOTS 25

/* Prototype du moteur du jeu */
void lire_dic(void);
void ecrire_grille(void);
int verifie_direction(int x, int y, long t);
void remplir_grille(void);
void imprime_grille(void);
int Random (int _iMin, int _iMax);

extern int NombreMot;

extern char GrilleMotMele[X_MAX][Y_MAX];
extern char Dic[MAXLEN][MAX_MOTS];

extern int x_alloc;
extern int y_alloc;

extern int size_x, size_y;