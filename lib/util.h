/**
 * \file util.h
 * \brief Header pour le fichier util.c.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#ifndef UTIL_H_FILE
#define UTIL_H_FILE


/* Fonctions système */
int printf(const char*, ...);


/** Options du programme. */
typedef struct options 
{
	/** Déplacements horizontaux et verticaux */
    int n;

    /** Déplacements en diagonale */
    int d;

    /** Chemin vers le fichier de configuration du damier */
    char* confPath;

    /** Nombre de colonnes dans le fichier de configuration */
    int confWidth;

    /** Nombre de lignes dans le fichier de configuration du damier. */
    int confLength;
} options;

/* Fonctions du module */
void help(char*);
int getOptions(options*, int, char**);
int readConfFile(options*);

#endif
