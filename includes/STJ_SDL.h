/**
 * \file STJ_SDL.h
 * \brief Ensemble des définitions et prototypes de fonctions / variables.
 * \author TAHRI Ahmed, SIMON Jeremy
 * \version 0.1
 * \date 27 Octobre 2014
 */
#define N 10
#define M 50

void SPI_SDL_Init_Video(int x, int y, char titre[100], int ttf_support, int audio_support);
int SDL_Create_Menu(TTF_Font *police, int nb_entre, char sommaire[N][M]);
void SDL_Create_Menu_Ch(TTF_Font *police, int id, char titre[M], int x, int y);
void SDL_Print_Btn(int id,TTF_Font *police, char titre[30], int x, int y);
void SDL_Print_bg(char file[50], int x, int y);
void SDL_Print_popup();
int SDL_Souris_Survol(int hauteur, int largeur, int x, int y);
void SDL_Write_popup(int ligne, TTF_Font *police, char txt_ligne1[200], char txt_ligne2[200], char txt_ligne3[200]);
void SDL_Open_PopUp(int ligne, TTF_Font *police, char txt_ligne1[200], char txt_ligne2[200], char txt_ligne3[200]);

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