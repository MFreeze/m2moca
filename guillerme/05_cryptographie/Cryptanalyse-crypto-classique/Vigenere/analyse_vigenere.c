/*******************************************************************************/
/****                                                                        ***/
/****                          VIGENERE.C                                    ***/
/****                                                                        ***/
/*******************************************************************************/


#include "outil_vigenere.h"
#include <string.h>


/* Dans un premier temps nous essayons de calculer la longueur de la clef, 
pour cela, nous regardons les differents indice de coincidence des sous 
texte 
*/

int analyse_coincidence(int *texte_chiffre, int longueur_texte)
{
  int periode,i;
  char commande[100];
  int longueur_sous_texte=0;
  int sous_texte[N];
  // char clef_analysee[26];

  /* Boucle de decryptage par analyse de frequence */
  while( commande != "exit")
    {
      /* Entree de la commande */
      printf("\nTapez une commande: ");
      scanf("%s",commande);
 
      /*Analyse et traitement de la commande*/

      /* demande d'aide */
      if(  strcmp(commande,"help") == 0)
	{
	  printf("\nListe de commmandes :\n exit = quitter le programme,\nsuite = suite de l'analyse, \n help = affiche la liste des commandes, \n tester periode  =  affiche les coincidences des sous-textes correspondants a la periode donnee \n");	  
	}

      /* demande de quitter */
      else if(  strcmp(commande,"exit") == 0)
	{
	  printf("\n");
	  return(EXIT_FAILURE);
	}
      /* demande de fin d'analyse */
      else if(  strcmp(commande,"suite") == 0)
	{
	  printf("\n");
	  return(EXIT_SUCCESS);
	}

      /* analyse de coincidence */
      else if( strcmp(commande,"tester") == 0)
	{
	  scanf("%d",&periode);
	  if( periode <= 0)
	    {
	      printf("\n Erreur: la periode est un entier strictement positif");
	      continue;
	    }
	  else
	    {

	      printf("\n Indices de Coincidence pour cette periode :");
	      for(i=0;i<periode;i++)
		{
		  extraction_sous_texte( texte_chiffre, longueur_texte, sous_texte, &longueur_sous_texte, periode, i);
		  affiche_alphabetique(sous_texte,longueur_sous_texte);

		  printf(" %f ", indice_coincidence(sous_texte,longueur_sous_texte));
		}
	      printf("\n");
	    }

	}

      /* cas de commande non valide */
      else 
	{
	  printf("\n Erreur : Nom de commande Invalide");
	}
    }
  return(EXIT_SUCCESS);
}


