/**
 * \file SpiTaJoke_SDL.c
 * \brief Librairies SDL/C 1.2
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.2
 * \date 27 Octobre 2014
 *
 * Toutes les fonctions utiles à la SDL
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_mixer.h> //Librairies du mixage.. ( LOVE PARAPHRASE :-D )
#include <SDL_ttf.h> //Pour imprimer du texte graphiquement
#include <SDL_image.h> //Pour imprimer des images
#include <string.h> //Gestion des chaines

#include "includes/STJ_SDL.h"

void SPI_SDL_Init_Video(int x, int y, char titre[100], int ttf_support, int audio_support) {
	
    int sdl_start = 0;
	
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;
	
    /* Initialize SDL */
    if (audio_support == 1) {
    	sdl_start = SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    }else {
    	sdl_start = SDL_Init (SDL_INIT_VIDEO);
    }
    
    if (sdl_start < 0)
    {
        
        fprintf (stderr, "La SDL n'a pas pu s'initialiser: %s\n", SDL_GetError ());
        exit (1);
        
    }
    
	atexit (SDL_Quit);

    screen = SDL_SetVideoMode (x, y, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL)
    {
        
        fprintf (stderr, "Impossible d'initialiser la fenetre en %ix%i mode 16 bits': %s\n", x, y ,SDL_GetError ());
        exit (2);
        
    }
    
	SDL_WM_SetCaption (titre, NULL);
	
	if (audio_support == 1) {
		
		if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
			
			fprintf (stderr, "Erreur d'initialisation audio: %s", Mix_GetError());
		    exit (1);
		    
		}
		
	}
	
	if (ttf_support == 1) {
		
		if(TTF_Init() == -1)
		{
	    	fprintf (stderr, "Erreur d'initialisation de la SDL_ttf !");
	        exit (1);
		}
		
		SDL_EnableUNICODE(1);
		
	}
	
}

int SDL_Create_Menu(TTF_Font *police, int nb_entre, char sommaire[N][M]) {
	
	int i = 0, action = 0;
	int lastevent = -1;

	sound = Mix_LoadWAV("ressources/snd/select.wav");
	
	//On ne quitte pas la boucle tant qu'aucune selection n'a ÈtÈ faite
	while (1) {
		
		action = SDL_WaitEvent(&GlobalEvent); /* RÈcupÈration de l'ÈvÈnement dans event (non-blocant) */
		
		SDL_Print_bg("ressources/images/app_bg.png", 0, 0); //Fond d'écran
		SDL_Print_bg("ressources/images/BarreLaterale.png", 80, 25); //Barre du menu
		
		for (i = 0; i < nb_entre; i++) {
			SDL_Create_Menu_Ch(police, i, sommaire[i], 100, 100+(i*50));
		}
		
		if (lastevent != sel_menu_m) {
		
			SDL_Flip (screen);
			channel = Mix_PlayChannel(-1, sound, 0);
			lastevent = sel_menu_m;
		
		}
		
		if (action) {
				
			switch (GlobalEvent.type)
        	{
		        case SDL_MOUSEBUTTONDOWN: //Si on clique
		        	
					if (SDL_Souris_Survol(40, 230, 100, 100+(sel_menu_m*50)) == 1) {
						sound = Mix_LoadWAV("ressources/snd/enter.wav");
						channel = Mix_PlayChannel(-1, sound, 0);
						while(Mix_Playing(channel) != 0);
						return sel_menu_m;
					}
					
					break;
					
		        case SDL_QUIT:
		        
		        	exit (0);
					break;
					
			}
		}
		
		SDL_Delay(20);
		
	}
	
}

void SDL_Create_Menu_Ch(TTF_Font *police, int id, char titre[M], int x, int y) {
	
	SDL_Rect positionFond; //(dynamique)
	SDL_Surface *imageDeFond = NULL, *titre_ttf = NULL;
	positionFond.x = x;
	positionFond.y = y;
	
	int sel_souris = SDL_Souris_Survol(40, 230, x, y);
	
	
	//On charge l'image concernÈe ++ si souris survol choix
	if (sel_souris == 1) {
		imageDeFond = IMG_Load("ressources/images/m_bg_s1.png");
		sel_menu_m = id;
	}else{
		imageDeFond = IMG_Load("ressources/images/m_bg_s0.png");
	}
	
	SDL_BlitSurface(imageDeFond, NULL, screen, &positionFond);
	
	titre_ttf = TTF_RenderText_Blended(police, titre, couleurBlanche);
	positionFond.x += 15;
	SDL_BlitSurface(titre_ttf, NULL, screen, &positionFond);
	
	SDL_FreeSurface(imageDeFond);
	SDL_FreeSurface(titre_ttf);
	
}

