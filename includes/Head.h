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
#define N 10
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

void SPI_SDL_Init_Video(int x, int y, char titre[100], int ttf_support, int audio_support);
int SDL_Create_Menu(TTF_Font *police, int nb_entre, char sommaire[N][M]);
void SDL_Create_Menu_Ch(TTF_Font *police, int id, char titre[M], int x, int y);
void SDL_Print_Btn(int id,TTF_Font *police, char titre[30], int x, int y);
void SDL_Print_bg(char file[50]);
void SDL_Print_popup();
int SDL_Souris_Survol(int hauteur, int largeur, int x, int y);
void SDL_Write_popup(int ligne, TTF_Font *police, char txt_ligne1[200], char txt_ligne2[200], char txt_ligne3[200]);
void SDL_Open_PopUp(int ligne, TTF_Font *police, char txt_ligne1[200], char txt_ligne2[200], char txt_ligne3[200]);

extern int NombreMot;

extern char GrilleMotMele[X_MAX][Y_MAX];
extern char Dic[MAXLEN][MAX_MOTS];

extern int x_alloc;
extern int y_alloc;

extern int size_x, size_y;

/* Variable pointeur pour SDL */
extern SDL_Surface *screen;

extern SDL_Color couleurRouge;
extern SDL_Color couleurBlanche;
extern SDL_Color couleurNoire;

extern SDL_Event GlobalEvent;
/* Fin pointeur SDL */

/* Variable pour le mixage */
extern int channel;	
extern Mix_Chunk *sound;

//SDL_mutex *MutexEcran = NULL; Inutile pour l'instant.. Faut s'en passer.. :D
//extern FILE * fichier_db;

extern int sel_menu_m; //Indice du choix (menu) survolé à un moment t.