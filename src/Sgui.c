/**
 * \file Sgui.c
 * \brief Fonctions de gestion de l'interface utilisateur.
 * 
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#include <curses.h>
#include <unistd.h>
#include <string.h>

#include "Sgui.h"

/* Prototypes statiques */
static void create_color();

/*
 * Fonctions externes
 */


/** Affiche l'aide au cours du jeu.
 *
 * \param table_width le nombre de colonnes du damier.
 * \param print Entier :
 *    - 0 : efface le contenu de la zone d'aide.
 *    - 1 : affiche le contenu de la done d'aide.
 */
extern void Sgui_Help(int table_width, int print)
{
    int x_offset = 6 + table_width * 3 + 10;
    int y_offset = 4;

    if(print == TRUE)
    {
        mvprintw(y_offset,     x_offset, "Aide :");
        mvprintw(y_offset + 2, x_offset, "LnnLnn : coordonnées");
        mvprintw(y_offset + 3, x_offset, "\tL : lettre");
        mvprintw(y_offset + 4, x_offset, "\tnn : chiffres");
        mvprintw(y_offset + 5, x_offset, "p : Revenir au coup "
            "précédent");
        mvprintw(y_offset + 6, x_offset, "i : Indication");
        mvprintw(y_offset + 8, x_offset, "? : Cacher cette "
            "aide");
        mvprintw(y_offset + 9, x_offset, "q : Quitter");
    }
    else
    {
        mvhline(y_offset,     x_offset, ' ', COLS - 1);
        mvhline(y_offset + 2, x_offset, ' ', COLS - 1);
        mvhline(y_offset + 3, x_offset, ' ', COLS - 1);
        mvhline(y_offset + 4, x_offset, ' ', COLS - 1);
        mvhline(y_offset + 5, x_offset, ' ', COLS - 1);
        mvhline(y_offset + 6, x_offset, ' ', COLS - 1);
        mvhline(y_offset + 7, x_offset, ' ', COLS - 1);
    }
}


/** Initialise la fenêtre curses de l'application.
 *
 * \return la fenêtre curses correctement initialisée.
 */
extern WINDOW* Sgui_Initialiser()
{
    WINDOW* app_window = initscr();

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    create_color();
    refresh();

    return app_window;
}


/** Lit la chaîne de caractères saisie par l'utilisateur.
 *
 * \param userinput une chaîne de caractères correctement allouée. Passée par
 *    référence.
 *
 * \return la longueur de la chaîne lue.
 */
extern int Sgui_ReadCoup(char* userinput)
{
    echo();
    mvprintw(LINES - 2, 0, "Saisissez un coup (? pour l'aide) : ");
    getstr(userinput);
    noecho();

    return strlen(userinput);
}


/** Affiche un message pendant l'exécution de l'application.
 *
 * \param message le message à afficher.
 * \param type le type de message à afficher:
 *    - SUCCES : message de succès, en vert.
 *    - ERROR : message d'erreur, en rouge.
 */
extern void Sgui_RuntimeMessage(char* message, message_type type)
{
    if(type == SUCCESS)
        attron(COLOR_PAIR(3));
    else if(type == ERROR)
        attron(COLOR_PAIR(2));

    mvprintw(LINES - 1, 0, message);
    refresh();

    create_color();
    attron(COLOR_PAIR(1));
    sleep(3);
    mvhline(LINES - 1, 0, ' ', COLS);
    mvhline(LINES - 2, 36, ' ', COLS - 36);
    refresh();
}


/** Affiche l'écrand de lancement. */
extern void Sgui_Splash()
{
    #ifdef VERSION
    mvprintw(0, 0, "Version : " VERSION);
    #endif

    #ifdef BUILD
    mvprintw(1, 0, "Build : " BUILD);
    #endif

    int x_offset = (LINES / 2) - 4;
    int y_offset = (COLS / 2) - 31;

    if(COLS >= 65)
    {
        /* THE splash ;-) */
        mvprintw(x_offset + 0, y_offset, 
            " ____            ___        __                                ");
        mvprintw(x_offset + 1, y_offset, 
            "/\\  _`\\         /\\_ \\    __/\\ \\__           __           "
            "     ");
        mvprintw(x_offset + 2, y_offset, 
            "\\ \\,\\L\\_\\    ___\\//\\ \\  /\\_\\ \\ ,_\\    __   /\\_\\"
            "  _ __    __   ");
        mvprintw(x_offset + 3, y_offset, 
            " \\/_\\__ \\   / __`\\\\ \\ \\ \\/\\ \\ \\ \\/  /'__`\\ \\/\\ \\"
            "/\\`'__\\/'__`\\ ");
        mvprintw(x_offset + 4, y_offset, 
            "   /\\ \\L\\ \\/\\ \\L\\ \\\\_\\ \\_\\ \\ \\ \\ \\_/\\ \\L\\.\\_"
            "\\ \\ \\ \\ \\//\\  __/ ");
        mvprintw(x_offset + 5, y_offset, 
            "   \\ `\\____\\ \\____//\\____\\\\ \\_\\ \\__\\ \\__/.\\_\\\\ "
            "\\_\\ \\_\\\\ \\____\\");
        mvprintw(x_offset + 6, y_offset, 
            "    \\/_____/\\/___/ \\/____/ \\/_/\\/__/\\/__/\\/_/ \\/_/\\/_/"
            " \\/____/");
    }
    else
        mvprintw(LINES / 2, COLS / 2 - 4, "SOLITAIRE");

    mvprintw(x_offset + 8, COLS / 2 - 14, "ENSIIE / IALP - FIPA6 - 2013");  

    mvprintw(LINES - 2, COLS / 2 - 20, "Appuyez sur une touche pour "
        "commencer !");  
}


/** Affiche un message au démarrage de l'application.
 *
 * \param message le message à afficher.
 */
extern void Sgui_StartupError(char* message)
{
    int x_offset = (LINES / 2) + 6;

    create_color();
    attron(COLOR_PAIR(2));
    mvprintw(x_offset, (COLS / 2) - (strlen(message) / 2), message);
    attron(COLOR_PAIR(1));
}


/** Termine la fenêtre principale de l'application. */
extern void Sgui_Terminer()
{
    clear();
    mvprintw(LINES / 2, COLS / 2 - 17, "Merci d'avoir joué, à bientôt !");
    refresh();
    sleep(3);
    endwin();
}


/*
 * Fonctions statiques
 */

static void create_color()
{
    start_color();
    init_pair(1,  COLOR_WHITE, COLOR_BLACK);
    init_pair(2,  COLOR_RED,   COLOR_BLACK);
    init_pair(3,  COLOR_GREEN, COLOR_BLACK);
}