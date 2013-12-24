/** 
 * \file Sjeu.c 
 * \brief Fonctions relatives à la gestion du jeu.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
 
#include "Sconsts.h"
#include "Sgui.h"
#include "Sjeu.h"

#include "../lib/coordutils.h"
#include "../lib/Stack.h"

/** Taille des cotés du damier par défaut. */
#define T_TAILLE 7

/** Taille maximum d'un côté du damier. */
#define MAX_TAILLE 26

/** Renvoie le maximum de deux valeurs/ */
#define MAX(x, y) (((x) > (y) ? (x) : (y)))

/** Renvoie le minimum de deux valeurs. */
#define MIN(x, y) (((x) < (y) ? (x) : (y)))


/* Prototypes statiques */
static void Sjeu_Afficher(damier*);
static void Sjeu_AnnulerCoup(damier*, coup);
static int  Sjeu_CoupPossible(damier*, options*, coup);
static int  Sjeu_Initialiser(options*, damier*, int);
static int  Sjeu_EstTermine(damier*, options*, coup*);
static int  Sjeu_Jouer(damier*, options*, coup);


/*
 * Fonction externes
 */


/** Affiche l'usage du programme.
 *
 * \param executable le nom du programme (correspond à <tt>argv[0]</tt>).
 */
extern void Sjeu_Aide(char* executable)
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
 * \return un code de statut : 
 *    - 0 : tous les paramètres ont été lus correctement.
 *    - 1 : a affiché l'aide.
 *    - 2 : une erreur de lecture des paramètres est survenue.
 */
extern int Sjeu_LoadOptions(options* config, int argc, char** argv)
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
                    Sjeu_Aide(argv[0]);
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
                config->confPath = malloc(strlen(argv[i]));
                strcpy(config->confPath, argv[i]);
                config->confExists = TRUE;
            }
    }

    /* Par défaut on active l'option -n */
    if(config->n == FALSE && config->d == FALSE)
        config->n = TRUE;

    return FUNC_SUCCESS;
}


/** Point d'entrée du module de jeu. Créé un nouveau jeu.
 *
 * \param config l'instance d'options de l'application.
 *
 * \return le code de retour à retourner au processus père.
 */
