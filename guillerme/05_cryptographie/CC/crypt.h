#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Couple {
    unsigned int u;
    unsigned int uprime;
} couple_t;

// La structure est l'équivalent du unsigned int[3] précédemment utilisé, il représente U
typedef struct Mot {
    unsigned int u[3];
} mot_t;				/* ----------  end of struct Mot  ---------- */

typedef struct Couple_Mot {
    mot_t x;
    mot_t xprime;
} mcouple_t;

// la macro SUM fait un XOR des 4 bits de poids faible
#define SUM(s) ((s &  1) ^ ((s & 2) >> 1)  ^ ((s & 4) >> 2)  ^ ((s & 8) >> 3))
// la macro suivante permet d'afficher une structure de couple
#define PRINTCOUPLE(c) printf("( %2d, %2d)\n", c.u, c.uprime)
#define MAX 16
#define NB_ITER 4
#define K1 13
#define K2 9
#define K3 9
// La cosntante suivante est utilisée pour calculer le nombre de couple de mots à sélectioner pour
// l'attaque
#define CM 4

// Les tables stockant la substitution S et l'inverse de S
//                          0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
unsigned int S[MAX]       ={14, 2, 1, 3,13, 9, 0, 6,15, 4, 5,10, 8,12, 7,11};
/* Pour obtenir S' on utilise : S'[S[i]] = i
 *                          0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 */
unsigned int Sinverse[MAX]={ 6, 2, 1, 3, 9,10, 7,14,12, 5,11,15,13, 4, 0, 8};

void XOR(mot_t *V, mot_t *A, mot_t *B);

void SBoite(mot_t *V, mot_t *U);
void SBoiteInverse(mot_t *V, mot_t *U);

void addKey(mot_t *U, mot_t *W,mot_t *K);

void P(mot_t *W, mot_t *V);
void PInverse(mot_t *W, mot_t *V);

void chiffrement(mot_t *C, mot_t *M, mot_t *K);
void dechiffrement(mot_t *M, mot_t *C, mot_t *K);

void rondeInverse(mot_t *M, mot_t *C, mot_t *K);

couple_t *trouveDiff(unsigned int delta, unsigned int deltastar, unsigned int *trouve);
couple_t *trouveMaxTableDiff(unsigned int *table, unsigned int *nb, unsigned int *m);

unsigned int *tableDistributionDiff();
void afficheTable(unsigned int *table);

void aleaMot(mot_t *A);
void copieMot(mot_t *A, mot_t *B);

mcouple_t *trouveCoupleMot(mot_t *diff, int nbmess);
void printCoupleMot(mcouple_t m);

mot_t creeMot(unsigned int a, unsigned int b, unsigned int c);
mot_t creeMotInt(unsigned int a);
mot_t creeMotUtili();
void printMot(mot_t a);

void afficheBit(mot_t m);

int estEgalViaXor(mot_t a, mot_t b, mot_t diff);
