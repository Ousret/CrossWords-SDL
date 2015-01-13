/*
	mliste.c 
	TAHRI Ahmed, SIMON Jérémy
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/mliste.h"

t_alphabet * dataAlphabet = NULL;

int nb_unique_char = 0;

int ajouterEnFin(int wordid, char letter, int pos, int * indexId)
{
	int row = 0;
	
	//On vÈrifie que la colonne LETTRE est bien existante le cas ÈchÈant on la crÈer
	if (isrowExist(letter) == 0) {
		
		if (rowCreate(letter) == 0) return -1;
		
	}else{
		
		row = rowId(letter);
		
	}
		
	if(!(dataAlphabet[row].index)) {
		
		dataAlphabet[row].index = malloc(sizeof(t_index));
		
	}else {
		
		dataAlphabet[row].index = realloc(dataAlphabet[row].index, sizeof(t_index)*((dataAlphabet[row].indexSize)+2));
		
	}
	
	if ((indexId) && ((*indexId) < (dataAlphabet[row].indexSize))) {
		
		if (dataAlphabet[row].index[dataAlphabet[row].indexSize].wordid == wordid) {
			
			//dataAlphabet[row].index[dataAlphabet[row].indexSize].pos = realloc(dataAlphabet[row].index[dataAlphabet[row].indexSize].pos, sizeof(int)*((dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ)+1));
			//dataAlphabet[row].index[dataAlphabet[row].indexSize].pos[dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ] = pos;
			//dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ = (dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ) + 1;
			
			return (*indexId);
			
		}else{
			
			dataAlphabet[row].index[dataAlphabet[row].indexSize].wordid = wordid;
			dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ = 0;
		
			dataAlphabet[row].index[dataAlphabet[row].indexSize].pos = malloc(sizeof(int));
			dataAlphabet[row].index[dataAlphabet[row].indexSize].pos[dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ] = pos;
		
			dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ = (dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ) + 1;
			dataAlphabet[row].indexSize++;
			return (dataAlphabet[row].indexSize);
			
		}
		
		
		
	}else{
		
		dataAlphabet[row].index[dataAlphabet[row].indexSize].wordid = wordid;
		dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ = 0;
		
		dataAlphabet[row].index[dataAlphabet[row].indexSize].pos = malloc(sizeof(int));
		dataAlphabet[row].index[dataAlphabet[row].indexSize].pos[dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ] = pos;
		
		dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ = (dataAlphabet[row].index[dataAlphabet[row].indexSize].nb_occ) + 1;
		
		dataAlphabet[row].indexSize++;
		
		return (dataAlphabet[row].indexSize);
	}
	
}

int isrowExist(char letter) {
	
	int row;
	
	for (row = 0; row < nb_unique_char; row++) {
			if (dataAlphabet[row].letter == letter) return 1;
	}
	
	return 0;
	
}

int rowId(char letter) {
	
	int row;
	
	for (row = 0; row < nb_unique_char; row++) {
			if (dataAlphabet[row].letter == letter) return row;
	}
	
	return -1;
	
}

int rowCreate(char letter) {
	
	if (nb_unique_char == 0) {
		
		dataAlphabet = malloc(sizeof(t_alphabet));
		
	}else{
		
		if (isrowExist(letter) == 1) return 0;
		dataAlphabet = realloc(dataAlphabet, sizeof(t_alphabet) * (nb_unique_char+1));
		
	}
	
	dataAlphabet[nb_unique_char].letter = letter;
	dataAlphabet[nb_unique_char].index = NULL;
	dataAlphabet[nb_unique_char].indexSize = 0;
	
	nb_unique_char++;
	
	return 1;
	
}

int ischarMatch(char * word, char lettre, int indice) {
	if ((int)strlen(word) <= indice) return 0;
	if (word[indice] == lettre) return 1;
	return 0; 
}

char * rechercherElement(char * pattern, int indice)
{
    int len = strlen(pattern), i = 0, j = 0, k = 0, p = 0;
    int rowId_tmp = -1, onlyLenMatch = 1;
    int indice_round = 0, nbLettertomatch = 0, nbLetterRemain = 0;
    
    for (i = 0; i < len; i++) {
    	if (pattern[i] != '*') nbLettertomatch++;
	}
	
	nbLetterRemain = nbLettertomatch;
    
    if (nbLettertomatch == 0) {
    	
    	for (i = 0; i < nb_words; i++) {
    		if ((int)strlen(allData[i].word) == len) {
    			
    			if (indice_round == indice) {
    				return allData[i].word;	
    			}else{
    				indice_round++;
				}
				
			} 
		}
		
		return NULL;
	}
    
    //printf("<?> You wanted to get word like %s\n", pattern);
    
    for (i = 0; i < len; i++) {
    	
		if (pattern[i] != '*') {
			
			onlyLenMatch = 0;
			rowId_tmp = rowId(pattern[i]);

			for (j = 0; j < dataAlphabet[rowId_tmp].indexSize; j++) {

    			if ((int)strlen(allData[dataAlphabet[rowId_tmp].index[j].wordid].word) == len) {
    				
    				for (k = 0; k < dataAlphabet[rowId_tmp].index[j].nb_occ; k++) {
    					
    					//On trouve une lettre en commun avec le mot id j
    					if (dataAlphabet[rowId_tmp].index[j].pos[k] == i) {
    						
							if (indice_round == indice) {
    							//On test le reste des lettres pour le même mot
    							for (p = 0; p < (int)strlen(pattern); p++) {
    								if (pattern[p] != '*') {
    									if (ischarMatch(allData[dataAlphabet[rowId_tmp].index[j].wordid].word, pattern[p], p) == 1) {
    										nbLetterRemain--;
										}
										
									}
									if (nbLetterRemain == 0) {
										return (allData[dataAlphabet[rowId_tmp].index[j].wordid].word);
									}
								}
								
								nbLetterRemain = nbLettertomatch;
								
								 
							}else{
								indice_round++;
								break;
							}	
						}
					}
					
					
					
				}
				
			}
		}
	}
    
    
    
    return NULL;
}

void flushIndex()
{
    int i = 0;
    
    for (i = 0; i < nb_unique_char; i++) {
    	
    	if (dataAlphabet[i].index) {
    		free (dataAlphabet[i].index);
		}
    	
	}
	
	if (dataAlphabet) {
		free (dataAlphabet);
	}	
}

char * getrandpatternMatch(char * pattern) {
	
	char * results[1000], * wordPTR_tmp= NULL, *wordPTR=NULL;
	int count = 0, nb_results = 0;
	
	do {
		
		wordPTR_tmp = wordPTR;
		wordPTR = rechercherElement(pattern, count);
			
		if ((wordPTR) && (wordPTR != wordPTR_tmp)) {
			results[nb_results] = wordPTR;
			nb_results++;
			if (nb_results > 500) break;
		}
		
		count++;
		
	}while(wordPTR);
	
	if (nb_results > 0) {
		return results[rand()%nb_results];	
	}else{
		return NULL;
	}
	
}
