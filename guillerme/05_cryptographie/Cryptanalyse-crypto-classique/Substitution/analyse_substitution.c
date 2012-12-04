
/**************************************************************************/
/****                                                                   ***/
/****                     Fonction MAIN                                 ***/
/****                                                                   ***/
/**************************************************************************/

#include "outil_substitution.h"
#include <string.h>


int dechiffrement(int *texte_chiffre, int longueur_texte, char *clef)
{
  int i,j,longueur;
  int *frequence_bigrammes=NULL;
  double frequence_lettre[26];
  char commande[100],lettre1,lettre2,lettre;
  char clef_analysee[26];
  char lettre_vers_lettre[10];

  /* Allocation de mémoire  */
  frequence_bigrammes=(int *) malloc(sizeof(int)*26*26);

  /* Calcul de la fréquence des lettres et des bigrammes */
    for(i=0;i<26;i++)
    {
      for(j=0;j<26;j++)
	{
	  frequence_bigrammes[i+26*j]=compter_bigramme(texte_chiffre, longueur_texte,i,j);
	}
    }

    for(i=0;i<26;i++)
    {
      frequence_lettre[i] = ((double) compter_lettre(texte_chiffre,longueur_texte ,i) )/((double) longueur_texte);
    }



  /* On met la clef a zero */
  for(i=0;i<26;i++)
    {
      clef_analysee[i]='*';
    }


  printf("\n Cryptanalyse par substitution \nEn tapant help vous obtiendrez les descriptions des commandes permettants d'analyser les frequences des lettres, et des bigrammes, et de remplacer chaque lettre du chiffre par sa lettre claire correspondante\n");
  // printf("Pour avoir de l'aide taper help");

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
	  printf("\nListe de commmandes :\nexit = quitter le programme, \nhelp = affiche la liste des commandes, \naffecter lettre1->lettre2  =  lettre1 est substituée par lettre2, \ndesaffecter lettre = on enleve l'affectatio associé à lettre dans la clef, \nclair =affiche le message partiellement decrypter \nchiffre = affiche le message chiffre \nclairchiffre longueur  = affiche les longueur-premieres lettres du message clair et en parallele le message chiffre \nclef = affiche la clef partiellement retrouve \nbigramme = affiche les frequences des biagrammes \nmonogramme = affiche les frequences des monogrammes\n");	  
	}

      /*  fin d'analyse */
      else if(  strcmp(commande,"exit") == 0)
	{
	  printf("\n");
	  return(EXIT_SUCCESS);
	}

      /* affiche clair */
      else if(  strcmp(commande,"clair") == 0)
	{
	  printf("\n");
	  affiche_clair(texte_chiffre,longueur_texte,clef_analysee);
	}

      /* affiche chiffre */
      else if(  strcmp(commande,"chiffre") == 0)
	{
	  //	  printf("\n%s\n",texte_chiffre);
	  affiche_alphabetique(texte_chiffre,longueur_texte);
	}

      /* affiche clair chiffre */
      else if(  strcmp(commande,"clairchiffre") == 0)
	{
	  scanf("%d",&longueur);
	  affiche_clair_chiffre(texte_chiffre,longueur_texte,longueur,clef_analysee);

	}

      /* affichage de la clef */
      else if(  strcmp(commande,"clef") == 0)
	{
	  printf("\n");
	  for(i=0;i<26;i++)
	    {
	      printf(" %c ",'a'+ ((char)i));
	    }
	  printf("\n");
	  for(i=0;i<26;i++)
	    {
	      printf(" %c ",clef_analysee[i]);
	    }
	  printf("\n\n");

	}
      /* desaffectation  */
      else if(  strcmp(commande,"desaffecter") == 0)
	{
	  scanf("%s",lettre_vers_lettre);
	  lettre=lettre_vers_lettre[0];
	  if( ( lettre >= 'a') && ( lettre <= 'z'))
	    {
	      clef_analysee[(int)(lettre-'a')] = '*';
		}
	}


      /* analyse de frequence-lettre */
      else if( strcmp(commande,"affecter") == 0)
	{
	  //	  printf("%s",commande);
	  //	  scanf("%c%c",&lettre1,&lettre2);
	  	  scanf("%s",lettre_vers_lettre);
		  lettre1 =  lettre_vers_lettre[0];
		  lettre2=lettre_vers_lettre[3];
		  //  	  scanf("%c",&lettre2);
		  //	  printf("les trucs entree %c%c", lettre1,lettre2);
	  if( (lettre1 <= 'z') && (lettre1 >= 'a')  && (lettre2 <= 'z') && (lettre2 >= 'a') )
	    {
	      clef_analysee[(int) (lettre1-'a')] = lettre2;
	      affiche_clair_chiffre(texte_chiffre,longueur_texte,longueur_texte,clef_analysee);
	    }
	  else
	    {
	      printf("\nAttention : lettre(s) non valide");
	    }
	}


      /* affichage frequence-bigramme*/
      else if(  strcmp(commande,"bigramme") == 0)
	{
	  printf("\n");
	  for(i=0;i<26;i++)
	    {
	      for(j=0;j<26;j++)
		{
		  if(frequence_bigrammes[i+26*j] >= 5)
		    {
		      printf("La frequence du bigramme %c%c vaut %d \n",(char)(((int) 'a') + i),(char)(((int) 'a') + j), frequence_bigrammes[i+26*j]);
		    }
		}
	    }	  
	}

      /* affichage de frequence-lettre*/
      else if(  strcmp(commande,"monogramme") == 0)
	{
	  printf("\n");
	  for(i=0;i<26;i++)
	    {

	      printf("La frequence de  %c vaut %f \n",(char)(((int) 'a') + i), frequence_lettre[i]);

	    }	  
	  printf("\n");
	}



      /* cas de commande non valide */

      else 
	{
	  printf("\nErreur : Nom de commande Invalide");
	}
    }

  return(EXIT_SUCCESS);
}