extern int Sjeu_New(options* config)
{
    Sgui_Initialiser();

    /* Génération damier */
    damier jeu;
    char* error;
    int init = Sjeu_Initialiser(config, &jeu, FALSE);

    switch(init)
    {
        case 1:
            error = "Le fichier de configuration contient des valeurs non "
                "prévues. Utilisation du damier par défaut.";
            Sjeu_Initialiser(config, &jeu, TRUE);

            break;

        case 2:
            error = "Le fichier de configuration contient des lignes dont "
                "la longueur n'est pas identique. Utilisation du damier par "
                "défaut.";
            Sjeu_Initialiser(config, &jeu, TRUE);

            break;

        case 3:
            error = "Le fichier de configuration ne contient aucune ligne "
                "valide.";
            Sjeu_Initialiser(config, &jeu, TRUE);

            break;

        case 4:
            error = "Le fichier de configuration spécifié est introuvable.";
            Sjeu_Initialiser(config, &jeu, TRUE);

            break;
    }

    /* Affichage du splash screen - en RELEASE uniquement */
    #ifndef DEBUG
    Sgui_Splash();
    if(init != 0) Sgui_StartupError(error);

    getch();
    clear();
    #endif

    int coord[4], statut_jeu, help_printed = FALSE;
    char hint[30];

    Stack* histo = Stack_New(25);
    coup possible;

    while(TRUE)
    {
        statut_jeu = Sjeu_EstTermine(&jeu, config, &possible);

        if(statut_jeu == 1)
        {
            attron(COLOR_PAIR(3));
            mvprintw(7 + jeu.length, 4, "Vous avez gagné, bravo !");
            mvprintw(8 + jeu.length, 4, "Appuyez sur une touche pour quitter.");
            attron(COLOR_PAIR(1));
            
            break;
        }
        else if(statut_jeu == 2)
        {
            attron(COLOR_PAIR(2));
            mvprintw(7 + jeu.length, 4, "Vous avez perdu :-(");
            mvprintw(8 + jeu.length, 4, "Appuyez sur une touche pour quitter.");
            attron(COLOR_PAIR(1));
            break;
        }

        Sjeu_Afficher(&jeu);
    
        char userinput[MAX_INPUT];
        
        int nbchars = Sgui_ReadCoup(userinput);

        /* Nombre de caractères saisis incorrect */
        if(nbchars == 0 || (nbchars > 1 && nbchars < 4) || nbchars > 4)
        {
            Sgui_RuntimeMessage("Merci de saisir une commande valide.", ERROR);
            continue;
        }

        coup* a_annuler;

        /* Une seule lettre : une commande */
        if(nbchars == 1)
        {
            switch(userinput[0])
            {
                /* Aide */
                case '?':
                    if(help_printed == TRUE)
                    {
                        Sgui_Help(jeu.width, FALSE);
                        help_printed = FALSE;
                    }
                    else
                    {
                        Sgui_Help(jeu.width, TRUE);
                        help_printed = TRUE;
                    }
                    
                    mvhline(LINES - 2, 36, ' ', COLS);
                    continue;

                /* Hint : indication */
                case 'I':
                case 'i':
                    sprintf(hint, "Indication : %c%d%c%d",
                        'A' + possible.depart.a,
                        possible.depart.o + 1,
                        'A' + possible.arrivee.a,
                        possible.arrivee.o + 1);
                    Sgui_RuntimeMessage(hint, INFO);

                    continue;

                /* Tour précédent */
                case 'P':
                case 'p':
                    if((a_annuler = (coup*)Stack_Pop(histo)) == NULL)
                    {
                        Sgui_RuntimeMessage("Impossible de charger le tour "
                            "précédent.", ERROR);
                    }
                    else
                    {                        
                        Sgui_RuntimeMessage("Retour au tour précédent.", 
                            SUCCESS);
                        Sjeu_AnnulerCoup(&jeu, *a_annuler);
                    }

                    continue;
                
                /* Quitter */
                case 'Q':
                case 'q':
                    Sgui_Terminer();

                    return EXIT_SUCCESS;

                default:
                    Sgui_RuntimeMessage("Commande non reconnue.", ERROR);

                    continue;
            }
        }
    
        /* 4 caractères : les coordonnées d'un coup */
        switch(coordutils_ToIntCoord(userinput, coord, jeu.width, jeu.length))
        {
            case 1:
                Sgui_RuntimeMessage("Votre saisie contient des caractères "
                    "non standards.", ERROR);
                continue;

            case 2:
                Sgui_RuntimeMessage("Votre saisie contient des valeurs "
                    "menant hors du damier.", ERROR);
                continue;            
        }
        
        coup* current_coup = (coup*)malloc(sizeof(coup));
        current_coup->depart.a  = coord[0];
        current_coup->depart.o  = coord[1];
        current_coup->arrivee.a = coord[2];
        current_coup->arrivee.o = coord[3];

        switch(Sjeu_Jouer(&jeu, config, *current_coup))
        {
            case 0:
                Sjeu_Afficher(&jeu);
                Sgui_RuntimeMessage("Le coup a bien été joué !", SUCCESS);

                Stack_Push(current_coup, histo);

                continue;

            case 1:
                Sgui_RuntimeMessage("La case de départ n'est pas occupée par"
                    " un pion.", ERROR);
                break;

            case 2:
                Sgui_RuntimeMessage("La case d'arrivée n'est pas libre.", 
                    ERROR);
                break;

            case 3:
                Sgui_RuntimeMessage("La case centrale n'est pas occupée par "
                    "un pion.", ERROR);
                break;

            case 4:
                Sgui_RuntimeMessage("Le mouvement n'est pas autorisé par "
                    "les optons de la ligne de commande.", ERROR);
                break;

            case 5:
                Sgui_RuntimeMessage("La distance entre deux cases n'est pas "
                    "égale à 2.", ERROR);
                break;
        }
    }

    getch();

    Sgui_Terminer();

    return FALSE;
}


/*
 * Fonction statiques
 */


/** Affiche le damier dans la fenêtre principale.
 *
 * \param jeu l'instance de damier du jeu.
 */
static void Sjeu_Afficher(damier* jeu)
{
    int c, l;

    int x_offset = 2;
    int y_offset = 4;

    mvprintw(1, 7, "Solitaire v" VERSION);

    for(c = 0; c < jeu->width; c++)
    {
        /* Affichage de la première ligne (lettres) */
        mvprintw(y_offset, x_offset + 5 + (3 * c), "%c", 'A' + c);
        mvhline(y_offset + 1, x_offset + 3, '-', (jeu->width * 3));
    }

    for(l = 0; l < jeu->length; l++)
    {
        /* Affichage de la première colonne (nombres) */
        mvprintw(y_offset + 2 + l, x_offset, "%d", l + 1);
        mvvline (y_offset + 2, x_offset + 2, '|', jeu->length);
    }

    for(l = 0; l < jeu->length; l++)
    {
        for(c = 0; c < jeu->width; c++)
        {
            switch(jeu->table[c][l])
            {
                case VIDE:
                    mvprintw(y_offset + 2 + l, x_offset + 5 + (3 * c), " ");
                    break;

                case LIBRE:
                    mvprintw(y_offset + 2 + l, x_offset + 5 + (3 * c), "-");
                    break;

                case PION:
                    mvprintw(y_offset + 2 + l, x_offset + 5 + (3 * c), "#");
                    break;
            }
        }
    }

    mvprintw(y_offset + l + 3, 4, "Il reste %d pion(s)", jeu->nb_pion);
}


