/*
 * =====================================================================================
 *
 *       Filename:  menu.c
 *
 *    Description:  Implementation file of MMI routines
 *
 *        Version:  1.0
 *        Created:  04/01/2013 23:59:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  DUVILLIE Guillerme (mfreeze), guillerme.duvillie@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "menu.h"

// Hierarchy :
// \__ RSA
// | \__ Key Generation
// | \__ Encrypt
// | \__ Decrypt
// | \__ Sign
// \__ El Gamal
// | \__ Key Generation
// | \__ Encrypt
// | \__ Decrypt
// | \__ Sign
// \__ Diffie Hellman
//

// It could be a good idea to use function pointer 

#define NB_SUBMENU 2 // RSA and El Gamal
#define NB_MENU 1 // Main menu

char ***hierarchy = NULL;
// Always NULL, at the end AND last item must be Back
// This must be in main file, (HORRIBLE!!!)
char *princ_menu[] = {"RSA", "ElGamal", "Diffie Hellman", "Help", "Quit", NULL};
char *enc_opt[] = {"Key Generation", "Encrypt", "Decrypt", "Sign", "Back", NULL};

void freeBuf() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

// {{{ Man Machine Interface init and clean
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initMMI
 *  Description:  
 * =====================================================================================
 */
void initMMI(int *menu, int *submenu) {
    // Memory Allocation
    if (!hierarchy) {
        hierarchy = (char ***) calloc (NB_SUBMENU + NB_MENU, sizeof(char **));
        if (!hierarchy) {
            fprintf(stderr, "Erreur lors de l'initialisation du menu.\n");
            exit(EXIT_FAILURE);
        }
    }

    *menu = 0;
    *submenu = 0;

    // Hierarchy[0] is always the principal menu
    hierarchy[0] = princ_menu;
    // Then Hierarchy contains the items proposing a submenu
    hierarchy[1] = enc_opt;
    hierarchy[2] = enc_opt;
}		/* -----  end of function initMMI  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  clearMMI
 *  Description:  Memory cleaning
 * =====================================================================================
 */
void clearMMI() {
    free(hierarchy);
}
// }}}

// {{{ Menu routines
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printAppropMenu
 *  Description:  Print Appropriate menu according to given parameters
 * =====================================================================================
 */
void printAppropMenu(int menu) {
    int i = 0;
    if (menu >= 0 && menu < NB_MENU + NB_SUBMENU) {
        printf("\n");
        while (hierarchy[menu][i]) {
            printf("%d - %s\n", i+1, hierarchy[menu][i]);
            i++;
        }
        printf("\n");
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  askUser
 *  Description:  ask user for action it returns -1 for an error, 0 for a move in the 
 *                hierarchy, 1 on other case
 * =====================================================================================
 */
int askUser(int *menu, int *submenu) {
    int choice = -1;
    char tmp_choice = 0;
    int nb_choice = 0;
    
    if (*menu >= NB_SUBMENU + NB_MENU || *menu < 0)
        return -1;

    while (hierarchy[*menu][nb_choice++]) {}

    switch (*menu) {
        case 0:
            while (choice <= 0 || choice >= nb_choice) {
                printAppropMenu(*menu);
                printf("\nYour choice : ");
                tmp_choice = getchar();
                choice = atoi(&tmp_choice);
                freeBuf();
            }
            if (choice <= NB_SUBMENU) {
                *menu = choice;
                return 0;
            }
            *submenu = choice;
            return 1;
            break;
        case 1:
        case 2:
            while (choice <= 0 || choice >= nb_choice) {
                printAppropMenu(*menu);
                printf("\nYour choice : ");
                tmp_choice = getchar();
                choice = atoi(&tmp_choice);
                freeBuf();
            }
            if (choice == nb_choice - 1) {
                *menu = 0;
                return 0;
            }
            *submenu = choice;
            return 1;
        break;
    }
}
// }}}

// {{{ Parsers

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getFile
 *  Description:  
 * =====================================================================================
 */
FILE *getFile(const char *mod) {
    char c, *name, *tmp;
    int nsize = 32, csize = 1;
    FILE *result;

    name = (char *) malloc (sizeof(char) * nsize);

    do {
        c = getchar();
        if (csize >= nsize) {
            nsize = nsize << 1;
            tmp = (char *) realloc (name, sizeof(char) * nsize);
            if (tmp)
                name = tmp;
            else {
                free(name);
                return NULL;
            }
        }
        name[csize - 1] = c;
        csize ++;
    } while(c != EOF && c != '\n');
    name[csize - 2] = '\0';

    // File opening
    errno = 0;
    result = fopen(name, mod);
    if (!result && errno) {
        fprintf(stderr, "Erreur à l'ouverture du fichier %s (mode : %s). %s\n", name, mod, strerror(errno));
        free(name);
        return NULL;
    }
    else if (!result) {
        fprintf (stderr, "Erreur inconnue à l'ouverture du fichier %s (mode : %s).\n", name, mod);
        free(name);
        return NULL;
    }

    free(name);
	return result;
}		/* -----  end of function getFile  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  readFile
 *  Description:  
 * =====================================================================================
 */
char *readFile(FILE *f, int *s) {
    char c, *result, *tmp;
    int nsize = 32, csize = 1;

    result = (char *) malloc (sizeof(char) * nsize);

    do {
        c = fgetc(f);
        if (csize >= nsize) {
            nsize = nsize << 1;
            tmp = (char *) realloc (result, sizeof(char) * nsize);
            if (tmp)
                result = tmp;
            else {
                free(result);
                return NULL;
            }
        }
        result[csize - 1] = c;
        csize ++;
    } while(c != EOF);
    result[csize - 2] = '\0';
    *s = csize - 3;
//    printf("%s\n",result);
//    free(result);
	return result;
}		/* -----  end of function readFile  ----- */
// }}}
