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


extern void Sgui_ReadCoup(char* userinput)
{
    echo();
    mvprintw(LINES - 2, 0, "Saisissez un coup (? pour l'aide) : ");
    getstr(userinput);
    noecho();
}


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


extern void Sgui_RuntimeMessage(WINDOW* app_window, char* message, 
    message_type type)
{
    create_color();

    if(type == SUCCESS)
        attron(COLOR_PAIR(3));
    else if(type == ERROR)
        attron(COLOR_PAIR(2));

    mvwprintw(app_window, LINES - 1, 0, message);
    sleep(3);

    attron(COLOR_PAIR(1));
    mvwhline (app_window, LINES - 1, 0, ' ', COLS);
    mvwhline (app_window, LINES - 2, 36, ' ', COLS - 36);
    refresh();
}


extern void Sgui_Splash(WINDOW* app_window)
{
    #ifdef VERSION
    mvwprintw(app_window, 0, 0, "Version : " VERSION);
    #endif

    #ifdef BUILD
    mvwprintw(app_window, 1, 0, "Build : " BUILD);
    #endif

    int x_offset = (LINES / 2) - 4;
    int y_offset = (COLS / 2) - 31;

    if(COLS >= 65)
    {
        /* THE splash ;-) */
        mvwprintw(app_window, x_offset + 0, y_offset, 
            " ____            ___        __                                ");
        mvwprintw(app_window, x_offset + 1, y_offset, 
            "/\\  _`\\         /\\_ \\    __/\\ \\__           __           "
            "     ");
        mvwprintw(app_window, x_offset + 2, y_offset, 
            "\\ \\,\\L\\_\\    ___\\//\\ \\  /\\_\\ \\ ,_\\    __   /\\_\\"
            "  _ __    __   ");
        mvwprintw(app_window, x_offset + 3, y_offset, 
            " \\/_\\__ \\   / __`\\\\ \\ \\ \\/\\ \\ \\ \\/  /'__`\\ \\/\\ \\"
            "/\\`'__\\/'__`\\ ");
        mvwprintw(app_window, x_offset + 4, y_offset, 
            "   /\\ \\L\\ \\/\\ \\L\\ \\\\_\\ \\_\\ \\ \\ \\ \\_/\\ \\L\\.\\_"
            "\\ \\ \\ \\ \\//\\  __/ ");
        mvwprintw(app_window, x_offset + 5, y_offset, 
            "   \\ `\\____\\ \\____//\\____\\\\ \\_\\ \\__\\ \\__/.\\_\\\\ "
            "\\_\\ \\_\\\\ \\____\\");
        mvwprintw(app_window, x_offset + 6, y_offset, 
            "    \\/_____/\\/___/ \\/____/ \\/_/\\/__/\\/__/\\/_/ \\/_/\\/_/"
            " \\/____/");
    }
    else
        mvwprintw(app_window, LINES / 2, COLS / 2 - 4, "SOLITAIRE");

    mvwprintw(app_window, x_offset + 8, COLS / 2 - 14, "ENSIIE / IALP - FIPA6 "
        "- 2013");  

    mvwprintw(app_window, LINES - 2, COLS / 2 - 20, "Appuyez sur une touche "
        "pour commencer !");  
}


extern void Sgui_StartupError(WINDOW* app_window, char* message)
{
    int x_offset = (LINES / 2) + 6;

    create_color();
    attron(COLOR_PAIR(2));
    mvwprintw(app_window, x_offset, (COLS / 2) - (strlen(message) / 2),
        message);
    attron(COLOR_PAIR(1));
}


extern void Sgui_Terminer(WINDOW* app_window)
{
    clear();
    mvwprintw(app_window, LINES / 2, COLS / 2 - 17, "Merci d'avoir joué, à "
        "bientôt !");
    refresh();
    sleep(3);
    endwin();
}

/*
 * Fonctions statiques
 */

static void create_color()
{
    start_color();   /* Start color */
    init_pair(1,  COLOR_WHITE, COLOR_BLACK);
    init_pair(2,  COLOR_RED,   COLOR_BLACK);
    init_pair(3,  COLOR_GREEN, COLOR_BLACK);
}