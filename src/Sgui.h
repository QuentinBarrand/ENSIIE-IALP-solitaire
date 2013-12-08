/**
 * \file Sgui.h
 * \brief Header pour le fichier Sgui.c.
 * 
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#ifndef SGUI_H_FILE
#define SGUI_H_FILE

#include <curses.h>

/** La taille maximale de la saisie de l'utilisateur */
#define MAX_INPUT 100

/* Fonctions externes du module. */
extern WINDOW* Sgui_Initialiser();
extern void    Sgui_ReadCoup(char*);
extern void    Sgui_RuntimeError(WINDOW*, char*);
extern void    Sgui_RuntimeSuccess(WINDOW*, char*);
extern void    Sgui_Splash(WINDOW*);
extern void    Sgui_StartupError(WINDOW*, char*);
extern void    Sgui_Terminer(WINDOW*);

#endif