/** Retourne la case centrale d'un coup (entre la case de départ et celle
 * d'arrivée).
 *
 * \param current_coup le coup dont on souhaite obtenir la case centrale.
 *
 * \return les coordonnées de la case centrale du coup.
 */
static coordonnees Sjeu_CaseCentrale(coup current_coup)
{
    coordonnees centrale;

    if(current_coup.depart.a == current_coup.arrivee.a)
        centrale.a = current_coup.depart.a;
    else 
        centrale.a = MAX(current_coup.depart.a, current_coup.arrivee.a) - 1;

    if(current_coup.depart.o == current_coup.arrivee.o)
        centrale.o = current_coup.depart.o;
    else
        centrale.o = MAX(current_coup.depart.o, current_coup.arrivee.o) - 1;

    return centrale;
}


/** Vérifie si un coup est jouable.
 *
 * \param jeu une instance de damier.
 * \param config la configuration de l'application.
 * \param current_coup le coup à jouer.
 *
 * \return un code de statut
 *    - 1 : la case de départ n'est pas occupée par un pion.
 *    - 2 : la case d'arrivée n'est pas libre.
 *    - 3 : la case centrale n'est pas libre.
 *    - 4 : le mouvement n'est pas autorisé par les options.
 *    - 5 : la distance entre les deux cases n'est pas égale à 2.
 */
static int Sjeu_CoupPossible(damier* jeu, options* config, coup current_coup)
{
    const int FUNC_SUCCESS      = 0;
    const int FUNC_BAD_DEP      = 1;
    const int FUNC_BAD_ARR      = 2;
    const int FUNC_BAD_MIDDLE   = 3;
    const int FUNC_ILLEGAL_MOVE = 4;
    const int FUNC_BAD_DISTANCE = 5;

    coordonnees centrale = Sjeu_CaseCentrale(current_coup);

    /* Test case de départ */
    if(jeu->table[current_coup.depart.a][current_coup.depart.o] != PION)
        return FUNC_BAD_DEP;

    /* Test case d'arrivée */
    if(jeu->table[current_coup.arrivee.a][current_coup.arrivee.o] != LIBRE)
        return FUNC_BAD_ARR;

    /* Test case centrale */
    if(jeu->table[centrale.a][centrale.o] != PION)
        return FUNC_BAD_MIDDLE;

    /* Test mouvement horizontal / vertical autorisé */
    if((config->n == FALSE) && 
        ((current_coup.depart.a == current_coup.arrivee.a) || 
        (current_coup.depart.o == current_coup.arrivee.o)))
        return FUNC_ILLEGAL_MOVE;

    /* Test mouvement diagonal autorisé */
    if((config->d == FALSE) && 
        ((current_coup.depart.a != current_coup.arrivee.a) && 
        (current_coup.depart.o != current_coup.arrivee.o)))
        return FUNC_ILLEGAL_MOVE;

    /* Test distance égale à 2 */
    if((MAX(current_coup.depart.a, current_coup.arrivee.a) - 
        MIN(current_coup.depart.a, current_coup.arrivee.a) != 2) &&
       (MAX(current_coup.depart.o, current_coup.arrivee.o) - 
        MIN(current_coup.depart.o, current_coup.arrivee.o) != 2))
        return FUNC_BAD_DISTANCE;

    return FUNC_SUCCESS;
}


/** Vérifie si le jeu est terminé ou bloqué.
 *
 * \param jeu l'instance de damier.
 * \param config la configuration de l'application.
 * \param possible pointeur vers le premier coup possible (le cas échéant).
 *
 * \return un code de statut :
 *    - 0 : le jeu n'est pas terminé.
 *    - 1 : le jeu est gagné.
 *    - 2 : le jeu est bloqué.
 */
