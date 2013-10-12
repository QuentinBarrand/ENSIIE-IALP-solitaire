/** 
 * \file util.c 
 * \brief Fonctions utilitaires de l'application.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */
#include "util.h"


/** Affiche l'usage du programme.
 *
 * \param executable le nom du programme (correspond à <tt>argv[0]</tt>).
 */
void help(char* executable)
{
    printf("Usage : %s [options]"
        "-h    Affiche l'aide et quitte le programme."
        "-n    Autorise les coups horizontaux et verticaux (par défaut)."
        "-d    Autorise les coups en diagonale.\n", executable);
}


/** Récupère les options de la ligne de commande
 *
 * \param config la configuration actuelle de l'application.
 * \param argc le nombre d'arguments depuis la ligne de commande.
 * \param argv la matrice représentant les arguments de la ligne de commande.
 *
 * \return un code de statut: 
 *    - 0 : tous les paramètres ont été lus correctement
 *    - 1 : a affiché l'aide
 *    - 2 : une erreur de lecture des paramètres est survenue
 */
int getOptions(options* config, int argc, char** argv)
{
    const int FUNC_SUCCESS         = 0;
    const int FUNC_PRINTED_HELP    = 1;
    const int FUNC_INCORRECT_PARAM = 2;

    int i;

    config->n = 0;
    config->d = 0;

    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    help(argv[0]);
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
            /* Inutile d'aller plus loin, au moins un argument est incorrect */
            return 2;
    }

    /* Par défaut on active l'option -n */
    if(config->n == 0 && config->d == 0)
        config->n =1;

    return FUNC_SUCCESS;
}


/** Lit la configuration du damier dans le fichier de configuration.
 *
 * \param config les options courantes de l'application.
 */
int readConfFile(options* config)
{

}
