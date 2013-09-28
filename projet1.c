#include <stdio.h>

/* Constantes du pré-processeur
 * T_TAILLE : la T_TAILLE des cotés du damier.
 * H_TAILLE : le nombre d'instantanés du damier à conserver 
 *			  (taille de l'historique).
 */
#define T_TAILLE	7
#define H_TAILLE 	25


/* Représente les trois états possibles d'une case. */
typedef enum cases {
	VIDE, 
	LIBRE, 
	PION
} cases;


/* Représente les coordonnées d'une case */
typedef struct coordonnees {
	int a;
	int o;
} coordonnees;


/* Représente le jeu dans son état actuel. */
typedef struct damier {
	cases table[T_TAILLE][T_TAILLE];
	int nb_pion;
} damier;


/* Initialise une instance de damier et la renvoie au programme principal.
 * Retourne : Instance de damier contenant une matrice table initialisée
 *			  conformément au sujet
 */
damier initJeu()
{
	damier jeu;

	cases temp[T_TAILLE][T_TAILLE] = { 
		{VIDE, VIDE, VIDE, PION,  VIDE, VIDE, VIDE},
		{VIDE, PION, PION, PION,  PION, PION, VIDE},
		{VIDE, PION, VIDE, PION,  VIDE, PION, VIDE},
		{PION, PION, PION, LIBRE, PION, PION, PION},
		{VIDE, PION, VIDE, PION,  VIDE, PION, VIDE},
		{VIDE, PION, PION, PION,  PION, PION, VIDE},
		{VIDE, VIDE, VIDE, PION,  VIDE, VIDE, VIDE}
	};

	// memmove(jeu.table, temp, sizeof(temp));
	jeu.nb_pion = 24;

	return jeu;
}


/* Affiche le damier sur la sortie standard.
 * Paramètres : 
 *		-une instance de Damier
 * Retourne : code de statut
 *		 0 : l'affichage s'est déroulé correctement
 *		-1 : le damier contient des valeurs non prévues
 */
int afficher(damier jeu)
{
	int f, i, j, statut = 0;

	printf("\n      ");

	for(f = 1; f < 8; f++)
		printf("%d   ", f);

	printf("\n   -");

	for(f = 1; f < 8; f++)
		printf("----", f);

	printf("\n");

	for(i = 0; i < 7; i++)
	{
		printf("%d |  ", i + 1);

		for(j = 0; j < 7; j++)
		{
			switch(jeu.table[i][j])
			{
				case VIDE:
					printf("   ");
					break;
				
				case LIBRE:
					printf("[ ]");
					break;

				case PION:
					printf("[#]");
					break;

				default:
					printf("---");
					statut = -1;
			}

			printf(" ");
		}
	
		printf("\n");
	}

	printf("\n");

	return statut;
}


/*
 * Retourne : un code de statut
 *		-1 : la case de départ n'est pas occupée par un pion
 *		-2 : la case d'arrivée n'est pas libre
 */
int jouer(damier* jeu, coordonnees depart, coordonnees arrivee)
{
	/* Test case de départ */
	if(jeu->table[depart.a][depart.o] != PION)
		return -1;

	/* Test case d'arrivée */
	if(jeu->table[arrivee.a][arrivee.o] != LIBRE)
		return -1;

	return 0;
}


int main(int arg, char** argv) 
{
	damier jeu = initJeu();
	damier historique[H_TAILLE];

	historique[0] = jeu;

	afficher(jeu);

	coordonnees jinputD;
	jinputD.a = 3;
	jinputD.o = 3;

	coordonnees jinputA;
	jinputA.a = 3;
	jinputA.o = 3;

	printf("%d", jouer(&jeu, jinputD, jinputA));

	return 0;
}