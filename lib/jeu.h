/** 
 * \file jeu.h
 * \brief Header pour le fichier jeu.c.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#ifndef JEU_H_FILE
#define JEU_H_FILE

#include <stdio.h>
#include "consts.h"

/** Taille des cotés du damier par défaut. */
#define T_TAILLE 7

/** Taille maximum d'un côté du damier. */
#define MAX_TAILLE 26

/** Renvoie le maximum de deux valeurs/ */
#define MAX(x, y) (((x) > (y) ? (x) : (y)))

/** Renvoie le minimum de deux valeurs. */
#define MIN(x, y) (((x) < (y) ? (x) : (y)))


/* Déclaration des fonctions système utilisées */
// void* memmove(void*, const void*, size_t);
int printf(const char*, ...);
void* malloc(size_t);
char* strcpy(char*, const char*);
size_t strlen(const char*);


/** Représente les trois états possibles d'une case. */
typedef enum cases 
{
	/** La case spécifiée n'existe pas. */
    VIDE, 

    /** La case spécifiée existe et ne contient pas de pion. */
    LIBRE, 

    /** La case spécifiée existe et contient un pion. */
    PION
} cases;

/** Représente le jeu dans son état actuel. */
typedef struct damier 
{
	/** Une matrice de cases représentant le damier. */
    cases** table;
    
   	/** Le nombre de pions encore présents sur le damier. */
    int nb_pion;
} damier;

/** Représente les coordonnées d'une case */
typedef struct coordonnees 
{
	/** Abscisse */
    int a;

    /** Ordonnée */
    int o;
} coordonnees;

/* Fonctions du module */
int initJeu(options*, damier*, int def);
int afficher(damier, options);
int jouer(damier*, options*, int*);

#endif
