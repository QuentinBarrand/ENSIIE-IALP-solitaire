/**
 * \file main.c
 * \brief Fichier principal et point d'entrée de la solution. 
 * 
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 * \version 1.1
 */
#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "jeu.h"

/** Le nombre d'instantanés du damier à conserver (taille de l'historique). */
#define H_TAILLE 100

/** Le nom et les coordonnées du développeur. */
#define AUTEUR "Quentin Barrand <quentin.barrand@ensiie.fr>"

/** Fonction main : point d'entrée de l'application. */
int main(int argc, char** argv) 
{
    /** Variable interne utilisée pour la saisie. */
    static char cont;

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
            return EXIT_FAILURE;
    }

    if(config.n == 0 && config.d == 0)
    {
        printf("Vous devez autoriser au moins un type de déplacement. "
            "Lancez %s -h ou consultez le fichier README pour obtenir "
            "de l'aide.\n", argv[0]);

        return EXIT_FAILURE;
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
            printf("Le damier contient des valeurs non prévues." 
                   "Sortie du programme.\n" 
                   "Merci d'envoyer un rapport d'erreur à " AUTEUR ".");

            return EXIT_FAILURE;
        }

        cont = 0;

        /* Menu principal */
        while(cont != 'c')
        {
            printf("Action ? ('c' continuer, 'p' (précédent), "
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
                        printf("Etape n°%d : impossible de charger le "
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
                printf("La case de départ n'est pas occupée par un pion. "
                    "Réessayez.\n");
                i--;
                break;

            case 2:
                printf("La case d'arrivée n'est pas libre. Réessayez.\n");
                i--;
                break;

            case 3:
                printf("Ce type de mouvement n'est pas autorisé par les "
                    "options de la ligne de commande. Réessayez.\n");
                i--;
                break;

            case 4:
                printf("La distance entre les deux cases est incorrecte. "
                    "Réessayez.\n");
                i--;
                break;
        }
    }

    printf("Le jeu est terminé, merci d'avoir joué !\n");
    return EXIT_SUCCESS;
}