void SDL_Print_bg(char file[50], int x, int y) {
	
	SDL_Rect positionFond; //(dynamique)
	SDL_Surface *imageDeFond = NULL;
	
	positionFond.x = x;
	positionFond.y = y;
	
	if (x == 0 && y == 0) SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	
	imageDeFond = IMG_Load(file);
	
	SDL_BlitSurface(imageDeFond, NULL, screen, &positionFond);
	
	SDL_FreeSurface(imageDeFond);
	
}

void SDL_Print_popup() {
	
	SDL_Rect positionFond; //(dynamique)
	SDL_Surface *imageDeFond = NULL;
	
	positionFond.x = 100;
	positionFond.y = 100;
	
	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	
	imageDeFond = IMG_Load("ressources/images/popup.png");
	
	SDL_BlitSurface(imageDeFond, NULL, screen, &positionFond);
	
	SDL_FreeSurface(imageDeFond);
	
}

int SDL_Souris_Survol(int hauteur, int largeur, int x, int y) {
	
	if ( GlobalEvent.button.y > y && GlobalEvent.button.y <= y+hauteur && GlobalEvent.button.x > x && GlobalEvent.button.x <= x+largeur ) {
		return 1;
	}else{
		return 0;
	}
	
}

void SDL_Open_PopUp(int ligne, TTF_Font *police, char txt_ligne1[200], char txt_ligne2[200], char txt_ligne3[200]) {
	
	int action = 0;
	
	while (1) {
		
		action = SDL_WaitEvent(&GlobalEvent); /* RÈcupÈration de l'ÈvÈnement dans event (non-blocant) */
		
		SDL_Print_bg("ressources/images/", 0, 0);
		SDL_Print_popup();
		
		SDL_Write_popup(ligne, police, txt_ligne1, txt_ligne2, txt_ligne3);
		SDL_Print_Btn(1, police, "OK", 270, 300);
		
		SDL_Flip (screen);
		
		if (action) {
			switch (GlobalEvent.type)
        	{
		        case SDL_MOUSEBUTTONDOWN: //Si on clique
		        	
					if (SDL_Souris_Survol(40, 140, 270, 300) == 1) {
						return;
					}
					break;
					
		        case SDL_QUIT:
		        	exit (0);
					break;
			}
		}
		
		
		SDL_Delay (20);
		
	}
	
}

void SDL_Write_popup(int ligne, TTF_Font *police, char txt_ligne1[200], char txt_ligne2[200], char txt_ligne3[200]) {
	
	SDL_Surface *ligne1 = NULL, *ligne2 = NULL, *ligne3 = NULL;
	SDL_Rect positionLigne;
	
	positionLigne.x = 110;
	positionLigne.y = 140;
	
	switch (ligne) {
		
		case 1:
			ligne1 = TTF_RenderText_Blended(police, txt_ligne1, couleurBlanche);
			
			SDL_BlitSurface(ligne1, NULL, screen, &positionLigne);
			SDL_FreeSurface(ligne1);
			break;
		case 2:
			ligne1 = TTF_RenderText_Blended(police, txt_ligne1, couleurBlanche);
			ligne2 = TTF_RenderText_Blended(police, txt_ligne2, couleurBlanche);
			
			SDL_BlitSurface(ligne1, NULL, screen, &positionLigne);
			positionLigne.y += 25;
			SDL_BlitSurface(ligne2, NULL, screen, &positionLigne);
			
			SDL_FreeSurface(ligne1);
			SDL_FreeSurface(ligne2);
			break;
		case 3:
			ligne1 = TTF_RenderText_Blended(police, txt_ligne1, couleurBlanche);
			ligne2 = TTF_RenderText_Blended(police, txt_ligne2, couleurBlanche);
			ligne3 = TTF_RenderText_Blended(police, txt_ligne3, couleurBlanche);
			
			SDL_BlitSurface(ligne1, NULL, screen, &positionLigne);
			positionLigne.y += 25;
			SDL_BlitSurface(ligne2, NULL, screen, &positionLigne);
			positionLigne.y += 25;
			SDL_BlitSurface(ligne3, NULL, screen, &positionLigne);
			
			SDL_FreeSurface(ligne1);
			SDL_FreeSurface(ligne2);
			SDL_FreeSurface(ligne3);
			
			break;
	}
	
	
	
}

