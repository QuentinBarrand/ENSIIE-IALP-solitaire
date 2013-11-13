/** 
 * \file jeu.c 
 * \brief Fonctions relatives à la gestion du jeu.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#include "util.h"
#include "jeu.h"

/** Initialise une instance de damier et la renvoie au programme principal.
 *
 * \param config l'instance d'options de l'application.
 * \param jeu l'instance de damier de l'application.
 *
 * \return une instance de damier contenant une matrice table initialisée
 *    conformément au sujet.
 */
int initJeu(options* config, damier* jeu)
{
    const int FUNC_SUCCESS         = 0;
    const int FUNC_ILLEGAL_CHAR    = 1;
    const int FUNC_LINES_NOT_EQUAL = 2;

    if(config->confExists == TRUE)
    {
        FILE* stream = fopen(config->confPath, "r");

        char* tabString[MAX_TAILLE], currentString;
        int i = 0, j = 0, c = 0, refLength = 0;

        while(fscanf(stream, "%s", &currentString) != EOF && i < MAX_TAILLE)
        {
            strcpy(tabString[i], currentString);
            i++;
        }

        fclose(stream);

        refLength = strlen(tabString[0]);

        /* Vérification de la conformité du damier lu */
        for(j = 1; j <= i; j++)
            if(strlen(tabString[j]) != refLength)
                return FUNC_LINES_NOT_EQUAL;

        config->confWidth = refLength;
        config->confLength = i;

        jeu->table = malloc(i * refLength);

        /* Import du damier lu dans la configuration */
        for(j = 0; j < i; j++)
        {
            for(c = 0; c < refLength; c++) // ;-)
            {
                switch(tabString[j][c]) 
                {
                    case 'v':
                    case 'V':
                        jeu->table[j][c] = VIDE;
                        break;
                    
                    case '.':
                        jeu->table[j][c] = LIBRE;
                        break;

                    case '*':
                        jeu->table[j][c] = PION;
                        break;

                    default:
                        return FUNC_ILLEGAL_CHAR;
                }
            }
        }
    }
    else
    {
        #ifdef DEBUG
        printf("Utilisation du damier par défaut.\n");
        #endif

        cases temp[T_TAILLE][T_TAILLE] = { 
            {VIDE, VIDE, VIDE, PION,  VIDE, VIDE, VIDE},
            {VIDE, PION, PION, PION,  PION, PION, VIDE},
            {VIDE, PION, VIDE, PION,  VIDE, PION, VIDE},
            {PION, PION, PION, LIBRE, PION, PION, PION},
            {VIDE, PION, VIDE, PION,  VIDE, PION, VIDE},
            {VIDE, PION, PION, PION,  PION, PION, VIDE},
            {VIDE, VIDE, VIDE, PION,  VIDE, VIDE, VIDE}
        };

        int i, j;

        jeu->table = malloc(T_TAILLE * sizeof(cases*));

        for(i = 0; i < T_TAILLE; i++)
        {
            jeu->table[i] = malloc(T_TAILLE * sizeof(cases));
            for(j = 0; j < T_TAILLE; j++)
                jeu->table[i][j] = temp[i][j];
        }
        
        jeu->nb_pion = 24;
    
        config->confWidth = T_TAILLE;
        config->confLength = T_TAILLE;

    }

    return FUNC_SUCCESS;
}


/** Affiche le damier sur la sortie standard.
 * \param jeu une instance du type damier.
 * \param config l'instance d'options qui configure l'application.
 * \return code de statut
 *    - 0 l'affichage s'est déroulé correctement
 *    - 1 le damier contient des valeurs non prévues
 */
int afficher(damier jeu, options config)
{
    int f, i, j, statut = 0;

    printf("\n      ");

    for(f = 0; f < config.confWidth; f++)
        printf("%c   ", 'A' + f);

    printf("\n   -");

    for(f = 0; f < config.confLength; f++)
        printf("----");

    printf("\n");

    for(i = 0; i < config.confLength; i++)
    {
        printf("%d |  ", i + 1);

        for(j = 0; j < config.confLength; j++)
        {
            switch(jeu.table[j][i])
            {
                case VIDE:
                    printf("   ");
                    break;

                case LIBRE:
                    printf(" - ");
                    break;

                case PION:
                    printf(" # ");
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
    const int FUNC_SUCCESS      = 0;
    const int FUNC_DEP_FAULT    = 1;
    const int FUNC_ARR_FAULT    = 2;
    const int FUNC_ILLEGAL_MOVE = 3;
    const int FUNC_BAD_DISTANCE = 4;

    /* Test case de départ */
    if(jeu->table[depart.a][depart.o] != PION)
        return FUNC_DEP_FAULT;

    /* Test case d'arrivée */
    if(jeu->table[arrivee.a][arrivee.o] != LIBRE)
        return FUNC_ARR_FAULT;

    /* Test mouvement horizontal / vertical autorisé */
    if(!config->n && (depart.a != arrivee.a || depart.o != arrivee.o))
        return FUNC_ILLEGAL_MOVE;

    /* Test mouvement diagonal autorisé */
    if(!config->d && (depart.a != arrivee.a || depart.o != arrivee.o))
        return FUNC_ILLEGAL_MOVE;

    /* Test distance égale à 2 */
    if((MAX(depart.a, arrivee.a) - MIN(depart.a, arrivee.a) != 2) &&
       (MAX(depart.o, arrivee.o) - MIN(depart.o, arrivee.o) != 2))
        return FUNC_BAD_DISTANCE;

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

    return FUNC_SUCCESS;
}
