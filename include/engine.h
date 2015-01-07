/**
 * \file engine.h
 * \brief Partie moteur du mot-mêlés
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.1
 * \date 29/12/2014
 *
 * Génération de la grille; Indexation des mots
 *
 */
// Mettre DEBUG Ã  1 pour activer les messages d'erreur en console
#define DEBUG 0
#define M 20
#define MAX_LEN 128
#define NB_EMPTY_CELL_MIN 50

extern char matrice[M][M];
extern char words[100][20];
extern int id_matched[40];
extern int MAX_WORDS;
extern int nb_empty_cell;
extern int limite_mots;

// DECLARATIONS FONCTIONS
/**
 * \fn void initMatrice(void)
 * \brief Initialise la matrice manuellement
 *
 * \return AUCUNE
 */
void initMatrice(void);
/**
 * \fn void displayMatrice(void)
 * \brief Affiche la matrice sur la sortie standard
 *
 * \return AUCUNE
 */
void displayMatrice(void);
/**
 * \fn void fillMatrice(void)
 * \brief Rempli la matrice avec des mots par deux méthodes
 *
 * \return AUCUNE
 */
void fillMatrice(void);
/**
 * \fn void makePattern(char * pattern, int * x, int * y, int * direction)
 * \brief Génére un patron pour une situation donnée
 *
 * \param *pattern Pointeur vers la destion du patron (chaine)
 * \param *x Case de départ
 * \param *y Case de départ
 * \param *direction Entier représentant la direction
 * \return AUCUNE
 */
void makePattern(char * pattern, int * x, int * y, int * direction);
/**
 * \fn void initialiseFrom(FILE * fic)
 * \brief Lance l'indexation depuis un fichier texte
 *
 * \return AUCUNE
 */
void initialiseFrom(FILE * fic);
void writeWord(char * mot, int direction, int x, int y);
/**
 * \fn void randomFilling(void)
 * \brief Comble les trous s'il en reste après la génération de la grille
 *
 * \return AUCUNE
 */
void randomFilling(void);
/**
 * \fn int isAllreadyIn(char * mot)
 * \brief Savoir si un mot est présent dans la grille
 *
 * \param *mot Chaine à rechercher
 * \return Indice d'apparition ou -1 le cas échéant
 */
int isAllreadyIn(char * mot);