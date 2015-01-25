#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <ESDL.h>

#include "include/engine.h"
#include "include/mliste.h"
#include "include/save.h"

#define DIR_UP 4
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

int EXP_J1 = 0, LVL_J1 = 1;
d_save *savegame = NULL;

void generateIndex() {
	
	char msg[200];
	memset(msg, 0, sizeof(msg));
	
	/* Load new context */
	t_context * loading = SDL_newContext("CrossWords SDL", 0, 0, 800, 600);
	SDL_newImage(loading, NULL, "APP_BG_LOAD.png", 0, 0);
	SDL_newText(loading, NULL, msg, colorBlack, 50, 550);
	
	/* Opening file that contain all words */
	FILE * fic = NULL;
	fic=fopen("fr.dic","r");
	
	if (!fic) exit(0); //If file does not exit, just quit..
	
	sprintf(msg,"Lecture du dictionnaire..");
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	
	initialiseFrom(fic);
	sprintf(msg, "Dictionnaire pret a l'emploi..");
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	
	fclose(fic);
	SDL_freeContext(loading);
	
}

void generateGrille() {
	
	char msg[200];
	pthread_t thread_fillMatrice;
	
	memset(msg, 0, sizeof(msg));
	t_context * loading = SDL_newContext("CrossWords SDL", 0, 0, 800, 600);
	SDL_newImage(loading, NULL, "APP_BG_LOAD.png", 0, 0);
	SDL_newText(loading, NULL, msg, colorBlack, 50, 550);
	/* Init var for engine.c */
	nb_empty_cell = M*M;
	MAX_WORDS=0;
	memset(words, 0, sizeof(words));
	memset(id_matched, 0, sizeof(id_matched));
	
	// Matrice
	sprintf(msg, "Preparation de la table de mot-meles..");
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	
	initMatrice();
	
	sprintf(msg, "Table preparee !");
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	
	// Moteur
	sprintf(msg, "Generation d'une nouvelle grille, %i mot(s) sur %i places..", MAX_WORDS, limite_mots);
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	
	if (pthread_create(&thread_fillMatrice, NULL, fillMatrice, NULL)) {
		perror("pthread_create");
		return;
    }
		
	while (MAX_WORDS < limite_mots) {
		
		sprintf(msg, "Generation d'une nouvelle grille, %i mot(s) sur %i places..", MAX_WORDS, limite_mots);
		SDL_modText(loading, 0, msg, colorBlack, 50, 550);
		SDL_generate(loading);
		
		SDL_Delay(100);
		
	}
	
	randomFilling();
	
	sprintf(msg, "Chargement..");
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	
	SDL_freeContext(loading);
	
}

int pause() {
	int choix = 0;
	t_context * popup = SDL_newContext("Mon compte", 200, 200, 500, 250);
			
	SDL_newImage(popup, NULL, "popup.png", 0, 0);
	SDL_newText(popup, NULL, "PAUSE", colorWhite, 100, 100);
	
	SDL_newObj(popup, NULL, 0, "Continuer", ALIGN_CENTER, NULL, NONE, 10, 200);
	SDL_newObj(popup, NULL, 0, "Menu", ALIGN_CENTER, NULL , NONE, 250, 200);
			
	choix = SDL_generate(popup);
	SDL_freeContext(popup);	
	
	return choix;
}

