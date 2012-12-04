/*******************************************************************************/
/****                                                                        ***/
/****                 CHIFFREMENT   VIGENERE.C                                    ***/
/****                                                                        ***/
/*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define N 5000

void chiffrer_substitution( int *,  int *, int *,int);
void lecture_texte_clair( int * , int *, char *);
void lecture_clef( int * , char *);
void ecrire_chiffre( int *, int , char *);


/******************* Ecriture Texte Chiffre ***************************/

void ecrire_chiffre( int *texte_chiffre, int longueur_texte, char *nom_fichier)
{

  char lettre;
  FILE *hand_file;
  int i;
 
  /* Ouverture du fichier , gerer l'impossibilité d'ouverture du fichier*/

  if( (hand_file = fopen(nom_fichier,"w")) == NULL)
  {
    fprintf(stderr,"\n Erreur: Impossible d'écrire dans le fichier %s \n",nom_fichier);
    exit(EXIT_FAILURE);
  } 

  /** Ecriture du texte chiffre dans le fichier donner en parametre **/
  for(i=0;i<longueur_texte;i++)
    {
      fputc('a'+(char) texte_chiffre[i],hand_file);
    }

  /* Fermeture du fichier */
  fclose(hand_file); 
}



/******************* Lecture du texte clair ***************************/


void lecture_texte_clair( int *texte_clair, int *longueur_texte, char *nom_fichier)
{

  char lettre;
  FILE *hand_file;

  (*longueur_texte)=0;

  /* Ouverture du fichier , gerer l'impossibilité d'ouverture du fichier*/

  if( (hand_file = fopen(nom_fichier,"r")) == NULL)
  {
    fprintf(stderr,"\n Erreur: Impossible d'écrire dans le fichier %s \n",nom_fichier);
    exit(EXIT_FAILURE);
  } 

  /** Lecture du fichier contenant le texte a decrypter **/
  lettre=fgetc(hand_file);
  while( (lettre != EOF) && ((*longueur_texte) < N))
    {
      if( ( lettre <= 'z') && ( lettre >= 'a')) 
	{
	  (*longueur_texte)++;
	  texte_clair[(*longueur_texte)-1]=((int) lettre ) - ((int) 'a');
	}
      else if( ( lettre <= 'Z') && ( lettre >= 'A')) 
	{
	  (*longueur_texte)++;
	  texte_clair[(*longueur_texte)-1]=((int) lettre ) - ((int) 'A');
	}
       lettre=fgetc(hand_file);
    }

  /* Fermeture du fichier */
  fclose(hand_file);
 
}


/******************* Lecture de la clef  ***************************/


void lecture_clef( int clef[26], char *fichier_clef)
{

  char lettre;
  FILE *hand_file;
  int i;

  /* Ouverture du fichier , gerer l'impossibilité d'ouverture du fichier*/

  if( (hand_file = fopen(fichier_clef,"r")) == NULL)
  {
    fprintf(stderr,"\n Erreur: Impossible d'écrire dans le fichier %s \n",fichier_clef);
    exit(EXIT_FAILURE);
  } 

  /** Lecture du fichier contenant la clef en alphabetique **/
  for(i=0;i<26;i++)
    {
        lettre=fgetc(hand_file);
	clef[i] = (int) (lettre - 'a');
    }
  /* Fermeture du fichier */
  fclose(hand_file);
}

/******************** Fonction de Chiffrement **********************/

void chiffrer_substitution( int *clef,  int *texte_clair, int *texte_chiffre,int longueur_texte)
{
  int i;
  for(i=0;i<longueur_texte;i++)
    {
      texte_chiffre[i] = clef[texte_clair[i]];
    }
  return;

}

/*********************** Fonction Main *************************/
 
int main(int argc, char *argv[])
{
  int i,texte_clair[N],indice; 
  int texte_chiffre[N]; 
  int longueur_texte;
  int clef[26];
  int longueur_clef;

 
 if( argc < 4)
    {
      printf("Attention: en parametre il faut :\n Un fichier contenant le texte a chiffrer \n Un fichier contenant la clef \n Un fichier qui contiendra le texte chiffre \n");
    }
   

  /** Lecture du texte clair **/
  lecture_texte_clair( texte_clair , &longueur_texte, argv[1]);
  /** Lecture de la clef **/
  lecture_clef( clef , argv[2]);
  /** Chiffrement **/
  chiffrer_substitution( clef,  texte_clair, texte_chiffre,longueur_texte);
  /** Ecriture texte chiffre **/
  ecrire_chiffre(texte_chiffre, longueur_texte, argv[3]);

}