int main(int argc, char *argv[])
{
  /* Les differents formats du messages */

  int i;

  int longueur_texte=0;
  int texte_clair[N];
  int texte_chiffre[N];

  int clef_substitution[26];
  char clef_analysee[26]; 
  //  char *phrase="mfobecnvxsjukhtwydqgrilpza";
    char *phrase="mfrbucnvasjekhtwydgqoilpzx";
  //char *phrase="bcdefghijklmaopqrstuvwxyzn";
  short test; 



  
  /************************** LECTURE DE LA CLEF ****************************/

  for(i=0;i<26;i++)
    {
      clef_substitution[i]=(int)(phrase[i]- 'a');
 
    }
   
  /************************** LECTURE DU TEXTE CHIFFRE ***********************/
 
  
  /* Erreur  d'argument  */
  if( argc != 2)
  {
    printf("\nErreur : nombre invalide d'arguments");
    printf("\nUsage : %s string string",argv[0]); 
    return(EXIT_FAILURE);
  }
  
   
  test=lecture_texte_clair(texte_chiffre,&longueur_texte, argv[1]);

  /* On traite le probleme d'ouverture de fichier*/
  if( test == EXIT_FAILURE)
    return(EXIT_FAILURE);


  /*************************** CHIFFREMENT *********************************/

  //chiffrer_substitution( clef_substitution,  texte_clair, texte_chiffre,longueur_texte);
  //  affiche_numerique(texte_chiffre,longueur_texte);

  printf("\n\nrogramme est outil qui aide a l'analyse d'un texte chiffre avec une méthode par substitution\n");
  printf("Tapez sur entrée pour continuer :");
  getchar();

  printf("\n Le texte a déchiffrer est le suivant");

  affiche_alphabetique(texte_chiffre,longueur_texte);

  /************************** ANALYSE ********************************/

   dechiffrement( texte_chiffre, longueur_texte, clef_analysee);

    return(EXIT_SUCCESS);

}
