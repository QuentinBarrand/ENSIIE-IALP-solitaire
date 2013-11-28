/**
 * \file main.c
 * \brief Fichier principal et point d'entrée de la solution. 
 * 
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#include <stdio.h>
#include <stdlib.h>

#include "lib/Sutils.h"
#include "lib/Sjeu.h"
#include "lib/Sconsts.h"

/** Fonction main : point d'entrée de l'application. */
int main(int argc, char** argv) 
{
    #ifdef DEBUG
    printf("Mode DEBUG activé\n");
    #endif

    /* Import des paramètres de la ligne de commande */
    options config;
    switch(Sutils_GetOptions(&config, argc, argv))
    {
        case 1:
            /* Si l'aide a été affichée, le programme se termine 
             * correctement.
             */
            return EXIT_SUCCESS;

        case 2:
            fprintf(stderr, "Paramètres de la ligne de commande incorrects.\n");
            Sutils_Help(argv[0]);

            return EXIT_FAILURE;
    }

    /* Création du damier */
    damier jeu;
    switch(Sjeu_Initialiser(&config, &jeu, FALSE))
    {
        case 1:
            fprintf(stderr, "Le fichier de configuration contient des valeurs "
                "non prévues. Utilisation du damier par défaut.\n");

            Sjeu_Initialiser(&config, &jeu, TRUE);
            break;

        case 2:
            fprintf(stderr, "Le fichier de configuration contient des lignes "
                "dont la longueur n'est pas identique. Utilisation du damier "
                "par défaut.\n");

            Sjeu_Initialiser(&config, &jeu, TRUE);
            break;

        case 3:
            fprintf(stderr, "Le fichier de configuration ne contient aucune "
            "ligne valide.\n");
            break;

        case 4:
            fprintf(stderr, "Le fichier de configuration spécifié est "
            "introuvable.\n");
            Sjeu_Initialiser(&config, &jeu, TRUE);
            break;
    }

    damier historique[H_TAILLE];

    /* Variable interne utilisée pour la saisie. */
    char cont;

    int i, read, coord[4];

    printf("Bienvenue dans le jeu du Solitaire !");

    for(i = 0; jeu.nb_pion > 1; i++)
    {
        historique[i] = jeu;

        printf("\n\nTour n°%d - grille de jeu :\n", i + 1);

        if(Sjeu_Afficher(jeu, config) != 0)
        {
            printf("Le damier contient des valeurs non prévues." 
                   "Sortie du programme.\n" 
                   "Merci d'envoyer un rapport d'erreur à " AUTEUR ".");

            return EXIT_FAILURE;
        }

        cont = 0;

        /* Menu principal */
        while(cont != 'q')
        {
            printf("Jouer (h pour l'aide) : ");

            char userinput[MAX_INPUT], c = 0;
            read = 0;

            while((c = getchar()) != '\n' && i < MAX_INPUT - 1)
                userinput[read++] = c;

            userinput[read + 1] = 0;  
            cont = userinput[0];

            /* Aucun caractère lu : on affiche l'aide */
            if(read == 0)
            {
                read = 1;
                cont = 'h';
            }

            /* Un seul caractère : une commande */
            if(read == 1)
            {
                switch(cont)
                {
                    case 'h':
                        printf("Aide :\n"
                            "AOAO : coordonnées du déplacement\n"
                            "\t\tA : abscisse (A - Z)\n"
                            "\t\tO : ordonnée (1 - 26)\n"
                            "\tp : Annuler le déplacement précédent\n"
                            "\tq : Quitter\n");
                        break;

                    case 'p':
                        if(i > 0)
                        {
                            i--;
                            printf("Retour au tour n°%d\n", i + 1);
                            jeu = historique[i];
                            Sjeu_Afficher(jeu, config);
                        }
                        else
                            printf("Tour n°%d : impossible de charger le "
                                "tour précédent.\n", i + 1);

                        break;

                    case 'q':
                        printf("Le jeu est terminé, merci d'avoir joué !\n");
                        return EXIT_SUCCESS;
                }
            }

            /* Entre 4 et 6 caractères : des coordonnées */
            if(read >= 4 && read <= 6)
            {
                switch(Sutils_ToCoord(userinput, coord, config))
                {
                    case 0:
                        break;

                    case 1:
                        fprintf(stderr, "Votre saisie contient des caractères "
                            "non valides.\n");
                        continue;

                    case 2:
                        fprintf(stderr, "Votre saisie contient des caractères "
                            "hors du damier.\n");
                        continue;
                }

                break;
            }
        }

        printf("\n");

        switch(Sjeu_Jouer(&jeu, &config, coord))
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

    printf("Vous avez gagné, félicitations ! Merci d'avoir joué.\n");
    return EXIT_SUCCESS;
}