int analyse_decalage(int *texte_chiffre, int longueur_texte,int periode,int *clef_analysee)
{
  int i,j,entree;
  char commande[100];
  int longueur_sous_texte=0,longueur;
  int sous_texte[N];

  /* allocation de memoire pour clef_analysee */
  if( periode > 0) 
    {
      clef_analysee = (int *) malloc(sizeof(int)*periode);
    }

  else{
    return(EXIT_FAILURE);
  }

  for(i=0;i<periode;i++)
    {
      clef_analysee[i]=-1;
    }

  /* Boucle de decryptage par analyse de frequence */
  while( commande != "exit")
    {
      /* Entree de la commande */
      printf("\nTapez une commande: ");
      scanf("%s",commande);
 

      /*Analyse et traitement de la commande*/

      /* demande d'aide */
      if(  strcmp(commande,"help") == 0)
	{
	  printf("\nListe de commmandes :");
	  printf("\nexit = quitter le programme,");
	  printf("\nhelp = affiche la liste des commandes,");
	  printf("\nfrequence j  =  affiche la frequence des lettres du sous texte d'indice j");
	  printf("\naffichage longueur = affichage les premières lettres du texte clair  partiellement retrouve");	  
	  printf("\nclef = affichage la clef  partiellement retrouve \n");	  

	}

      /* demande de fin d'analyse */
      else if(  strcmp(commande,"exit") == 0)
	{
	  printf("\n");
	  return(EXIT_SUCCESS);
	}

     /* affichage texte clair et chiffrer */
      else if(  strcmp(commande,"affichage") == 0)
	{
	  scanf("%d",&longueur);
	  affiche_clair_vigenere(texte_chiffre,longueur_texte,longueur,clef_analysee,periode);
	}
      /* affichage de la clef */
     else if(  strcmp(commande,"clef") == 0)
	{
	  printf("\n");
	  for(i=0;i<periode;i++)
	    {
	      if( clef_analysee[i] == -1)
		{
		  printf(" * ");
		}
	      else
		{
		  printf(" %d ",clef_analysee[i]);
		}
	    }
	  printf("\n");
	}

      /* analyse de coincidence */
      else if( strcmp(commande,"frequence") == 0)
	{
	  scanf("%d",&j);
	  if( (0 < j) && (j <= periode))
	    {
	      extraction_sous_texte( texte_chiffre, longueur_texte, sous_texte, &longueur_sous_texte, periode, j);

	      printf("\n");
	      for(i=0;i<26;i++)
		{
		  printf("La frequence de  %c vaut %f \n",(char)(((int) 'a') + i), ((double) compter_lettre(sous_texte, longueur_sous_texte,i))/((double) longueur_sous_texte) );

		}
	      printf("\nNumerisation\n");
	      for(i=0;i<26;i++)
		{
		  printf(" %c ",'a'+(char)i);
		}
	      printf("\n");
	      for(i=0;i<10;i++)
		{
		  printf(" %d ",i);
		}
	      for(i=10;i<26;i++)
		{
		  printf(" %d",i);
		}
	      printf("\n");
	      printf("\nDecalage pour ce sous texte :");
	      scanf("%d",&entree); 
	      if(j != periode)
		{
		  clef_analysee[j] = entree;
		}
	      else
		{
		  clef_analysee[0] = entree;

		}
	    }
	  else
	    {
	      printf("\nAttention : indice de sous texte invalide");
	    }
	}

      /* cas de commande non valide */
      else 
	{
	  printf("\n Erreur : Nom de commande invalide");
	}
    }
  return(EXIT_SUCCESS);

}



 
int main(int argc, char *argv[])
{
  int i,texte_clair[N],indice; 
  int texte_chiffre[N]; 
  int longueur_texte;
  //int clef[20]={ 2, 15, 17, 3, 20, 23, 11, 5, 1, 9, 11, 19, 15, 17, 19, 12, 21, 2, 2, 3},longueur_clef=20;
  //int clef[10]={ 2, 15, 3, 20, 11, 5, 1, 11, 19, 17},longueur_clef=10;
   int clef[3]={5,21,11},longueur_clef=3;
   int clef1[3]={21,9,21};
  //int clef[6]={2,17,14,16,22,7},longueur_clef=6;

  int *clef_analysee;
//  int longueur_clef=2;
  int periode;
  short int test;

  

  /** Lecture de la clef **/
  


  /* Erreur  d'argument  */
  if( argc != 2)
    {
      printf("\nErreur : nombre invalide d'arguments");
      printf("\nUsage : %s string",argv[0]); 
      return(EXIT_FAILURE);
    }

  
  test=lecture_texte_clair(texte_chiffre,&longueur_texte, argv[1]);
  //  chiffrer_vigenere(clef, longueur_clef, texte_clair, texte_chiffre, longueur_texte);

 
  /* On traite le probleme d'ouverture de fichier*/
  if( test == EXIT_FAILURE)
    return(EXIT_FAILURE);

  /** Chiffrement **/
  printf("\n\nCe programme est un outil qui aide a l'analyse d'un texte chiffre avec une methode de vigenere\n");
  printf("L'analyse va se derouler en deux temps");
  printf("1ere étape : determination de la longueur de la clef de chiffrement par analyse de coincidences \n ");
  printf("2eme étape : determination de la clef par analyse de frequences des sous textes \n");
  printf("Tapez sur entrée pour continuer :");
  getchar();

  printf("\n Le texte a déchiffrer est le suivant");

  affiche_alphabetique(texte_chiffre,longueur_texte);

  printf("\n\nPREMIERE ETAPE : Determination des coefficients de la longueur de la clef par analyse de coincidence");
  printf("\n\nPour avoir de l'aide taper help\n");
    test=analyse_coincidence(texte_chiffre,longueur_texte);

  if( test == EXIT_FAILURE)
    {
      return(EXIT_FAILURE);
    }
  
  printf("Entrez la longueur de la clef :");
  scanf("%d",&periode);

  printf("\n\nDEUXIEME ETAPE : Determination des coefficients de la clefs par analyse de frequence \n");
  printf("\n\nPour avoir de l'aide taper help\n");
   analyse_decalage(texte_chiffre,longueur_texte,periode,clef_analysee);
  
  return(EXIT_SUCCESS);

}
