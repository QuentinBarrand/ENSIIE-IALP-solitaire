/** 
 * \file util.c 
 * \brief Fonctions utilitaires de l'application.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#include "util.h"
#include "consts.h"

/* Prototypes statiques */
static int getOrdByNumber(char);
static int getOrdByTwoNumbers(char, char);
static int getAbsByLetter(char);

/** Affiche l'usage du programme.
 *
 * \param executable le nom du programme (correspond à <tt>argv[0]</tt>).
 */
void help(char* executable)
{
    printf("Usage : %s [options]\n"
        "Options :\n"
        "-h    Affiche l'aide et quitte le programme.\n"
        "-n    Autorise les coups horizontaux et verticaux (par défaut).\n"
        "-d    Autorise les coups en diagonale.\n"
        "<str> Spécifie le chemin vers le fichier de configuration du "
        "damier.\n", executable);
}


/** Récupère les options de la ligne de commande
 *
 * \param config la configuration actuelle de l'application.
 * \param argc le nombre d'arguments depuis la ligne de commande.
 * \param argv la matrice représentant les arguments de la ligne de commande.
 *
 * \return un code de statut: 
 *    - 0 : tous les paramètres ont été lus correctement
 *    - 1 : a affiché l'aide
 *    - 2 : une erreur de lecture des paramètres est survenue
 */
int getOptions(options* config, int argc, char** argv)
{
    const int FUNC_SUCCESS         = 0;
    const int FUNC_PRINTED_HELP    = 1;
    const int FUNC_INCORRECT_PARAM = 2;

    int i;

    config->n = FALSE;
    config->d = FALSE;
    config->confExists = FALSE;

    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    help(argv[0]);
                    return FUNC_PRINTED_HELP;

                case 'n':
                    config->n = 1;
                    continue;

                case 'd':
                    config->d = 1;
                    continue;

                default:
                    return FUNC_INCORRECT_PARAM;
            }
        }
        else
            /* Il s'agit du chemin vers le fichier de configuration
             * du damier. Il ne doit pas pouvoir être spécifié plusieurs fois.
             */
            if(config->confExists == FALSE)
            {
                config->confPath = malloc(MAX_PATH);
                strcpy(config->confPath, argv[i]);
                config->confExists = TRUE;
            }
    }

    /* Par défaut on active l'option -n */
    if(config->n == FALSE && config->d == FALSE)
    {
        config->n = TRUE;

        #ifdef DEBUG
        printf("Utilisation du déplacement vertical et horizontal "
            "par défaut.\n");
        #endif
    }

    return FUNC_SUCCESS;
}


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
int toCoord(char* userinput, int* coord, options config)
{
    const int FUNC_SUCCESS        = 0;
    const int FUNC_INCORRECT_CHAR = 1;
    const int FUNC_OUT_OF_BOUNDS  = 2;

    int i, j = 0, current;

    for(i = 0; i < (int)strlen(userinput); i++)
    {
        #ifdef DEBUG
        printf("Caractère %c\n", userinput[i]);
        #endif

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
                #ifdef DEBUG
                printf("Traitement de deux nombre : %c%c ==> %d\n", 
                    userinput[i], userinput[i + 1], current);
                #endif

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

    #ifdef DEBUG
        for(i = 0; i < 4; i++)
            printf("Nombre %d\n", coord[i]);
    #endif

    if(coord[0] + 1 > config.confWidth ||
        coord[1] + 1 > config.confLength ||
        coord[2] + 1 > config.confWidth ||
        coord[3] + 1 > config.confLength)
        return FUNC_OUT_OF_BOUNDS;

    return FUNC_SUCCESS;
}


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


