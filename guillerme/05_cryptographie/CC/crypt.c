#include "crypt.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  XOR
 *  Description:  Réalise l'opération suivante : V = A XOR B. On pourrait utiliser
 *  addKey pour réaliser l'opération mais à des fins de facilité de compréhension
 *  on utilise une autre fonction.
 * =====================================================================================
 */
void XOR(mot_t *V, mot_t *A, mot_t *B) {
    unsigned int i = 0;
    for(; i<3; V->u[i] = A->u[i] ^ B->u[i], i++);
}		/* -----  end of function XOR  ----- */


/*-----------------------------------------------------------------------------
 *  Fonctions relatives au cryptosystème
 *-----------------------------------------------------------------------------*/
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SBoite
 *  Description:  Implément une S-Boite
 * =====================================================================================
 */
void SBoite(mot_t *V, mot_t *U) {
    int i;
    for(i=0;i<3;i++)
        V->u[i]=S[U->u[i]];
}		/* -----  end of function SBoite  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SBoiteInverse
 *  Description:  Implémente une S-Boite Inverse
 * =====================================================================================
 */
void SBoiteInverse(mot_t *V, mot_t *U) {
    int i;
    for(i=0; i<3; V->u[i] = Sinverse[U->u[i]], i++);
}		/* -----  end of function SBoiteInverse  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  addKey
 *  Description:  Ajoute la clé K au mot W et place le résultat dans U
 * =====================================================================================
 */
void addKey(mot_t *U, mot_t *W, mot_t* K) {
    int i;
    for(i=0;i<3;i++)
        U->u[i]=W->u[i]^K->u[i];
}		/* -----  end of function addKey  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  P
 *  Description:  Effectue une permutation des bits
 * =====================================================================================
 */
void P(mot_t *W, mot_t *V) {
    W->u[0]= (V->u[1]& 8 ) ^ ((V->u[2]& 8) >> 1) ^((V->u[0] & 4) >>1) ^( (V->u[1] & 2) >> 1);
    W->u[1]=((V->u[2]& 4 ) << 1) ^ ((V->u[0]& 8) >> 1) ^((V->u[0] & 1) << 1) ^ ( (V->u[2] & 2) >> 1);
    W->u[2]=((V->u[1]& 4 ) << 1) ^ ((V->u[0]& 2)  << 1) ^ ((V->u[2] & 1) << 1)  ^ (V->u[1] & 1) ;
}		/* -----  end of function P  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  PInverse
 *  Description:  Effectue une permutation des bits telle que Pinverse(P(M)) = M
 * =====================================================================================
 */
void PInverse(mot_t *W, mot_t *V) {
    W->u[0] = ((V->u[1] & 4) << 1) ^ ((V->u[0] & 2) << 1) ^ ((V->u[2] & 4) >> 1) ^ ((V->u[1] & 2) >> 1);
    W->u[1] = (V->u[0] & 8) ^ ((V->u[2] & 8) >> 1) ^ ((V->u[0] & 1) << 1) ^ (V->u[2] & 1);
    W->u[2] = ((V->u[0] & 4) << 1) ^ ((V->u[1] & 8) >> 1) ^ ((V->u[1] & 1) << 1) ^ ((V->u[2] & 2) >> 1);
}		/* -----  end of function PInverse  ----- */


/*-----------------------------------------------------------------------------
 *  Fonctions de chiffrement et de déchiffrement
 *-----------------------------------------------------------------------------*/
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chiffrement
 *  Description:  Effectue le chiffrement du mot M avec la clé K et place le résultat 
 *  dans le mot C
 * =====================================================================================
 */
void chiffrement(mot_t *C, mot_t *M, mot_t *K) {
    unsigned int i;
    mot_t U, V, W;
    addKey(&U, M, K);
    for(i=0;i<NB_ITER;i++) {
        SBoite(&V,&U);
        P(&W,&V);
        addKey(&U,&W,K);
    }
    copieMot(C, &U);
}		/* -----  end of function chiffrement  ----- */


