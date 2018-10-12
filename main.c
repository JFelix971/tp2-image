#include "image.h" 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int first_ligne;
//------------------------------------------------------------------------------
// Code source pour le projet d'UE035
// description : (les fonctions sont définit dans image.h)
//
// les pointeurs images dans ce code sont de type : struct fichierimage *
//
// fonction struct fichierimage * charger(char *)
// permet de charger une image presente sur le disque en memoire vive, la fonction
// renvoie un pointeur de type : struct fichierimage *
//
// fonction int enregistrer(struct fichierimage *,char *)
// permet d'enregistrer une image sur le disque sous le nom donné en arg2, cette 
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

void extract_ligne(struct fichierimage *fichier)
{
	int i, j;
	int stop=0,debut=0;

	//struct fichierimage *text[]= NULL;
	struct fichierimage *ligne = NULL;
	ligne=nouveau(fichier->entetebmp.largeur,fichier->entetebmp.hauteur); 

	debut=debut_text(fichier);
	printf("debut text : %d",debut);
	for(j=debut; j<fichier->entetebmp.hauteur; j++)
	{
		for(i=0; i<fichier->entetebmp.largeur; i++)
		{
			if(fichier->image[i][j].r==255)
			{
				ligne->image[i][j].r=fichier->image[i][j].r;
				ligne->image[i][j].g=fichier->image[i][j].g;
				ligne->image[i][j].b=fichier->image[i][j].b;
			}
			else
			{
				stop++;
			}
		}
		if(stop==fichier->entetebmp.largeur)
		{
			printf("ici stop = %d j:%d\n",stop,j);
			enregistrer("image_ligne1.bmp",ligne);
			supprimer(ligne);
			printf("j = %d\n",j);
			return;
		}
		else
		{
			stop=0;
		}
	}

}

int main()
{

// exemple de déclaration d'un pointeur image
struct fichierimage *fichier=NULL;

//1- Inversion des Contrastes  de l'image et binarisation de l'image

fichier = charger ("Document1.bmp");

inv_contraste(fichier);

fichier = charger ("image_binaire.bmp");
extract_ligne(fichier);

}
