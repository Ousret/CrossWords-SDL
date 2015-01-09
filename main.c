#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ESDL.h>

#include "include/engine.h"

#define DIR_UP 4
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

int EXP_J1 = 0;

void generateIndex() {
	char msg[200];
	memset(msg, 0, sizeof(msg));
	t_window * loading = SDL_newWindow("CrossWords SDL", 0, 0, 800, 600);
	SDL_newTexture(loading, NULL, "APP_BG_LOAD.png", 0, 0, 800, 600);
	SDL_newText(loading, NULL, msg, colorBlack, 50, 550);
	
	//Indexation
	FILE * fic = NULL;
	fic=fopen("fr.dic","r");
	//if(DEBUG)fprintf(stderr,"\nIndex initializing");
	
	sprintf(msg,"Lecture du dictionnaire..");
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	initialiseFrom(fic);
	//if(DEBUG)fprintf(stderr,"\nIndex initialized\n");
	
	sprintf(msg, "Dictionnaire pret a l'emploi..");
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	
	fclose(fic);
	SDL_freeWindow(loading);
}

void generateGrille() {
	
	char msg[200];
	memset(msg, 0, sizeof(msg));
	t_window * loading = SDL_newWindow("CrossWords SDL", 0, 0, 800, 600);
	SDL_newTexture(loading, NULL, "APP_BG_LOAD.png", 0, 0, 800, 600);
	SDL_newText(loading, NULL, msg, colorBlack, 50, 550);
	
	// DEBUG BENCHMARK
	time_t tbegin;
	time_t tend;
	time_t texec;
	if(DEBUG)tbegin=time(NULL);
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
	sprintf(msg, "Generation d'une nouvelle grille..");
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	
	fillMatrice();
	randomFilling();
	
	sprintf(msg, "Chargement..");
	SDL_modText(loading, 0, msg, colorBlack, 50, 550);
	SDL_generate(loading);
	
	// AFFICHAGE DE LA MATRICE (CONSOLE)
	if(DEBUG)displayMatrice();
	
	// DEBUG BENCHMARK
	if(DEBUG)tend = time(NULL);
	if(DEBUG)texec = difftime(tend,tbegin);
	if(DEBUG)fprintf(stderr,"\n\nBENCH : %li\n\n",texec);
	
	SDL_freeWindow(loading);
	
}

int pause() {
	int choix = 0;
	t_window * popup = SDL_newWindow("Mon compte", 200, 200, 500, 250);
				
	SDL_newTexture(popup, NULL, "popup.png", 0, 0, 500, 250);
	SDL_newText(popup, NULL, "PAUSE", colorWhite, 100, 100);
	
	SDL_newObj(popup, NULL, 0, "Continuer", NULL, ALL, 10, 200, 40, 230);
	SDL_newObj(popup, NULL, 0, "Menu", NULL , ALL, 250, 200, 40, 230);
				
	choix = SDL_generate(popup);
	SDL_freeWindow(popup);	
	
	return choix;
}