void ingame() {
	
	t_context * ingame =  SDL_newContext("CrossWords SDL", 0, 0, 800, 600);
	int posX = 40, posY = 80, i = 0, j = 0, update = 1, animate = 1, x_tmp = 85, y_tmp = 40;
	char XP_HUD[100];
	int pos_relative_x = 0, pos_x_tmp = 0, pos_y_tmp = 0, pos_relative_y = 0, id_mod = 0;
	
	int nb_word_remain = MAX_WORDS;
	
	char select[399];
	int select_mode = 0, id_match = 0;
	int HUD = 0;
	char tmp[5];
	tmp[1] = '\0';
	
	memset(select, 0, sizeof(select));
	memset(XP_HUD, 0, sizeof(XP_HUD));
	memset(id_matched, -1, sizeof(id_matched));
	
	SDL_newImage(ingame, NULL, "APP_INGAME.png", 0, 0);
	SDL_newSprite(ingame, "panda_sp.png", colorGreenLight, 36, 33, posX, posY, DIR_DOWN, 1, 0);
	SDL_loadSound("xpup.wav");
	
	//510x 40y
	for (i = 0; i < M; i++) {
		
		for (j = 0; j < M; j++) {
			tmp[0] = matrice[j][i];
			SDL_newText(ingame, NULL, tmp, colorWhite, x_tmp, y_tmp);
			x_tmp+=20;
		}
		
		y_tmp += 20;
		x_tmp = 85;
	}
	
	for (i = 0; i < MAX_WORDS; i++) {
		
		if (i >= 20) {
			SDL_newText(ingame, NULL, words[i], colorWhite, 670, 40+(20*(i-20)));
		}else{
			SDL_newText(ingame, NULL, words[i], colorWhite, 510, 40+(20*i));
		}
		
	}
	
	SDL_newText(ingame, &HUD, "No Space !", colorWhite, 50, 550);
	sprintf(XP_HUD, "%i d'XP", EXP_J1);
	SDL_newText(ingame, NULL, XP_HUD, colorWhite, 100, 580);
	
	while (1) {
		
		SDL_UpdateEvents(&in); //Manual update event mapping, SDL_generate do update mapping too..
		
		if (SDL_isKeyPressed(SDLK_ESCAPE)) {
			if (pause() == 1) {
				SDL_freeContext(ingame);
				return;
			}else{
				update = 1;
			}
		}
		
		if (SDL_isKeyPressed(SDLK_SPACE)) {
			
			select_mode = 1;
			SDL_modText(ingame, HUD, "SPACE PRESSED !", colorRed, -1, -1);
			update = 1;
			
		}else{
			
			if (select_mode == 1) {
			
				SDL_modText(ingame, HUD, "SPACE RELEASED !", colorWhite, -1, -1);
				select_mode = 0;
				
				id_match = isAllreadyIn(select);
				
				//-nb_word_remain
				for (i = 0; i < MAX_WORDS; i++) {
						if (id_matched[i] == id_match) break;
				}
				
				if ((id_match >= 0) && (id_match != id_matched[i])) {
					
					SDL_modText(ingame, 400+id_match, words[id_match], colorRed, -1, -1);
					SDL_playSound("xpup.wav");
					id_matched[MAX_WORDS-nb_word_remain] = id_match;
					
					nb_word_remain--;
					EXP_J1+=3;
					sprintf(XP_HUD, "%i d'XP", EXP_J1);
					SDL_modText(ingame, HUD+1, XP_HUD, colorWhite, -1, -1);
					if (nb_word_remain <= 0) {
						SDL_freeContext(ingame);
						return;	
					}else{
						update = 1;
					} 
				}else{
					update = 1;
				}
				
				for (i = 0; i < M; i++) {
		
					for (j = 0; j < M; j++) {
						tmp[0] = matrice[j][i];
						SDL_modText(ingame, (i*20)+j ,tmp, colorWhite, -1, -1);
					}
	
				}
				
				memset(select, 0, sizeof(select));
				
			}
		}
		
		if (SDL_isKeyPressed(SDLK_UP)) {
			if (posY > 5) {
				posY-=5;
				update = 1;
			}
			SDL_modSprite(ingame, 0, posX, posY, DIR_UP, animate, 0);
			animate++;
		}
		
		if (SDL_isKeyPressed(SDLK_DOWN)) {
			if (posY < 540) {
				posY+=5;
				update = 1;
			}
			SDL_modSprite(ingame, 0, posX, posY, DIR_DOWN, animate, 0);
			animate++;
		}
		
		if (SDL_isKeyPressed(SDLK_LEFT)) {
			if (posX > 5) {
				posX-=5;
				update = 1;
			}
			SDL_modSprite(ingame, 0, posX, posY, DIR_LEFT, animate, 0);
			animate++;
		}
		
		if (SDL_isKeyPressed(SDLK_RIGHT)) {
			if (posX < 780) {
				posX+=5;
				update = 1;
			}
			SDL_modSprite(ingame, 0, posX, posY, DIR_RIGHT, animate, 0);
			animate++;
		}
		
		if (select_mode == 1) {
			
			pos_x_tmp = pos_relative_x;
			pos_y_tmp = pos_relative_y;
		
			pos_relative_x = (posX - 75)/20;
			pos_relative_y = ((posY+33) - 50)/20;
			
		}
		
		if (pos_relative_x < M && pos_relative_y < M && select_mode == 1) {
			
			id_mod = (pos_relative_y*20) + pos_relative_x;
			tmp[0] = matrice[pos_relative_x][pos_relative_y];
			if (pos_x_tmp != pos_relative_x || pos_y_tmp != pos_relative_y) strcat (select, tmp);
			SDL_modText(ingame, id_mod, tmp, colorRed, -1 , -1);
			
		}
		
		if (in.quit == 1) break;
		
		if (update) {
			SDL_generate(ingame);
			update = 0;
		}
		
		if (animate >= 4) animate = 1;
		
	}

}