void dechiffrement(mot_t *M, mot_t *C, mot_t *K) {
    unsigned int i;
    mot_t U, V, W;
    copieMot(&U, C);
    for(i=0; i<NB_ITER; addKey(&W, &U, K), PInverse(&V, &W), SBoiteInverse(&U, &V), i++); 
    addKey(M, &U, K);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rondeInverse
 *  Description:  Effectue une ronde inverse
 * =====================================================================================
 */
void rondeInverse(mot_t *M, mot_t *C, mot_t *K) {
    unsigned int i;
    mot_t V, W;
    addKey(&W, C, K);
    PInverse(&V, &W);
    SBoiteInverse(M, &V); 
}		/* -----  end of function rondeInverse  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  trouveDiff
 *  Description:  trouveDiff permet de trouver l'ensemble des couples (u, u') vérifiant
 *  u xor u' = delta et y xor y' ) deltastar
 * =====================================================================================
 */
couple_t *trouveDiff(unsigned int delta, unsigned int deltastar, unsigned int *trouve) {
    unsigned int nb_entre = 0;
    couple_t *resultat = NULL;

    unsigned int i, j;
    for(i=0; i<MAX; i++) 
        for(j=0; j<MAX; j++) 
            if((i ^ j) == delta) 
                if((S[i] ^ S[j]) == deltastar) {
                    // Si on trouve un nouveau couple vérifiant les propriétés on réalloue la
                    // mémoire (TODO: Sécuriser l'allocation de mémoire)
                    couple_t *tmp = (couple_t *) realloc(resultat, (nb_entre + 1) * sizeof(couple_t));
                    resultat = tmp;
                    resultat[nb_entre].u = i;
                    resultat[nb_entre].uprime = j;
                    // On augmente le nombre de solution trovuées
                    nb_entre ++;
                }
    *trouve = nb_entre;
    return resultat;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  tableDistributionDiff
 *  Description:  cette fonction construit la table de distribution des différences. 
 *  Celle ci est de la forme :
 *     |    b    |
 *  a  | N(a->b) |
 * =====================================================================================
 */
unsigned int *tableDistributionDiff() {
    couple_t *tmp;
    unsigned int *table = (unsigned int *) calloc(MAX * MAX, sizeof(unsigned int));
    unsigned int i, j;
    for(i=0; i<MAX; i++)
        for(j=0; j<MAX; j++) {
            tmp = trouveDiff(i, j, table + i*MAX + j);
            free(tmp);
        }
    return table;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  afficheTable
 *  Description:  Permet d'afficher correctement une table de distribution des 
 *  différences
 * =====================================================================================
 */
void afficheTable(unsigned int *table) {
    unsigned int i, j;
    printf("   ");
    for(i=0; i<MAX; i++)
        printf("%2d ", i);
    printf("\n");
    for(i=0; i<MAX; i++){
        printf("%2d ", i);
        for(j=0; j<MAX; j++)
            printf("%2d ", table[i*MAX + j]);
        printf("\n");
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  trouveMaxTableDiff
 *  Description:  Permet de trouver les couples (delta, deltastar) pour lesquelles la 
 *  probabilté de traverser la boite S est la plus grande.
 * =====================================================================================
 */
couple_t *trouveMaxTableDiff(unsigned int *table, unsigned int *nb, unsigned int *m) {
    unsigned int i, trouve = 0, max = 0, sol[MAX * MAX];

    // On ne prend pas la première ligne de la table (car toujours égale à 16 0 ... 0
    for (i=MAX; i<MAX*MAX; i++) {
        if ( table[i] == max ) {
            sol[trouve] = i;
            trouve ++;
        }
        else {
            if ( table[i] > max ) {
                sol[0] = i;
                trouve = 1;
                max = table[i];
            }
        }
    }

    // TODO: Sécuriser le malloc
    couple_t *resultat = (couple_t *) malloc(sizeof(couple_t) * trouve);
    for ( i=0; i<trouve; i++ ) {
       (resultat[i]).u = sol[i] / MAX;
       (resultat[i]).uprime = sol[i] - ((resultat[i]).u * MAX);
    }
    *nb = trouve;
    *m = max;
	return resultat;
}		/* -----  end of function trouveMaxTableDiff  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  aleaMot
 *  Description:  génère un mot aléatoire
 * =====================================================================================
 */
void aleaMot(mot_t *A) {
    unsigned int i = 0;
    for(; i<3; i++)
        A->u[i] = rand()%MAX;
}		/* -----  end of function aleaMot  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  copieMot
 *  Description:  Copie B dans A
 * =====================================================================================
 */
void copieMot(mot_t *A, mot_t *B) {
	unsigned int i=0;
    for(; i<3; A->u[i] = B->u[i], i++);
}		/* -----  end of function copieMot  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  trouveCoupleMot
 *  Description:  Fonction permettant de trouver l'ensemble des couples de mots 
 *  vérifiant une différence donnée
 * =====================================================================================
 */
mcouple_t *trouveCoupleMot(mot_t *diff, int nbmess) {
    unsigned int i=0, j;
    mcouple_t *resultat = (mcouple_t *) calloc (nbmess, sizeof(mcouple_t));
    mot_t tmp, diff_null;
    for(j=0; j<3; j++)
        diff_null.u[j] = 0;
    int i_corrige, egal, correctif = 0;
    // L'idéal serait de regarder quels entiers sont touchés par la différence et de ne faire varier
    // que ces derniers. Ici la différence en entrée n'est que sur le premier entier, on ne fait
    // donc varier que celui là
    for (; i<nbmess; i++) {
        // L'inconvénient de l'aléatoire est qu'il est difficile de vérifier si deux mots sont
        // identiques
        //aleaMot(&(resultat[i].x));
        egal = 0;
        i_corrige = i + correctif; 
        tmp.u[0] = (i_corrige % (MAX * MAX)) % MAX;
        tmp.u[1] = (i_corrige % (MAX * MAX))/ MAX;
        tmp.u[2] = i_corrige / (MAX * MAX);
        j=0;
        while(j < i) {
            if(estEgalViaXor(tmp, resultat[j].x, diff_null) || estEgalViaXor(tmp, resultat[j].xprime, diff_null)) {
                egal ++;
                break;
            }
            j++;
        }
        if (!egal) {
            resultat[i].x = tmp;
            XOR(&(resultat[i].xprime), &(resultat[i].x), diff);
        } else {
            correctif ++;
            i --;
        }
    }
    return resultat;
}		/* -----  end of function trouveCoupleMot  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printCoupleMot
 *  Description:  affiche un couple de mot
 * =====================================================================================
 */
void printCoupleMot(mcouple_t m) {
    printf("([%2d, %2d, %2d], [%2d, %2d, %2d])\n", m.x.u[0], m.x.u[1], m.x.u[2], m.xprime.u[0], m.xprime.u[1], m.xprime.u[2]);
}		/* -----  end of function printCoupleMot  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  creeMotInt
 *  Description:  Crée un mot à partir d'un entier (plus d'actualité)
 * =====================================================================================
 */
mot_t creeMotInt(unsigned int a) {
    mot_t b;
    b.u[0] = a / (MAX * MAX);
    b.u[1] = (a - b.u[0] * MAX * MAX)/MAX;
    b.u[2] = a - b.u[0] * MAX * MAX - b.u[1] * MAX;

	return b;
}		/* -----  end of function creeMotInt  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  creeMot
 *  Description:  
 * =====================================================================================
 */
mot_t creeMot(unsigned int a, unsigned int b, unsigned int c) {
    mot_t m;
    m.u[0] = a;
    m.u[1] = b;
    m.u[2] = c;
    return m;
}		/* -----  end of function creeMot  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printMot
 *  Description:  
 * =====================================================================================
 */
void printMot(mot_t a) {
    printf("[ %2d, %2d, %2d]\n", a.u[0], a.u[1], a.u[2]);
}		/* -----  end of function printMot  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  estEgalViaXor
 *  Description:  teste si a = b xor diff
 * =====================================================================================
 */
int estEgalViaXor(mot_t a, mot_t b, mot_t diff) {
    return !(a.u[0] ^ b.u[0] ^ diff.u[0] | a.u[1] ^ b.u[1] ^ diff.u[1] | a.u[2] ^ b.u[2] ^ diff.u[2]);
}		/* -----  end of function estEgalViaXor  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  creeMotUtil
 *  Description:  Demande une clé à l'utilisateur
 * =====================================================================================
 */
mot_t creeMotUtil() {
    int a[3], i;
    for (i=0; i< 3; i++) {
        do {
            printf("Veuillez entrer la valeur du %d entier de la cle (compris entre 0 et %d) : ", i, MAX - 1);
            scanf("%d", a+i);
        } while ( (int)a[i] < 0 || (int)a[i] >= MAX );				/* -----  end do-while  ----- */
    }
	return creeMot(a[0], a[1], a[2]);
}		/* -----  end of function creeMotUtil  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  afficheBit
 *  Description:  Affiche sous forme de chaine de bits
 * =====================================================================================
 */
void afficheBit(mot_t m) {
    int i;
    for (i=0; i<3; i++) 
        printf("[%d,%d,%d,%d] ", (m.u[i] & 8)/8, (m.u[i] & 4)/4, (m.u[i] & 2)/2, m.u[i] & 1);
    printf("\n");
}		/* -----  end of function afficheBit  ----- */

int main(int argc, char **argv) {
    srand(time(NULL));
    unsigned int i, j, trouve;
    printf("\nRecherche du nombre de couples d'entier de différence en entrée [1,0,0,1] et en sortie [0,0,0,1]...\n");
    couple_t *diff = trouveDiff(9, 1, &trouve);
    if(trouve) {
        printf("\n======> SOLUTION <======\n\n");
        printf("Nombre = %d.\n", trouve);
        for(i=0; i<trouve; i++)
            PRINTCOUPLE(diff[i]);
        free(diff);
    }
    else {
        printf("\n======> SOLUTION <======\n\n");
        printf("Nombre = %d.\n", trouve);
    }

    printf("\nRecherche du nombre de couples d'entier de différence en entrée [0,0,1,0] et en sortie [1,1,1,1]...\n");
    diff = trouveDiff(2, 15, &trouve);
    if(trouve) {
        printf("\n======> SOLUTION <======\n\n");
        printf("Nombre = %d.\n", trouve);
        for(i=0; i<trouve; i++)
            PRINTCOUPLE(diff[i]);
        free(diff);
    }
    else {
        printf("\n======> SOLUTION <======\n\n");
        printf("Nombre = %d.\n", trouve);
    }

    printf("\nCalcul de la table de propagation des différences....\n");

    printf("\n======> TABLE DE PROPAGATION DES DIFFERENCES <======\n\n");
    unsigned int *table = tableDistributionDiff();
    afficheTable(table);

    printf("\nRecherche des couples dont la probabilité de propagation des différences est maximale...\n");
    unsigned int max;
    diff = trouveMaxTableDiff(table, &trouve, &max);
    printf("\n======> SOLUTION <======\n\n");
    printf("Nombre de couple = %d, Valeur = %d/%d\n", trouve, max, MAX);
    for(i=0; i<trouve; i++)
        PRINTCOUPLE(diff[i]);
    free(diff);
    free(table);

    printf("\n");
    mot_t K = creeMotUtil();

    printf("\nAttaque des bits actifs...\n");

    mot_t diff_init = creeMot(9, 0, 0);
    unsigned int nb_mot = 1000;

    mcouple_t *resultat = trouveCoupleMot(&diff_init, nb_mot);
    mcouple_t *chiffre = (mcouple_t *) malloc(sizeof(mcouple_t) * nb_mot);
    for(i=0; i<nb_mot; i++) {
        chiffrement(&(chiffre[i].x), &(resultat[i].x), &K);
        chiffrement(&(chiffre[i].xprime), &(resultat[i].xprime), &K);
    }

    mot_t propa_diff = creeMot(0, 2, 2);
    unsigned int probakey[MAX*MAX*MAX] = {0};

    mot_t diff_nulle = creeMotInt(0);
    // Recherche des bits actifs.
    int i11, i12, i14, i21, i24, i31, i33, i34;
    for ( i=0; i<nb_mot; i++ ) {
        for (i11=0; i11 <=1; i11++)
            for (i12=0; i12 <=1; i12++)
                for (i14=0; i14 <=1; i14++)
                    for (i21=0; i21 <=1; i21++)
                        for (i24=0; i24 <=1; i24++)
                            for (i31=0; i31 <=1; i31++)
                                for (i33=0; i33 <=1; i33++)
                                    for (i34=0; i34 <=1; i34++) {
                                        mot_t a, b, k;
                                        k.u[0] = i11*8 + i12*4 + i14;
                                        k.u[1] = i21*8 + i24;
                                        k.u[2] = i31*8 + i33*2 + i34;
                                        rondeInverse(&a, &(chiffre[i].x), &k);
                                        rondeInverse(&b, &(chiffre[i].xprime), &k);
                                        // Teste la différence sur les 8 derniers bits de sortie de la 3e ronde
                                        //if (a.u[1]^propa_diff.u[1] == b.u[1] && a.u[2]^propa_diff.u[2] == b.u[2])
                                        if (estEgalViaXor(a, b, propa_diff))
                                            probakey[k.u[0] * MAX * MAX + k.u[1] * MAX + k.u[2]]++;
        }
    }

    unsigned int cpt=0, maxj=0;
    for ( i=0; i<MAX*MAX*MAX; i++ ) {
        if ( probakey[i] > maxj ) {
            maxj = probakey[i];
            cpt = i;
        }
        //printf ("%d ", probakey[i]);
    }
    printf("\n");

    mot_t sol = creeMotInt(cpt);
    printf("\nClé rentrée par l'utilisateur : ");
    afficheBit(K);
    printf("Bits trouvés par attaque différentielle : ");
    afficheBit(sol);

    printf("\nAttaque exhaustive sur les bits restants...\n");
    mot_t m = resultat[10].x;
    mot_t c;
    chiffrement (&c, &m, &K);
    // Brute Force sur les bits restants
    int i13, i22, i23, i32;
    for (i13 = 0; i13 <=1; i13++)
        for (i22 = 0; i22 <=1; i22++)
            for (i23 = 0; i23 <=1; i23++)
                for (i32 = 0; i32 <=1; i32++) {
                    mot_t k, tmp;
                    k.u[0] = sol.u[0] + i13*2;
                    k.u[1] = sol.u[1] + i22*4 + i23 * 2;
                    k.u[2] = sol.u[2] + i32*4;
                    dechiffrement(&tmp, &c, &k);
                    if (estEgalViaXor(m, tmp, diff_nulle)) {
                        sol = k;
                        break;
                    }
                }

    printf("\nClé rentrée par l'utilisateur : ");
    afficheBit(K);
    printf("Bits trouvés par attaque différentielle : ");
    afficheBit(sol);

    free(resultat);
    free(chiffre);
}

