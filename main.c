#include "image.h" 
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
	enregistrer("Document_nv_gris.bmp",fichier);
	free(fichier);
}

/*Inversion des contrastes */
void inv_contraste(struct fichierimage *fichier,int seuil)
{
	int i , j ;
	int nv_gris=0 , inv_contraste =0;
	for(i=fichier->entetebmp.hauteur;i>=0;i--)
	{
	    	for(j=0;j<fichier->entetebmp.largeur;j++)
		{
		    nv_gris=fichier->image[i][j].b; // Recuperation de la valeur nv gris
		    inv_contraste = 255 - nv_gris;

		    //Binarisation	
		    if(inv_contraste>seuil)
			{
				fichier->image[i][j].b=255;
				fichier->image[i][j].g=255;
				fichier->image[i][j].r=255;
			}
		    else
			{
				fichier->image[i][j].b=0;
				fichier->image[i][j].g=0;
				fichier->image[i][j].r=0;
			}

		}
	}

	enregistrer("Document_binaire.bmp",fichier);
	free(fichier);
}

void extract_ligne(struct fichierimage *fichier, int num_ligne)
{

}

int main()
{

// exemple de déclaration d'un pointeur image
struct fichierimage *fichier=NULL;


//1er traitement - Mettre l'image en niveau de gris
fichier=charger("Document1.bmp");
niv_gris(fichier);

//2		- Inversion des Contrastes  de l'image et binarisation de l'image

fichier = charger ("Document_nv_gris.bmp");
inv_contraste(fichier,128);


}