static int Sjeu_EstTermine(damier* jeu, options* config, coup* possible)
{
    const int FUNC_NOT_OVER = 0;
    const int FUNC_WIN      = 1;
    const int FUNC_BLOCKED  = 2;

    int l1, c1, l2, c2;
    coup coup_teste;

    for(c1 = 0; c1 < jeu->width; c1++)
    {
        for(l1 = 0; l1 < jeu->length; l1++)
        {
            for(c2 = 0; c2 < jeu->width; c2++)
            {
                for(l2 = 0; l2 < jeu->length; l2++)
                {
                    coup_teste.depart.a = c1;
                    coup_teste.depart.o = l1;
                    coup_teste.arrivee.a = c2;
                    coup_teste.arrivee.o = l2;

                    if(Sjeu_CoupPossible(jeu, config, coup_teste) == 0)
                    {
                        possible->depart.a = c1;
                        possible->depart.o = l1;
                        possible->arrivee.a = c2;
                        possible->arrivee.o = l2;

                        return FUNC_NOT_OVER;
                    }
                }
            }
        }
    }

    if(jeu->nb_pion == 1) return FUNC_WIN;
    else return FUNC_BLOCKED;
}


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
static int Sjeu_Initialiser(options* config, damier* jeu, int def)
{
    const int FUNC_SUCCESS         = 0;
    const int FUNC_ILLEGAL_CHAR    = 1;
    const int FUNC_LINES_NOT_EQUAL = 2;
    const int FUNC_TOO_FEW_LINES   = 3;
    const int FUNC_FILE_NOT_FOUND  = 4;


    if(config->confExists == TRUE && def == FALSE)
    {
        int i = 0, l, c;
        char tabString[MAX_TAILLE][MAX_TAILLE];

        FILE* stream;

        if((stream = fopen(config->confPath, "r")) == NULL) 
            return FUNC_FILE_NOT_FOUND;

        while(fscanf(stream, "%26s", tabString[i]) != EOF) i++;

        if(i == 0) return FUNC_TOO_FEW_LINES;

        fclose(stream);

        jeu->width = (int)strlen(tabString[0]);

        /* Vérification de la longueur des lignes du damier lu */
        for(l = 0; l < i; l++)
            if((int)strlen(tabString[l]) != jeu->width)
                return FUNC_LINES_NOT_EQUAL;

        jeu->length = i;
        jeu->nb_pion = 0;

        /* Allocation de jeu->table */
        jeu->table = malloc(jeu->width * sizeof(cases*));

        for(l = 0; l < jeu->width; l++)
            jeu->table[l] = malloc(jeu->length * sizeof(cases));

        for(l = 0; l < jeu->length; l++)
        {
            for(c = 0; c < jeu->width; c++) // ;-)
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
                        jeu->nb_pion++;
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

        jeu->width = T_TAILLE;
        jeu->length = T_TAILLE;

        jeu->nb_pion = 24;
    }

    return FUNC_SUCCESS;
}


/** Joue un coup.
 *
 * \param jeu une instance de damier.
 * \param config la configuration de l'application.
 * \param current_coup le coup à jouer.
 *
 * \return un code de statut
 *    - 1 : la case de départ n'est pas occupée par un pion
 *    - 2 : la case d'arrivée n'est pas libre
 *    - 3 : la case du milieu n'est pas libre
 *    - 4 : le mouvement n'est pas autorisé par les options
 *    - 5 : la distance entre les deux cases n'est pas égale à 2
 */
static int Sjeu_Jouer(damier* jeu, options* config, coup current_coup)
{   
    const int FUNC_SUCCESS = 0; 

    coordonnees centrale = Sjeu_CaseCentrale(current_coup);
    int coup_possible;

    if ((coup_possible = Sjeu_CoupPossible(jeu, config, current_coup)) != 0)
        return coup_possible;

    /* Modification du damier */
    jeu->table[current_coup.depart.a][current_coup.depart.o] = LIBRE;
    jeu->table[current_coup.arrivee.a][current_coup.arrivee.o] = PION;
    jeu->table[centrale.a][centrale.o] = LIBRE;

    jeu->nb_pion -= 1;

    return FUNC_SUCCESS;
}


/** Annule un coup.
 *
 * \param jeu l'instance du damier sur lequel annuler le coup.
 * \param a_annuler le coup à annuler.
 */
static void Sjeu_AnnulerCoup(damier* jeu, coup a_annuler)
{
    jeu->table[a_annuler.depart.a][a_annuler.depart.o] = PION;
    jeu->table[a_annuler.arrivee.a][a_annuler.arrivee.o] = LIBRE;

    coordonnees centrale = Sjeu_CaseCentrale(a_annuler);
    jeu->table[centrale.a][centrale.o] = PION;

    jeu->nb_pion++;
}