int main() {
	
	char pseudo[100], ratio[100], nb_mots_txt[100];
	int choix = 0;
	t_context *menu = NULL, *popup = NULL;
	
	memset(pseudo, 0, sizeof(pseudo));
	strcpy(pseudo, "NoName");
	
	// Aléatoirité
	srand(time(NULL));
	
	SDL_initWindow(800, 600, 0, "CrossWords ESDL", NULL, 1, "global.ttf", 20, 1); //800x600 +tff_support +audio_support
	
	generateIndex();
	
	menu = SDL_newContext("CrossWords SDL", 0, 0, 800, 600);
	SDL_newImage(menu, NULL, "APP_BG_SAMPLE.png", 0, 0);
	
	SDL_newObj(menu, NULL, 0, "Nouvelle partie", ALIGN_CENTER, NULL, NONE, 50, 550);
	SDL_newObj(menu, NULL, 0, "Options", ALIGN_CENTER, NULL, NONE, 280, 550);
	SDL_newObj(menu, NULL, 0, "Quitter", ALIGN_CENTER, NULL, NONE, 510, 550);
	SDL_newText(menu, NULL, pseudo, colorBlack, 550, 40);
	sprintf(ratio, "Score : %i d'XP", EXP_J1);
	SDL_newText(menu, NULL, ratio, colorRed, 550, 60);
	
	while (1) {
		
		sprintf(ratio, "Score : %i d'XP", EXP_J1);
		SDL_modText(menu, 0, pseudo, colorBlack, 550, 40);
		SDL_modText(menu, 1, ratio, colorRed, 550, 60);
		
		choix = SDL_generate(menu);
		
		switch (choix) {
			case 0:
				
				popup = SDL_newContext("Mon compte", 200, 200, 500, 250);
				memset(nb_mots_txt,0,sizeof(nb_mots_txt));
				SDL_newImage(popup, NULL, "popup.png", 0, 0);
				SDL_newText(popup, NULL, "Preciser le nombre de mots (de 10 a 40)", colorWhite, 10, 40);
				SDL_newObj(popup, NULL, 1, "Mots", ALIGN_LEFT, nb_mots_txt, NUMERIC, 70, 70);
				
				SDL_newObj(popup, NULL, 0, "Generer", ALIGN_CENTER, NULL , NONE, 150, 200);
				
				SDL_generate(popup);
				SDL_freeContext(popup);
				
				if (atoi(nb_mots_txt) >= 10 && atoi(nb_mots_txt) <= 40) {
					limite_mots = atoi(nb_mots_txt);
					generateGrille();
					ingame();
				}
				
				break;
			case 1:
				//Options
				popup = SDL_newContext("Mon compte", 200, 200, 500, 250);
				
				SDL_newImage(popup, NULL, "popup.png", 0, 0);
				SDL_newText(popup, NULL, "Voici le recapitulatif de votre session", colorWhite, 10, 40);
				SDL_newObj(popup, NULL, 1, "Pseudo", ALIGN_LEFT, pseudo, NONE, 70, 70);
				
				SDL_newText(popup, NULL, ratio, colorWhite, 80, 110);
				SDL_newObj(popup, NULL, 0, "Fermer", ALIGN_CENTER, NULL , NONE, 150, 200);
				
				SDL_generate(popup);
				SDL_freeContext(popup);
				break;
			case 2:
				SDL_freeContext(menu);
				flushIndex();
				exit(0);
				break;
		}
		
	}
	
	SDL_freeContext(menu);
	flushIndex();
	
	return 0;
	
}
