/**
 * \file main.c
 * \brief Mot-mêlés - Projet Info SPI L2
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.1
 * \date 29 Octobre 2014
 *
 * Générateur de mot-meles avec la SDL/C 1.2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h> //Pour imprimer du texte graphiquement
#include <SDL_image.h> //Pour imprimer des images
#include <string.h> //Gestion des chaines

#include "includes/Head.h"

SDL_Surface *screen = NULL;

SDL_Color couleurRouge = {255, 0, 0};
SDL_Color couleurBlanche = {255, 255, 255};
SDL_Color couleurNoire = {0, 0, 0};

SDL_Event GlobalEvent;

int channel = 0;	
Mix_Chunk *sound = NULL;

int NombreMot = 0;

char GrilleMotMele[X_MAX][Y_MAX];
char Dic[MAXLEN][MAX_MOTS];

int x_alloc = 0;
int y_alloc = 0;

int size_x = 0, size_y = 0;

//SDL_mutex *MutexEcran = NULL; Inutile pour l'instant.
//FILE * fichier_db = NULL;

int sel_menu_m = 0;

/**
 * \fn int main (int argc, char *argv[])
 * \brief Entrée du programme.
 *
 * \return 0 - Arrêt normal du programme.
 */
int main (int argc, char *argv[]) {

	char sommaire[N][M] = { "Partie locale", "Partie internet", "Statistiques", "Quitter" };

	TTF_Font *police_menu = NULL; //Police d'Ècriture pour le menu 
	TTF_Font *police_std = NULL; //Police pour tout le reste sauf menu
	
	int MenuChoix = 0;

	SPI_SDL_Init_Video(800,600,"SPITaJoke SDL",1,1); //800x600 +tff_support +audio_support

	//SDL_Splash("ressources/Splash0.png", 1500);
	//SDL_Splash("ressources/Splash1.png", 1500);

	police_menu = TTF_OpenFont("ressources/ttf/global.ttf", 30);
	police_std = TTF_OpenFont("ressources/ttf/global.ttf", 20);

	while (1) {
		
		MenuChoix = SDL_Create_Menu(police_menu, 4, sommaire); //On affiche un menu de cinq entrÈe
		
		switch (MenuChoix) {
			
			case 0: 
				//Contre CPU ou SOLO
				break;
				
			case 1:
				//Multijoueur
				break;
			
			case 2:
				//Statistiques joueur
				break;
			
			case 3:
				exit(0);
				break;
				
			default:
				break;
				
		}
		
	}

}