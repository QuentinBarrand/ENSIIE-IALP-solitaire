#include <stdio.h>

/* Constantes du pré-processeur
 * T_TAILLE : la T_TAILLE des cotés du damier.
 * H_TAILLE : le nombre d'instantanés du damier à conserver 
 *			  (taille de l'historique).
 * AUTHOR :	  le nom et les coordonnées du développeur.
 */
#define T_TAILLE	7
#define H_TAILLE 	25
#define AUTHOR 		"Quentin Barrand <quentin.barrand@ensiie.fr>"


/* Déclaration des fonctions utilisées */
void* memmove(void*, const void*, size_t);


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

	memmove(jeu.table, temp, sizeof(temp));
	
	jeu.nb_pion = 24;

	return jeu;
}


/* Affiche le damier sur la sortie standard.
 * Paramètres : 
 *		jeu : une instance du type damier
 * Retourne : code de statut
 *		0 : l'affichage s'est déroulé correctement
 *		1 : le damier contient des valeurs non prévues
 */
int afficher(damier jeu)
{
	int f, i, j, statut = 0;

	printf("\n      ");

	for(f = 1; f < 8; f++)
		printf("%d   ", f);

	printf("\n   -");

	for(f = 1; f < 8; f++)
		printf("----");

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
 *		1 : la case de départ n'est pas occupée par un pion
 *		2 : la case d'arrivée n'est pas libre
 */
int jouer(damier* jeu, coordonnees depart, coordonnees arrivee)
{
	/* Test case de départ */
	if(jeu->table[depart.a][depart.o] != PION)
		return 1;

	/* Test case d'arrivée */
	if(jeu->table[arrivee.a][arrivee.o] != LIBRE)
		return 2;

	return 0;
}


int main(int arg, char** argv) 
{
	damier jeu = initJeu();
	damier historique[H_TAILLE];

	int i;

	printf("\nBienvenue dans le jeu du Solitaire !\n\n");

	for(i = 0; jeu.nb_pion != 1; i++)
	{
		historique[i] = jeu;

		printf("Tour n°%d - grille de jeu :\n", i + 1);

		if(afficher(jeu) != 0)
		{
			printf("Le damier contien des valeurs non prévues.");
			printf("Sortie du programme.\n");
			printf("Merci d'envoyer un rapport d'erreur à " AUTHOR ".");

			return 2;
		}

		coordonnees input_d, input_a;

		printf("Entrez les coordonnées du déplacement :");
		
		printf("\nAbscisse de la case de départ : ");
		scanf("%d", &input_d.a);
		
		printf("Ordonnée de la case de départ : ");
		scanf("%d", &input_d.o);
		
		printf("Abscisse de la case d'arrivée : ");
		scanf("%d", &input_a.a);

		printf("Ordonnée de la case d'arrivée : ");
		scanf("%d\n\n", &input_a.o);

		switch(jouer(&jeu, input_d, input_a))
		{
			case 0:
				break;

			case -1:
				printf("La case de départ n'est pas occupée par un pion. Réessayez.\n");
				break;

			case -2:
				printf("La case d'arrivée n'est pas libre. Réessayez.\n");
				break;

			//case -3:
		}
	}

	printf("Le jeu est terminé, merci d'avoir joué !\n");

	return 0;
}