void ingame() {
	
	t_window * ingame =  SDL_newWindow("CrossWords SDL", 0, 0, 800, 600);
	int posX = 40, posY = 80, i = 0, j = 0, update = 1, animate = 1, x_tmp = 85, y_tmp = 40;
	char XP_HUD[100];
	int pos_relative_x = 0, pos_x_tmp = 0, pos_y_tmp = 0, pos_relative_y = 0, id_mod = 0;
	
	int nb_word_remain = MAX_WORDS;
	
	char select[50];
	int select_mode = 0, id_match = 0;
	int HUD = 0;
	char tmp[5];
	tmp[1] = '\0';
	
	memset(select, 0, sizeof(select));
	memset(XP_HUD, 0, sizeof(XP_HUD));
	
	SDL_newTexture(ingame, NULL, "APP_INGAME.png", 0, 0, 800, 600);
	SDL_newSprite(ingame, "panda_sp.png", colorGreenLight, 145, 144, 36, 33, posX, posY, DIR_DOWN, 1, 0);
	//SDL_preloadwav("xpup.wav");
	
	//510x 40y
	for (i = 0; i < M; i++) {
		
		for (j = 0; j < M; j++) {
			tmp[0] = matrice[i][j];
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
				SDL_freeWindow(ingame);
				return;
			}else{
				update = 1;
			}
		}
		
		if (SDL_isKeyPressed(SDLK_SPACE)) {
			
			select_mode = 1;
			SDL_modText(ingame, HUD, "SPACE PRESSED !", colorRed, -1, -1);
			
		}else{
			if (select_mode == 1) {
			
				SDL_modText(ingame, HUD, "SPACE RELEASED !", colorWhite, -1, -1);
				fprintf(stderr, "%s\n", select);
				select_mode = 0;
				
				id_match = isAllreadyIn(select);
				
				for (i = 0; i < MAX_WORDS-nb_word_remain; i++) {
						if (id_matched[i] == id_match) break;
				}
				
				if ((id_match >= 0) && (id_match != id_matched[i])) {
					
					SDL_modText(ingame, 400+id_match, words[id_match], colorRed, -1, -1);
					//SDL_playwav("xpup.wav", 0, NULL);
					id_matched[MAX_WORDS-nb_word_remain] = id_match;
					
					nb_word_remain--;
					EXP_J1+=3;
					sprintf(XP_HUD, "%i d'XP", EXP_J1);
					SDL_modText(ingame, HUD+1, XP_HUD, colorWhite, -1, -1);
					if (nb_word_remain <= 0) {
						SDL_freeWindow(ingame);
						return;	
					}else{
						update = 1;
					} 
				}
				
				for (i = 0; i < M; i++) {
		
					for (j = 0; j < M; j++) {
						tmp[0] = matrice[i][j];
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
			tmp[0] = matrice[pos_relative_y][pos_relative_x];
			if (pos_x_tmp != pos_relative_x || pos_y_tmp != pos_relative_y) strcat (select, tmp);
			SDL_modText(ingame, id_mod, tmp, colorRed, -1 , -1);
			
		}
		
		if (in.quit == 1) break;
		
		if (update) {
			SDL_generate(ingame);
			update = 0;
		}
		
		SDL_Delay(50);
		
		if (animate >= 4) animate = 1;
		
	}

}

int main(int argc, char * argv[]) {
	
	char pseudo[100], ratio[100], nb_mots_txt[100];
	int choix = 0;
	t_window *menu = NULL, *popup = NULL;
	
	memset(pseudo, 0, sizeof(pseudo));
	strcpy(pseudo, "NoName");
	
	// Aléatoirité
	if(DEBUG)fprintf(stderr,"\nRandom initializing");
	srand(time(NULL));
	if(DEBUG)fprintf(stderr,"\nRandom initialized\n");
	
	SDL_init(800, 600, 0, "CrossWords ESDL", NULL, 1, "global.ttf", 20, 1); //800x600 +tff_support +audio_support
	generateIndex();
	
	menu = SDL_newWindow("CrossWords SDL", 0, 0, 800, 600);
	SDL_newTexture(menu, NULL, "APP_BG_SAMPLE.png", 0, 0, 800, 600);
	
	SDL_newObj(menu, NULL, 0, "Nouvelle partie", NULL, ALL, 50, 550, 40, 230);
	SDL_newObj(menu, NULL, 0, "Options", NULL, ALL, 280, 550, 40, 230);
	SDL_newObj(menu, NULL, 0, "Quitter", NULL, ALL, 510, 550, 40, 230);
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
				
				popup = SDL_newWindow("Mon compte", 200, 200, 500, 250);
				memset(nb_mots_txt,0,sizeof(nb_mots_txt));
				SDL_newTexture(popup, NULL, "popup.png", 0, 0, 500, 250);
				SDL_newText(popup, NULL, "Preciser le nombre de mots (de 10 a 40)", colorWhite, 10, 40);
				SDL_newObj(popup, NULL, 1, "Mots", nb_mots_txt, NUMERIC, 70, 70, 40, 400);
				
				SDL_newObj(popup, NULL, 0, "Generer", NULL , ALL, 150, 200, 40, 230);
				
				SDL_generate(popup);
				SDL_freeWindow(popup);
				
				if (atoi(nb_mots_txt) >= 10 && atoi(nb_mots_txt) <= 40) {
					limite_mots = atoi(nb_mots_txt);
					generateGrille();
					ingame();
				}
				
				break;
			case 1:
				//Options
				popup = SDL_newWindow("Mon compte", 200, 200, 500, 250);
				
				SDL_newTexture(popup, NULL, "popup.png", 0, 0, 500, 250);
				SDL_newText(popup, NULL, "Voici le recapitulatif de votre session", colorWhite, 10, 40);
				SDL_newObj(popup, NULL, 1, "Pseudo", pseudo, ALL, 70, 70, 40, 400);
				
				SDL_newText(popup, NULL, ratio, colorWhite, 80, 110);
				SDL_newObj(popup, NULL, 0, "Fermer", NULL , ALL, 150, 200, 40, 230);
				
				SDL_generate(popup);
				SDL_freeWindow(popup);
				break;
			case 2:
				exit(0);
				break;
		}
		
	}
	
	SDL_freeWindow(menu);
	
	return 0;
	
}
