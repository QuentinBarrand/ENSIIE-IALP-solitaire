/** 
 * \file Sjeu.h
 * \brief Header pour le fichier Sjeu.c.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#ifndef SJEU_H_FILE
#define SJEU_H_FILE

/** Options du programme. */
typedef struct options 
{
    /** Déplacements horizontaux et verticaux. */
    int n;

    /** Déplacements en diagonale/ */
    int d;

    /** Présence d'un fichier de configuration. */
    int confExists;

    /** Chemin vers le fichier de configuration du damier */
    char* confPath;
} options;

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

    /** Nombre de colonnes dans le fichier de configuration */
    int width;

    /** Nombre de lignes dans le fichier de configuration du damier. */
    int length;
} damier;

/** Représente les coordonnées d'une case */
typedef struct coordonnees 
{
	/** Abscisse */
    int a;

    /** Ordonnée */
    int o;
} coordonnees;

/** Représente un coup. */
typedef struct coup
{
    /** La case de départ du coup. */
    coordonnees depart;

    /** La case d'arrivée du coup. */
    coordonnees arrivee;
} coup;


/* Fonctions du module */
extern void Sjeu_Aide(char*);
extern int  Sjeu_LoadOptions(options*, int, char**);
extern int  Sjeu_New(options*);

#endif
