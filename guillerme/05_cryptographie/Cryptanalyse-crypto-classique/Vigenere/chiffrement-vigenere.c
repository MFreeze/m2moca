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


void chiffrer_vigenere( int *, int ,  int *, int *,int );
void lecture_texte_clair( int * , int *, char *);
void lecture_clef( int * , int *, char *);
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


void lecture_clef( int *clef,  int *longueur_clef, char *fichier_clef)
{

  char lettre;
  FILE *hand_file;

  (*longueur_clef)=0;

  /* Ouverture du fichier , gerer l'impossibilité d'ouverture du fichier*/

  if( (hand_file = fopen(fichier_clef,"r")) == NULL)
  {
    fprintf(stderr,"\n Erreur: Impossible d'écrire dans le fichier %s \n",fichier_clef);
    exit(EXIT_FAILURE);
  } 

  /** Lecture du fichier contenant la clef en alphabetique **/
  lettre=fgetc(hand_file);
  while( (lettre != EOF) && ((*longueur_clef) < N))
    {
      if( ( lettre <= 'z') && ( lettre >= 'a')) 
	{
	  (*longueur_clef)++;
	  clef[(*longueur_clef)-1]=((int) lettre ) - ((int) 'a');
	}
      else if( ( lettre <= 'Z') && ( lettre >= 'A')) 
	{
	  (*longueur_clef)++;
	  clef[(*longueur_clef)-1]=((int) lettre ) - ((int) 'A');
	}
       lettre=fgetc(hand_file);
    }
  /* Fermeture du fichier */
  fclose(hand_file);
}

/******************** Fonction de Chiffrement **********************/


void chiffrer_vigenere( int *clef, int longueur_clef,  int *texte_clair, int *texte_chiffre,int longueur_texte)
{

  int nombre_boucle;
  int i,j;
  if( longueur_clef > 0)
    {
      nombre_boucle = longueur_texte/longueur_clef;
      for(i=0;i<nombre_boucle;i++)
	{

	  for(j=0;j<longueur_clef;j++)
	    {
	      texte_chiffre[i*longueur_clef+j]=(texte_clair[i*longueur_clef+j]+clef[j]) % 26; 
	    }
	  
	}
      for(j=0;j<longueur_texte-nombre_boucle*longueur_clef;j++)
	{
	  i=nombre_boucle;
	  texte_chiffre[i*longueur_clef+j]=(texte_clair[i*longueur_clef+j]+clef[j]) % 26;
	  
	}

    }
  else /*gestion de l'erreur de la non positivite de la clef.*/
    {
      printf("\nErreur : Usage  longueur_clef est un entier strictement positif \n"); 
    }
}




 
int main(int argc, char *argv[])
{
  int i,texte_clair[N],indice; 
  int texte_chiffre[N]; 
  int longueur_texte;
  int clef[N];
  int longueur_clef;

  //int clef[20]={ 2, 15, 17, 3, 20, 23, 11, 5, 1, 9, 11, 19, 15, 17, 19, 12, 21, 2, 2, 3},longueur_clef=20
  //int clef[10]={ 2, 15, 3, 20, 11, 5, 1, 11, 19, 17},longueur_clef=10;
  //   int clef[3]={5,21,11},longueur_clef=3;
  //int clef1[3]={21,9,21};

  //int clef[6]={2,17,14,16,22,7},longueur_clef=6;

  //  int *clef_analysee;
//  int longueur_clef=2;
  //int periode;
  // short int test;


  if( argc < 4)
    {
      printf("Attention: en parametre il faut :\n Un fichier contenant le texte a chiffrer \n Un fichier contenant la clef \n Un fichier qui contiendra le texte chiffre \n");
    }
   

  /** Lecture du texte clair **/
  lecture_texte_clair( texte_clair , &longueur_texte, argv[1]);
  /** Lecture de la clef **/
  lecture_clef( clef , &longueur_clef, argv[2]);
  /** Chiffrement **/
  chiffrer_vigenere( clef, longueur_clef ,  texte_clair, texte_chiffre,longueur_texte);
  /** Ecriture texte chiffre **/
  ecrire_chiffre(texte_chiffre, longueur_texte, argv[3]);

}
