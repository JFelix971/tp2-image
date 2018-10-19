#include "image.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int  nb_cases=100;
int debut_ligne[100];
int fin_ligne[100];
int debut_caract[100];
int fin_caract[100];

//------------------------------------------------------------------------------
// Code source pour le projet d'UE035
// description : (les fonctions sont d�finit dans image.h)
//
// les pointeurs images dans ce code sont de type : struct fichierimage *
//
// fonction struct fichierimage * charger(char *)
// permet de charger une image presente sur le disque en memoire vive, la fonction
// renvoie un pointeur de type : struct fichierimage *
//
// fonction int enregistrer(struct fichierimage *,char *)
// permet d'enregistrer une image sur le disque sous le nom donn� en arg2, cette
// image est contenue dans une pointeur de type : struct fichierimage * fournit en arg1
//
// fonction struct fichierimage * nouveau(int,int)
// permet de creer une image en memoire de largeur arg1 et de hauteur arg2, la fonction
// retourne un pointeur de type : struct fichierimage *
//------------------------------------------------------------------------------
int debut_text(struct fichierimage *fichier)
{
	int i, j;
	for(j=0;j<fichier->entetebmp.largeur; j++)
		for(i=0;i<fichier->entetebmp.hauteur; i++)
			if(fichier->image[i][j].r == 255)
				return j;

}
/*Permet de mettre l'image en niv gris*/
void niv_gris(struct fichierimage *fichier)
{
	int nv_gris=0;
	int i,j;
	for(i=fichier->entetebmp.hauteur;i>=0;i--)
	{
		for(j=0;j<fichier->entetebmp.largeur;j++)
		{
		    nv_gris=(fichier->image[i][j].r + fichier->image[i][j].g + fichier->image[i][j].b)/3;
		    fichier->image[i][j].b=nv_gris;
		    fichier->image[i][j].g=nv_gris;
		    fichier->image[i][j].r=nv_gris;
		}
	}
	enregistrer("image_nvgris.bmp",fichier);
	free(fichier);
}

/*Inversion des contrastes */
void inv_contraste(struct fichierimage *fichier)
{
	int i, j;
	int seuil=128,nbpix=0;

	for(j=0; j<fichier->entetebmp.hauteur; j++)
	{
		for(i=0; i<fichier->entetebmp.largeur; i++)
		{
			if( fichier->image[i][j].r < seuil )
			{
				fichier->image[i][j].r = 255;
				fichier->image[i][j].g = 255;
				fichier->image[i][j].b = 255;
			}
			else
			{
				fichier->image[i][j].r = 0;
				fichier->image[i][j].g = 0;
				fichier->image[i][j].b = 0;

			}
		}
	}
	enregistrer("image_binaire.bmp",fichier);
	supprimer(fichier);
}

void extract_index_ligne(struct fichierimage *fichier)
{
	int i, j;
	int index_line=0;
	int fulline[fichier->entetebmp.hauteur];

	//Init de full line
	for(i=0;i<fichier->entetebmp.hauteur;i++)
		fulline[i]=0;

	//On parcours limage et si cest allume on incremente full line pour determiner les lignes en j
	for(j=0;j<fichier->entetebmp.hauteur; j++)
	{
		for(i=0;i<fichier->entetebmp.largeur;i++)
		{
			if(fichier->image[i][j].r > 0)
			{
				fulline[j]+=1;
			}
		}
	}

	for(j=0;j<fichier->entetebmp.hauteur; j++)
	{
		if(fulline[j] > 0 && fulline[j-1]==0)
		{
			debut_ligne[index_line]=j;
			printf("debut_ligne = %d  index = %d ",debut_ligne[index_line],index_line);
		}
		else if(fulline[j]>0 && fulline[j+1]==0)
		{
			fin_ligne[index_line]=j;
			printf("fin_ligne = %d  index = %d \n",fin_ligne[index_line],index_line);
			index_line+=1;
		}
	}
}

int main()
{
	//Init des tableaux globaux pour index line and caractere
	int i;
	for(i=0;i<nb_cases;i++)
	{
		debut_ligne[i]=0;
		fin_ligne[i]=0;
		debut_caract[i]=0;
		fin_ligne[i]=0;
	}
	// exemple de d�claration d'un pointeur image
	struct fichierimage *fichier=NULL;

	//1- Inversion des Contrastes  de l'image et binarisation de l'image

	fichier = charger ("Document1.bmp");

	inv_contraste(fichier);

	fichier = charger ("image_binaire.bmp");
	extract_index_ligne(fichier);

}
