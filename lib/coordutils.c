/** 
 * \file Sutils.c 
 * \brief Fonctions utilitaires de l'application.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#include <stdio.h>
#include <string.h>

#include "coordutils.h"

/* Prototypes statiques */
static int getOrdByNumber(char);
static int getOrdByTwoNumbers(char, char);
static int getAbsByLetter(char);

/* 
 * Fonctions externes
 */


/** Retourne les coordonnées du déplacement calculées depuis la saisie 
 * alphanumérique de l'utilisateur.
 *
 * \param userinput saisie de l'utilisateur.
 * \param coord coordonnées calculées par la fonction et passées par adresse 
 *    pour récupérer son contenu.
 * \param config la configuration de l'application.
 *
 * \return un code de statut :
 *    - 0 le calcul s'est déroulé correctement.
 *    - 1 la saisie contient des caractères non standards.
 *    - 2 la saisie contient des valeurs menant hors du damier.  
 */
extern int coordutils_ToIntCoord(char* userinput, int* coord, int width, int length)
{
    const int FUNC_SUCCESS        = 0;
    const int FUNC_INCORRECT_CHAR = 1;
    const int FUNC_OUT_OF_BOUNDS  = 2;

    int i, j = 0, current;

    for(i = 0; i < (int)strlen(userinput); i++)
    {
        /* Si on est sur une coordonnée paire (0, 2) */
        if(j % 2 == 0)
        {
            if((current = getAbsByLetter(userinput[i])) != -1)
            {
                coord[j] = current;
                j++;
                continue;
            }
        }
        else
        {
            if((current = 
                getOrdByTwoNumbers(userinput[i], userinput[i + 1])) != -1)
            {
                coord[j] = current;
                i++;
                j++;
                continue;
            }

            if((current = getOrdByNumber(userinput[i])) != -1)
            {
                coord[j] = current;
                j++;
                continue;
            }
        }

        return FUNC_INCORRECT_CHAR;
    }

    if(coord[0] + 1 > width ||
        coord[1] + 1 > length ||
        coord[2] + 1 > width ||
        coord[3] + 1 > length)
        return FUNC_OUT_OF_BOUNDS;

    return FUNC_SUCCESS;
}


/* 
 * Fonctions statiques
 */


/** Retourne la valeur d'un caractère numérique.
 *
 * \param c le caractère dont on souhaite obtenir la valeur entière.
 *
 * \return un entier la valeur de l'entier (>= 0) ou -1 en cas d'erreur.
 */
static int getOrdByNumber(char c)
{
    const int FUNC_NOT_A_NUMBER = -1;

    return (c >= '1' && c <= '9') ? c - '0' - 1 : FUNC_NOT_A_NUMBER;
}


/** Retourne la valeur d'un nombre constitué de deux chiffres au format char.
 *
 * \param c1 le premier chiffre du nombre.
 * \param c2 le deuxième chiffre du nombre.
 *
 * \return le nombre au format int (>= 0) ou -1 en cas d'erreur.
 */
static int getOrdByTwoNumbers(char c1, char c2)
{
    const int FUNC_NOT_A_NUMBER = -1;

    int p1 = getOrdByNumber(c1) + 1;
    int p2 = getOrdByNumber(c2) + 1;

    if(p1 == 0 || p2 == 0) return FUNC_NOT_A_NUMBER;

    return 10 * p1 + p2;
}


/** Retourne l'index du caractère spécifié dans l'alphabet.
 *
 * \param c le caractère dont on souhaite obtenir l'index.
 *
 * \return l'index du caractère (>= 0) ou -1 en cas d'erreur.
 */
static int getAbsByLetter(char c)
{
    const int FUNC_NOT_A_LETTER = -1;

    if(c >= 'A' && c <= 'Z') return c - 'A';
    if(c >= 'a' && c <= 'z') return c - 'a';

    return FUNC_NOT_A_LETTER;
}
