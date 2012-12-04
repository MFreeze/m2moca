#ifndef OUTIL_VIGENERE_H
#define OUTIL_VIGENERE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 5000


int comp_int(int *,int *);

/* fonction d'affichage */
void affiche_numerique(int *,int );
void affiche_alphabetique(int *,int );
void affiche_clair_vigenere(int *, int ,  int , int *, int );
void affiche_clair_chiffre(int *, int , int, char *);
void affiche_clair(int *,int ,char *);


/* fonctions de chiffrement */
void chiffrer_substitution( int *,  int *, int *,int );
void chiffrer_vigenere( int *, int ,  int *, int *,int );

/* Fonction de calcul de frequence */
int compter_lettre(int *,int ,int );
int compter_bigramme(int *,int ,int ,int );
void comptage_nb_apparition(int *, int );
void comptage_frequence_apparition(int *, int );
double indice_coincidence( int *, int );


void extraction_sous_texte( int *, int , int *, int *, int , int );
int lecture_texte_clair( int * , int *, char *);

#endif
