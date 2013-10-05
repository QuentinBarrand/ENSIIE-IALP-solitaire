#include <stdio.h>

/* Constantes du pré-processeur personnalisées
 * T_TAILLE : la taille des cotés du damier.
 * H_TAILLE : le nombre d'instantanés du damier à conserver
 *			  (taille de l'historique).
 * AUTEUR :	  le nom et les coordonnées du développeur.
 */
#define T_TAILLE	7
#define H_TAILLE 	100
#define AUTEUR 		"Quentin Barrand <quentin.barrand@ensiie.fr>"

/* Codes de retour pour le programme principal */
#define EXIT_SUCCESS		0
#define EXIT_SYNTAX_ERROR	1
#define EXIT_RUNTIME_ERROR	2

 /* Macros */
 #define MAX(x, y) (((x) > (y) ? (x) : (y)))
 #define MIN(x, y) (((x) < (y) ? (x) : (y)))


/* Déclaration des fonctions externes utilisées */
void* memmove(void*, const void*, size_t);


/* Options du programme */
typedef struct options {
	int n;
	int d;
} options;


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


/* Affiche l'usage du programme */
void help(char* executable)
{
	printf("Usage : %s [-h] [-n] [-d]\n", executable);
}


/* Récupère les options de la ligne de commande 
 * Paramètres :
 *		opt : adresse de l'instance de configuration de l'application
 *		argc : nombre d'arguments de la ligne de commande
 *		argv : matrice des arguments de la ligne de commande
 * Retourne : un code de statut
 *		0 : tous les paramètres ont été lus correctement
 *		1 : a affiché l'aide
 *		2 : une erreur de lecture des paramètres est survenue
 */
int getOptions(options* config, int argc, char** argv)
{
	int i, status = 0;

	config->n = 0;
	config->d = 0;

	for(i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'h':
					help(argv[0]);
					status = 1;
					break;

				case 'n':
					config->n = 1;
					break;

				case 'd':
					config->d = 1;
					break;

				default:
					status = 2;
					break;
			}
		}
		else
			/* Inutile d'aller plus loin, au moins un argument est incorrect */
			return 2;
	}

	return status;
}


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
 * Paramètre : 
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
			switch(jeu.table[j][i])
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
 *		3 : le mouvement n'est pas autorisé par les options
 *		4 : la distance entre les deux cases n'est pas égale à 2
 */
int jouer(damier* jeu, options* config, 
	coordonnees depart, coordonnees arrivee)
{
	/* Test case de départ */
	if(jeu->table[depart.a][depart.o] != PION)
		return 1;

	/* Test case d'arrivée */
	if(jeu->table[arrivee.a][arrivee.o] != LIBRE)
		return 2;

	/* Test mouvement horizontal / vertical autorisé */
	if(!config->n && (depart.a != arrivee.a || depart.o != arrivee.o))
		return 3;

	/* Test mouvement diagonal autorisé */
	if(!config->d && (depart.a != arrivee.a || depart.o != arrivee.o))
		return 3;

	/* Test distance égale à 2 */
	if((MAX(depart.a, arrivee.a) - MIN(depart.a, arrivee.a) != 2) && \
	   (MAX(depart.o, arrivee.o) - MIN(depart.o, arrivee.o) != 2))
		return 4;

	coordonnees centrale;

	if(depart.a == arrivee.a)
		centrale.a = depart.a;
	else 
		centrale.a = MAX(depart.a, arrivee.a) - 1;

	if(depart.o == arrivee.o)
		centrale.o = depart.o;
	else
		centrale.o = MAX(depart.o, arrivee.o) - 1;

	/* Modification du damier */
	jeu->table[depart.a][depart.o] = LIBRE;
	jeu->table[arrivee.a][arrivee.o] = PION;
	jeu->table[centrale.a][centrale.o] = LIBRE;

	jeu->nb_pion -= 1;

	return 0;
}


int main(int argc, char** argv) 
{

	char cont;

	/* Import des paramètres de la ligne de commande */
	options config;
	switch(getOptions(&config, argc, argv))
	{
		case 1:
			/* Si l'aide a été affichée, le programme se termine 
			 * correctement.
			 */
			return EXIT_SUCCESS;

		case 2:
			printf("Paramètres de la ligne de commande incorrects.\n");
			help(argv[0]);
			return EXIT_SYNTAX_ERROR;
	}

	if(config.n == 0 && config.d == 0)
	{
		printf("Vous devez autoriser au moins un type de déplacement. " \
			"Lancez %s -h ou consultez le fichier README pour obtenir " \
			"de l'aide.\n", argv[0]);

		return EXIT_SYNTAX_ERROR;
	}
	
	/* Création du damier */
	damier jeu = initJeu();
	damier historique[H_TAILLE];

	int i;

	printf("Bienvenue dans le jeu du Solitaire !");

	for(i = 0; jeu.nb_pion != 1; i++)
	{
		historique[i] = jeu;

		printf("\n\nTour n°%d - grille de jeu :\n", i + 1);

		if(afficher(jeu) != 0)
		{
			printf("Le damier contient des valeurs non prévues." \
				   "Sortie du programme.\n" \
				   "Merci d'envoyer un rapport d'erreur à " AUTEUR ".");

			return EXIT_RUNTIME_ERROR;
		}

		cont = 0;

		/* Menu principal */
		while(cont != 'c')
		{
			printf("Action ? ('c' continuer, 'p' (précédent), " \
				"q (quitter)) : ");

			/* Lire une chaine nous permet de vider l'intégralité du buffer 
			 * de stdin sans avoir des \n qui déclenchent des tours de boucle
			 * supplémentaires.
			 */
			scanf("%s", &cont);

			switch(cont)
			{
				case 'c':
					break;

				case 'p':
					if(i > 0)
					{
						i--;
						printf("Retour au tour n°%d\n", i + 1);
						jeu = historique[i];
						afficher(jeu);
						printf("\n\n");
					}
					else
						printf("Etape n°%d : impossible de charger le "\
							"tour précédent.\n", i);

					break;

				case 'q':
					printf("Le jeu est terminé, merci d'avoir joué !\n");
					return EXIT_SUCCESS;

				default:
					printf("Caractère non reconnu.\n");
			}
		}

		coordonnees input_d, input_a;

		printf("\nEntrez les coordonnées du déplacement :");

		printf("\nAbscisse de la case de départ : ");
		scanf("%d", &input_d.a);

		printf("Ordonnée de la case de départ : ");
		scanf("%d", &input_d.o);

		printf("Abscisse de la case d'arrivée : ");
		scanf("%d", &input_a.a);

		printf("Ordonnée de la case d'arrivée : ");
		scanf("%d", &input_a.o);

		/* Mise en conformité avec les coordonnées [O, ...] du damier */
		input_d.a -= 1;
		input_d.o -= 1;
		input_a.a -= 1;
		input_a.o -= 1;

		printf("\n");

		switch(jouer(&jeu, &config, input_d, input_a))
		{
			case 1:
				printf("La case de départ n'est pas occupée par un pion. " \
					"Réessayez.\n");
				i--;
				break;

			case 2:
				printf("La case d'arrivée n'est pas libre. Réessayez.\n");
				i--;
				break;

			case 3:
				printf("Ce type de mouvement n'est pas autorisé par les " \
					"options de la ligne de commande. Réessayez.\n");
				i--;
				break;

			case 4:
				printf("La distance entre les deux cases est incorrecte. " \
					"Réessayez.\n");
				i--;
				break;
		}
	}

	printf("Le jeu est terminé, merci d'avoir joué !\n");
	return EXIT_SUCCESS;
}