void SDL_Print_Btn(int id,TTF_Font *police, char titre[30], int x, int y) {
	
	SDL_Rect positionFond; //(dynamique)
	SDL_Surface *imageDeFond = NULL, *titre_ttf = NULL;
	
	positionFond.x = x;
	positionFond.y = y;
	
	int sel_souris = SDL_Souris_Survol(40, 140, x, y);
	
	//On charge l'image concernÈe ++ si souris survol choix
	if (sel_souris == 1) {
		imageDeFond = IMG_Load("ressources/images/btn_s1.png");
		sel_menu_m = id;
	}else{
		imageDeFond = IMG_Load("ressources/images/btn_s0.png");
	}
	
	SDL_BlitSurface(imageDeFond, NULL, screen, &positionFond);
	
	titre_ttf = TTF_RenderText_Blended(police, titre, couleurNoire);
	positionFond.x += 50;
	positionFond.y += 5;
	SDL_BlitSurface(titre_ttf, NULL, screen, &positionFond);
	
	SDL_FreeSurface(imageDeFond);
	SDL_FreeSurface(titre_ttf);
	
}

void SDL_Print_Form(int id, TTF_Font *police, char titre[30], int etat, char dest[], int * sel_souris_form, int x, int y) {
	
	SDL_Rect positionFond; //(dynamique)
	SDL_Surface *imageDeFond = NULL, *titre_ttf = NULL, *saisie_ttf = NULL;
	char saisie_content[100];
	
	memset (saisie_content, 0, sizeof (saisie_content));
	
	positionFond.x = x;
	positionFond.y = y;
	
	int sel_souris = SDL_Souris_Survol(40, 400, x, y);
	
	if ((sel_souris == 1) && (GlobalEvent.type == SDL_MOUSEBUTTONDOWN)) {
		*sel_souris_form = id;
		strcpy (saisie_content,dest);
  		strcat (saisie_content,"|");
  		puts (saisie_content);
	}else if(id == *sel_souris_form) {
		strcpy (saisie_content,dest);
  		strcat (saisie_content,"|");
  		puts (saisie_content);
	}else{
		strcpy (saisie_content,dest);
	}
	
	
	//On charge l'image concernÈe ++ si souris survol choix
	if (etat == 1) {
		imageDeFond = IMG_Load("ressources/images/ch_saisie_actif.png");
		//sel_menu_m = id;
	}else{
		imageDeFond = IMG_Load("ressources/images/ch_saisie_grise.png");
	}
	
	SDL_BlitSurface(imageDeFond, NULL, screen, &positionFond);
	
	saisie_ttf = TTF_RenderText_Blended(police, saisie_content, couleurNoire);
	positionFond.x = x+10;
	positionFond.y = y+5;
	SDL_BlitSurface(saisie_ttf, NULL, screen, &positionFond);
	
	titre_ttf = TTF_RenderText_Blended(police, titre, couleurBlanche);
	positionFond.x = x-55;
	positionFond.y = y+5;
	SDL_BlitSurface(titre_ttf, NULL, screen, &positionFond);
	
	SDL_FreeSurface(imageDeFond);
	SDL_FreeSurface(titre_ttf);
	
}

void SDL_Splash(char img[100], int attente) {
	
	SDL_Rect positionFond; //(dynamique)
	SDL_Surface *imageDeFond = NULL;
	//int i = 0;
	
	positionFond.x = 0;
	positionFond.y = 0;
	

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	
	imageDeFond = IMG_Load(img);
	SDL_BlitSurface(imageDeFond, NULL, screen, &positionFond);
	
		
	SDL_Flip (screen);
	
	SDL_FreeSurface(imageDeFond);
	
	SDL_Delay(attente);
	
}

void SDL_RAZ_ALL() {
		
}