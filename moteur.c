/**
 * \file moteur.c
 * \brief Moteur du Mot-mêlés en C
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.4
 * \date 27 Octobre 2014
 *
 * Toutes les fonctions utiles à la génération de la grille
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Gestion des chaines
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h> //Pour imprimer du texte graphiquement
#include <SDL_image.h> //Pour imprimer des images
#include "includes/Head.h"

/**
 * \fn void remplir_grille(void)
 * \brief Fonction qui complete la matrice si besoin est de lettres choisies au hasard 
 *
 * \return Ne retourne rien
 */
void remplir_grille(void) {
	
	char RandLetter = 0; //97 à 122
	int i = 0, j = 0;
	
	for (i = 0; i < 20; i++) {
		for (j = 0; j < 20; j++) {
			
			RandLetter = Random (97,122); //Lettre aléatoire
			
			if (GrilleMotMele[j][i] < 'a' || GrilleMotMele[j][i] > 'z') {

				GrilleMotMele[j][i] = RandLetter;
				
			}
			
		}
	}
}

/**
 * \fn void ecrire_grille(void)
 * \brief Fonction qui place les mots dans la matrice
 *
 * \return Ne retourne rien
 */
void ecrire_grille(void) {
	
	int i = 0, j = 0;
	int RandX = 0, RandY = 0, NbMots = 0;

	
	int Direction = 0; //1 Droite; 2 Gauche; 3 Haut; 4 Bas; 5 DiagBas_0; 6 DiagHaut_0
	
	for (i = 0; i < NombreMot; i ++) {
		
		srand (time (NULL)); 

		RandX = Random (0, x_alloc); //Pos X aléatoire
		RandY = Random (0, y_alloc); //Pos Y aléatoire
		
		// On décide de la direction à prendre
		Direction = verifie_direction(RandX,RandY,strlen(Dic[i]));
		
		switch (Direction) {
			case 1:
			
				for (j = 0; j < strlen(Dic[i]); j++) {
					
					GrilleMotMele[RandX+j][RandY] = Dic[i][j]; 

				}
				break;
			case 2:
			
				for (j = 0; j < strlen(Dic[i]); j++) {
					
					GrilleMotMele[RandX-j][RandY] = Dic[i][j];

				}
				break;
			case 3:
			
				for (j = 0; j < strlen(Dic[i]); j++) {
					
					GrilleMotMele[RandX][RandY+j] = Dic[i][j];

				}
				break;
			case 4:
			
				for (j = 0; j < strlen(Dic[i]); j++) {
					
					GrilleMotMele[RandX][RandY-j] = Dic[i][j];

				}
				break;
			case 5:
				
				for (j = 0; j < strlen(Dic[i]); j++) {
				
					GrilleMotMele[RandX][RandY] = Dic[i][j];
					RandX++;
					RandY++;
					
				}
				
				break;
			case 6:
				
				for (j = 0; j < strlen(Dic[i]); j++) {
				
					GrilleMotMele[RandX][RandY] = Dic[i][j];
					RandX--;
					RandY--;
					
				}
				
				break;
			default:
				
				break;
		}
	}
}

int verifie_direction(int x, int y, long t) {
	
	int i = 0, Direction = -1;
	int Droite = 0, Gauche = 0, Haut = 0, Bas = 0, DiagBas_0 = 0, DiagHaut_0 = 0, DiagBas_1 = 0, DiagHaut_1 = 0;
	int Force = 0; //Variable compteur pour éviter de chercher indéfiniment
	
	int x_test = 0, y_test = 0;
	
	//Test si possibilité de passer à droite
	if (x+t < (x_alloc-1)) {
		
		for (i = 0; i < t; i++) {
		
			if (GrilleMotMele[x+i][y] != 0) {
				Droite = 0;
				break;
			}
			
		Droite = 1;
		}
		
		
	}

	
	//Test si possibilité de passer à gauche
	if (x-t > -1) {
		for (i = 0; i < t; i++) {
			if (GrilleMotMele[x-i][y] != 0) {
				Gauche = 0;
				break;
			}
			Gauche = 1;
		}
	}
	

	//Test si possibilité de passer en haut
	if (y+t < (y_alloc-1)) {
		for (i = 0; i < t; i++) {
			if (GrilleMotMele[x][y+i] != 0) {
				Haut = 0;
				break;
			}
			Haut = 1;
		}
	}
	
	//Test si possibilité de passer en bas
	if (y-t > -1) {
		for (i = 0; i < t; i++) {
			
			if (GrilleMotMele[x][y-i] != 0) {
				Bas = 0;
				break;
			}
			
			Bas = 1;
		}
	}
	
	//Test si possibilité de passer diagonale haut vers bas (1)
	if ( (x-t < (x_alloc-1)) && (y-t > -1) ) {

		x_test = x;
		y_test = y;
		
		for (i = 0; i < t; i++) {
			
			if (GrilleMotMele[x_test][y_test] != 0) {
				DiagBas_0 = 0;
				break;
			}
			
			x_test++;
			y_test++;
			
		}
		
		DiagBas_0 = 1;
		
	}
	
	//Test si possibilité de passer diagonale bas vers haut (1)
	if ( (x-t > -1) && (y-t < (y_alloc-1) )) {

		x_test = x;
		y_test = y;
		
		for (i = 0; i < t; i++) {
			
			if (GrilleMotMele[x_test][y_test] != 0) {
				DiagHaut_0 = 0;
				break;
			}
			
			x_test--;
			y_test--;
			
		}
		
		DiagHaut_0 = 1;
		
	}
	
	//On choisi au hasard si en haut, bas, droite ou gauche.
	
	while (Force < 10) {
		
		srand (time (NULL)); 
		Direction = Random(1,6);
	
		switch (Direction) {
			case 1:
				if (Droite == 1) {
					return Direction;
				}
				break;
			case 2:
				if (Gauche == 1) {
					return Direction;
				}
				break;
			case 3:
				if (Haut == 1) {
					return Direction;
				}
				break;
			case 4:
				if (Bas == 1) {
					return Direction;
				}
				break;
			case 5:
				if (DiagBas_0 == 1) {
					return Direction;
				}
				break;
			case 6:
				if (DiagHaut_0 == 1) {
					return Direction;
				}
				break;
			default:
				break;
		}
		
		Force++; //On évite de boucler à l'infinie
	}

	return -1;
	
}

void lire_dic(void) {
   
   static const char filename[] = "fr.dic";
   FILE *file = fopen ( filename, "r" );
   
   if ( file != NULL )
   {
      	char line [ 128 ]; 
 		int NbMots = 0,CPURand=0,i = 0;
 
      	while ((fgets ( line, sizeof line, file ) != NULL) && ( NbMots < 10)) /* read a line */
      	{
         	srand (time (NULL)); 
         	CPURand = Random(0,2);
         	
         	if (CPURand == 1) {

				strcpy(Dic[NbMots], line);
         		
         		NbMots++;
         	}
         	
         	memset(line, 0, sizeof(line)); //On formatte la chaine de lecture de ligne fichier
			
      	}
      	
		fclose ( file );
   }
   else
   {
      perror ( filename ); 
   }
   
   return;
}

int Random (int _iMin, int _iMax)
{
	//srand (time (NULL)); 
	return (_iMin + (rand () % (_iMax-_iMin+1)));
} 