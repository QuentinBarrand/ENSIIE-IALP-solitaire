/** \file jeu.c 
 * Fonctions relatives à la gestion du jeu.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#include "util.h"
#include "jeu.h"

/** Initialise une instance de damier et la renvoie au programme principal.
 * \return une instance de damier contenant une matrice table initialisée
 *    conformément au sujet.
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


/** Affiche le damier sur la sortie standard.
 * \param jeu une instance du type damier
 * \return code de statut
 *    - 0 l'affichage s'est déroulé correctement
 *    - 1 : le damier contient des valeurs non prévues
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


/** Joue un coup.
 * \param jeu une instance de damier.
 * \param config la configuration de l'application.
 * \param depart les coordonnées de la case de départ.
 * \param arrivee les coordonnées de la case d'arrivée.
 * \return un code de statut
 *    - 1 : la case de départ n'est pas occupée par un pion
 *    - 2 : la case d'arrivée n'est pas libre
 *    - 3 : le mouvement n'est pas autorisé par les options
 *    - 4 : la distance entre les deux cases n'est pas égale à 2
 */
int jouer(damier* jeu, options* config, coordonnees depart, coordonnees arrivee)
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
    if((MAX(depart.a, arrivee.a) - MIN(depart.a, arrivee.a) != 2) &&
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
