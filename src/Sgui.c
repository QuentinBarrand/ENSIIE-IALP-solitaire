/**
 * \file Sgui.c
 * \brief Fonctions de gestion de l'interface utilisateur.
 * 
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#include <curses.h>
#include <unistd.h>

#include "Sgui.h"

static void create_color()
{
    start_color();   /* Start color */
    init_pair(1,  COLOR_WHITE, COLOR_BLACK);
    init_pair(2,  COLOR_RED,   COLOR_BLACK);
    init_pair(5,  COLOR_GREEN, COLOR_BLACK);
    init_pair(6,  COLOR_RED,   COLOR_BLACK);
    init_pair(10, COLOR_RED,   COLOR_BLACK);
    init_pair(11, COLOR_RED,   COLOR_GREEN);
    init_pair(20, COLOR_WHITE, COLOR_BLUE);
    init_pair(21, COLOR_WHITE, COLOR_BLUE);
}


void Sgui_ReadCoup(char* userinput)
{
    echo();
    getstr(userinput);
    noecho();
}


extern void Sgui_Splash(WINDOW* app_window)
{
    #ifdef VERSION
    mvwprintw(app_window, 0, 0, "Version : " VERSION);
    #endif

    #ifdef BUILD
    mvwprintw(app_window, 1, 0, "Build : " BUILD);
    #endif

    int x_offset = (LINES / 2) - 4, y_offset = (COLS / 2) - 40;

    if(COLS >= 80)
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

    /* A ajouter :
     * Par AUTEUR
     * Appuyez sur une touche pour commencer...
     */ 

    refresh();    

    getch();
    clear();


}


extern WINDOW* Sgui_Initialiser()
{
    WINDOW* app_window = initscr();
    // int dx = COLS;
    // int dy = LINES;

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    create_color();
    refresh();

    return app_window;
}


extern void Sgui_StartupError(WINDOW* app_window, char* message)
{
    create_color();
    attron(COLOR_PAIR(10));
    mvwprintw(app_window, 20, 20, message);
}


extern void Sgui_Terminer()
{
    endwin();
}
