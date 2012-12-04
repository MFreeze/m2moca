#include "outil_vigenere.h"


/**************************************************************************/
/****                                                                   ***/
/****              Fonction de comparaison                             ****/
/****                                                                   ***/
/**************************************************************************/


int comp_int(int *a,int *b)
{
  return((*a) - (*b));
}

/**********************************************************************
****                                                                ***
****            Fonction de Lecture du texte Clair                  ***
****                                                                ***
***********************************************************************/

int lecture_texte_clair( int *texte_clair, int *longueur_texte, char *nom_fichier)
{

  char lettre;
  FILE *hand_file;


  (*longueur_texte)=0;

  /* Ouverture du fichier , gerer l'impossibilité d'ouverture du fichier*/

  if( (hand_file = fopen(nom_fichier,"r")) == NULL)
  {
    fprintf(stderr,"\n Erreur: Impossible d'écrire dans le fichier %s \n",nom_fichier);
    return(EXIT_FAILURE);
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
  //printf("\nLe nombre de lettre dans le texte vaut : %d \n",*longueur_texte);
  return(EXIT_SUCCESS);

}




/*************************************************************************/
/****                                                                  ***/
/****                 Fonctions d'affichage d'un texte                 ***/
/****                                                                  ***/
/*************************************************************************/


void affiche_numerique(int *texte,int longueur_texte)
{
  int i;
  printf("\n");
  for(i=0;i<longueur_texte;i++)
    {
      printf(" %d ",texte[i]);
    }
  printf("\n");
}


void affiche_alphabetique(int *texte,int longueur_texte)
{
  int i;
  printf("\n");
  for(i=0;i<longueur_texte;i++)
    {
      printf("%c",(char)(texte[i]+((int) 'a')));
    }
  printf("\n");
  printf("\n La longueur du texte est de %d caractere\n\n", longueur_texte);
}

void affiche_clair(int *texte,int longueur_texte,char *clef_analysee)
{
  int i;
  printf("\n");
  for(i=0;i<longueur_texte;i++)
    {
      printf("%c",clef_analysee[texte[i]] );
    }
  printf("\n");
}


void affiche_clair_chiffre(int *texte, int longueur_texte, int longueur, char *clef_analysee)
{
  int i,j,longueur_ligne=100,nb_ligne;
  if( (longueur > longueur_texte) || (longueur <= 0))
    {
      nb_ligne = longueur_texte/longueur_ligne;
    }
  else
    {
      nb_ligne = longueur/longueur_ligne;
    }

  printf("\n");
  for(i=0;i<nb_ligne;i++)
    {

      /* affichage de la ligne claire */
      for(j=0;j<longueur_ligne;j++)
	{	  
	  printf("%c",clef_analysee[texte[i*longueur_ligne+j]]);
	}
      printf("\n");

      /* affichage de la ligne chiffre */
      for(j=0;j<longueur_ligne;j++)
	{	  
	  printf("%c",(char)(texte[i*longueur_ligne+j]+((int) 'A')));
	}
      printf("\n\n");

    }

  return;
}



void affiche_clair_vigenere(int *texte, int longueur_texte,  int longueur, int *clef_analysee, int periode)
{
  int i,j,longueur_ligne=100,nb_ligne,lettre;

  if( (longueur > longueur_texte) || (longueur <= 0))
    {
      nb_ligne = longueur_texte/longueur_ligne;
    }
  else
    {
      nb_ligne = longueur/longueur_ligne;
    }


  printf("\n");
  for(i=0;i<nb_ligne;i++)
    {

      /* affichage de la ligne claire */
      for(j=0;j<longueur_ligne;j++)
	{	  
	  if( clef_analysee[((i*longueur_ligne+j) % periode)] == -1)
	    {
	      printf("*");
	    }
	  else{

	    lettre=(texte[(i*longueur_ligne+j)] - clef_analysee[((i*longueur_ligne+j) % periode)]);
	    if( lettre >= 26)
	      {
			      printf("%c",( (char) (lettre - 26)) + 'a');
	      }
	    if( lettre < 0 )
	      {
			      printf("%c",( (char) (lettre + 26)) + 'a');
	      }

	    else
	      {
			      printf("%c",( (char) (lettre)) + 'a');
	      }
	  }
	}

      printf("\n");

      /* affichage de la ligne chiffre */
      for(j=0;j<longueur_ligne;j++)
	{	  
	  printf("%c",(char)(texte[i*longueur_ligne+j]+((int) 'A')));
	}
      printf("\n\n");

    }

  return;
}


/****************************************************************************************/
/****                                                                                 ***/
/****     Fonction de chiffrement utilisant la méthode de Substitution                ***/
/****                                                                                 ***/
/****************************************************************************************/


void chiffrer_substitution( int *clef,  int *texte_clair, int *texte_chiffre,int longueur_texte)
{
  int i;
  /*  printf("\n taille clef %d \n",sizeof(*clef)/sizeof(int));*/
  for(i=0;i<longueur_texte;i++)
    {
      texte_chiffre[i] = clef[texte_clair[i]];
    }
  return;

}

  

/****************************************************************************************/
/****                                                                                 ***/
/****     Fonction de chiffrement utilisant la méthode de Vigenère                    ***/
/****                                                                                 ***/
/****************************************************************************************/

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



/****************************************************************************************/
/****                                                                                 ***/
/****       Fonction qui compte le nombre de lettre  ou bigrammes                     ***/
/****                                                                                 ***/
/****************************************************************************************/

int compter_lettre(int *texte,int longueur_texte,int lettre)
{
  int i,compteur=0;

  for(i=0;i<longueur_texte;i++)
    {
      if( texte[i] == lettre)
	{
	  compteur++;
	}

    }

  return(compteur);

}


void comptage_nb_apparition(int *texte, int longueur_texte)
{
  int i,nb_lettre;
  for(i=0;i<26;i++)
    {
      nb_lettre=compter_lettre(texte,longueur_texte,i);
      printf("\n Frequence de %c =  %d ",(char)('a'+i), nb_lettre);

    }
  printf("\n");
  return;
}


void comptage_frequence_apparition(int *texte, int longueur_texte)
{
  int i,nb_lettre;
  for(i=0;i<26;i++)
    {
      nb_lettre=compter_lettre(texte,longueur_texte,i);
      printf("\n Frequence de %c =  %f ",(char)('a'+i),((double) nb_lettre)/((double) longueur_texte));

    }
  printf("\n");
  return;
}


int compter_bigramme(int *texte,int longueur_texte,int lettre1,int lettre2)
{
  int i,compteur=0;

  for(i=0;i<longueur_texte-1;i++)
    {
      if( (texte[i] == lettre1) && (texte[i+1] == lettre2) )
	{
	  compteur++;
	}
    }
  return(compteur);
}



/****************************************************************************************/
/****                                                                             *******/
/****       Fonction de calcul d'indices de coincidences                          *******/
/****                                                                             *******/
/****************************************************************************************/

double indice_coincidence( int *texte, int longueur_texte)
{
  int i;
  double indice=0,p;
  for(i=0;i<longueur_texte;i++)
    {
      p= ((double) compter_lettre(texte,longueur_texte,i))/((double) longueur_texte);
      indice += p*p;
    }
  return(indice);

}

void extraction_sous_texte( int *texte, int longueur_texte, int *sous_texte,int *longueur_sous_texte, int periode, int indice)
{
  int i,t;
  t=longueur_texte/periode;

  /** On extrait le sous texte */
  for(i=0;i<t;i++)
      {
	sous_texte[i]=texte[i*periode+indice];
      }
  /*** Determine la longueur du texte extrait et on recupere une lettre supplementaire si possible ***/
  if( t*periode+indice > longueur_texte)
    {
      (*longueur_sous_texte)=t;
    }
  else
    {
      (*longueur_sous_texte)=t+1;
      sous_texte[t]=texte[t*periode+indice];
    }
}


