/** 
 * \file jeu.h
 * \brief Header pour le fichier jeu.c.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#ifndef JEU_H_FILE
#define JEU_H_FILE

#include <stdio.h>
#include <string.h>
#include "Sconsts.h"

/** Taille des cotés du damier par défaut. */
#define T_TAILLE 7

/** Taille maximum d'un côté du damier. */
#define MAX_TAILLE 26

/** Renvoie le maximum de deux valeurs/ */
#define MAX(x, y) (((x) > (y) ? (x) : (y)))

/** Renvoie le minimum de deux valeurs. */
#define MIN(x, y) (((x) < (y) ? (x) : (y)))


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
extern int Sjeu_Initialiser(options*, damier*, int def);
extern int Sjeu_Afficher(damier, options);
extern int Sjeu_Jouer(damier*, options*, int*);

#endif
