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
 * \param def mode de création du damier.
 *    - 0 : utilisation du damier fourni en paramètre.
 *    - 1 : utilisation du damier par défaut.
 *
 * \return une instance de damier contenant une matrice table initialisée
 *    conformément au sujet.
 */
extern int initJeu(options* config, damier* jeu, int def)
{
    const int FUNC_SUCCESS         = 0;
    const int FUNC_ILLEGAL_CHAR    = 1;
    const int FUNC_LINES_NOT_EQUAL = 2;
    const int FUNC_TOO_FEW_LINES   = 3;
    const int FUNC_FILE_NOT_FOUND  = 4;


    if(config->confExists == TRUE && def == FALSE)
    {
        int i = 0, l, c, refWidth;
        char tabString[MAX_TAILLE][MAX_TAILLE];

        FILE* stream;

        if((stream = fopen(config->confPath, "r")) == NULL) return FUNC_FILE_NOT_FOUND;

        while(fscanf(stream, "%26s", tabString[i]) != EOF) i++;

        if(i == 0) return FUNC_TOO_FEW_LINES;

        fclose(stream);

        #ifdef DEBUG
        printf("Fichier lu (%d lignes) : \n", i);
        for(l = 0; l < i; l++)
            printf("%s\n", tabString[l]);
        #endif

        refWidth = (int)strlen(tabString[0]);

        /* Vérification de la longueur des lignes du damier lu */
        for(l = 0; l < i; l++)
            if((int)strlen(tabString[l]) != refWidth)
                return FUNC_LINES_NOT_EQUAL;

        config->confWidth = refWidth;
        config->confLength = i;

        /* Allocation de jeu->table */
        jeu->table = malloc(config->confWidth * sizeof(cases*));

        for(l = 0; l < config->confWidth; l++)
            jeu->table[l] = malloc(config->confLength * sizeof(cases));

        for(l = 0; l < config->confLength; l++)
        {
            for(c = 0; c < config->confWidth; c++) // ;-)
            {
                switch(tabString[l][c]) 
                {
                    case 'v':
                    case 'V':
                        jeu->table[c][l] = VIDE;
                        break;
                    
                    case '.':
                        jeu->table[c][l] = LIBRE;
                        break;

                    case '*':
                        jeu->table[c][l] = PION;
                        break;

                    default:
                        return FUNC_ILLEGAL_CHAR;
                        break;
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
            jeu->table[i] = malloc(T_TAILLE * sizeof(cases));

        for(i = 0; i < T_TAILLE; i++)
            for(j = 0; j < T_TAILLE; j++)
                jeu->table[j][i] = temp[i][j];
            
        config->confWidth = T_TAILLE;
        config->confLength = T_TAILLE;
    }

    jeu->nb_pion = 24;

    return FUNC_SUCCESS;
}


/** Affiche le damier sur la sortie standard.
 * \param jeu une instance du type damier.
 * \param config l'instance d'options qui configure l'application.
 * \return code de statut
 *    - 0 l'affichage s'est déroulé correctement.
 *    - 1 le damier contient des valeurs non prévues.
 */
extern int afficher(damier jeu, options config)
{
    int f, i, j, statut = 0;

    printf("\n      ");

    for(f = 0; f < config.confWidth; f++)
        printf("%c   ", 'A' + f);

    printf("\n   -");

    for(f = 0; f < config.confWidth; f++)
        printf("----");

    printf("\n");

    for(i = 0; i < config.confLength; i++)
    {
        printf("%d |  ", i + 1);

        for(j = 0; j < config.confWidth; j++)
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
extern int jouer(damier* jeu, options* config, int* coord)
{
    const int FUNC_SUCCESS      = 0;
    const int FUNC_DEP_FAULT    = 1;
    const int FUNC_ARR_FAULT    = 2;
    const int FUNC_ILLEGAL_MOVE = 3;
    const int FUNC_BAD_DISTANCE = 4;
    
    coordonnees depart, arrivee;

    depart.a = coord[0];
    depart.o = coord[1];
    arrivee.a = coord[2];
    arrivee.o = coord[3];

    /* Test case de départ */
    if(jeu->table[depart.a][depart.o] != PION)
        return FUNC_DEP_FAULT;

    /* Test case d'arrivée */
    if(jeu->table[arrivee.a][arrivee.o] != LIBRE)
        return FUNC_ARR_FAULT;

    /* Test mouvement horizontal / vertical autorisé */
    if((config->n == FALSE) && 
        ((depart.a == arrivee.a) || (depart.o == arrivee.o)))
        return FUNC_ILLEGAL_MOVE;

    /* Test mouvement diagonal autorisé */
    if((config->d == FALSE) && 
        ((depart.a != arrivee.a) && (depart.o != arrivee.o)))
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
