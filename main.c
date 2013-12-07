/**
 * \file main.c
 * \brief Fichier principal et point d'entrée de la solution. 
 * 
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#include <stdio.h>
#include <stdlib.h>


#include "lib/Sjeu.h"
#include "lib/Sconsts.h"

/** Fonction main : point d'entrée de l'application. */
int main(int argc, char** argv) 
{
    /* Test des arguments */
    options config;
    switch(Sjeu_LoadOptions(&config, argc, argv))
    {
        case 1:
            /* Si l'aide a été affichée, le programme se termine 
             * correctement.
             */
            return EXIT_SUCCESS;

        case 2:
            fprintf(stderr, "Paramètres de la ligne de commande "
                "incorrects.\n");
            Sjeu_Aide(argv[0]);

            return EXIT_FAILURE;
    }

    /* On joue ! */
    return Sjeu_New(&config);
}
