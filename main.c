#include "image.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int nb_lignes_txt=0;
int  nb_caract=100;
int debut_ligne[30];
int fin_ligne[30];
int debut_caracteres[30][100];
int fin_caracteres[30][100];

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

void init_variables_globales()
{
	//Init des tableaux globaux pour index line and caractere
	int i,j;
	for(i=0;i<nb_lignes_txt;i++)
	{
		debut_ligne[i]=0;
		fin_ligne[i]=0;
		for(j=0;j<nb_caract;j++)
		{
			debut_caracteres[i][j]=0;
			fin_caracteres[i][j]=0;
		}
	}
}

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

void extract_index_line(struct fichierimage *fichier)
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

	//Ajout des index de fin et debut de chaque ligne
	for(j=0;j<fichier->entetebmp.hauteur; j++)
	{
		if(fulline[j] > 0 && fulline[j-1]==0)
		{
			debut_ligne[index_line]=j;
			//printf("debut_ligne = %d  index = %d ",debut_ligne[index_line],index_line);
		}
		else if(fulline[j]>0 && fulline[j+1]==0)
		{
			fin_ligne[index_line]=j;
			//printf("fin_ligne = %d  index = %d \n",fin_ligne[index_line],index_line);
			index_line+=1;
		}
	}
	nb_lignes_txt=index_line+1;
}

void extract_line(struct fichierimage *fichier,int num_line)
{
	int i=0,j=0,j_bis=0;
	int hauteur_ligne=0;
	char nom[50];
	struct fichierimage *buff=NULL;
	hauteur_ligne=fin_ligne[num_line-1] - debut_ligne[num_line-1];
	//printf("hauteur line= %d\n",hauteur_ligne );
	buff=nouveau(fichier->entetebmp.largeur,hauteur_ligne);
	//printf(" hauteur buff = %d largeur= %d \n",buff->entetebmp.hauteur,buff->entetebmp.largeur);
	//printf(" hauteur buff = %d largeur= %d \n",fichier->entetebmp.hauteur,fichier->entetebmp.largeur);
	//printf("debut: %d fin: %d \n",debut_ligne[num_line-1],fin_ligne[num_line-1]);
	for(j=debut_ligne[num_line-1]; j<fin_ligne[num_line-1]; j++)
	{
		for(i=0; i<fichier->entetebmp.largeur; i++)
		{
			buff->image[i][j_bis].b=fichier->image[i][j].b;
			buff->image[i][j_bis].g=fichier->image[i][j].g;
			buff->image[i][j_bis].r=fichier->image[i][j].r;

		}
		j_bis++;
	}
	sprintf(nom,"lignes/ligne_%d.bmp",num_line);
	enregistrer(nom,buff);
	supprimer(buff);

}

void recup_lines(struct fichierimage *fichier)
{
	int i;
	for(i=1;i<nb_lignes_txt;i++)
		extract_line(fichier,i);
}

void extract_index_caractere(struct fichierimage *fichier)
{
	int i, j,k;
	int line=0;
	int index_caract=0;
	int fullcaract[nb_lignes_txt][fichier->entetebmp.largeur];

	for(line=0;line<nb_lignes_txt;line++)
		for(i=0;i<fichier->entetebmp.largeur;i++)
			fullcaract[line][i]=0;

	//On parcours limage et si cest allume on incremente full line pour determiner les lignes en j
	for(line=0;line<nb_lignes_txt;line++)
	{
		for(j=debut_ligne[line];j<fin_ligne[line]; j++)
		{
			for(i=0;i<fichier->entetebmp.largeur;i++)
			{
				if(fichier->image[i][j].r > 0)
				{
					fullcaract[line][i]+=1;//songer à creer une structure line qui aurait le tableau de i pour chaque ligneou bien un tableau de pointeurs qui pointe sur un tableau
				}
			}
		}
		//Ajout des index de fin et debut de chaque ligne
		for(k=0;k<fichier->entetebmp.largeur; k++)
		{
			if(fullcaract[line][k] > 0 && fullcaract[line][k-1]==0)
			{
				debut_caracteres[line][index_caract]=k;
			}
			else if(fullcaract[line][k]>0 && fullcaract[line][k+1]==0)
			{
				fin_caracteres[line][index_caract]=k;
				index_caract+=1;
			}
		}
		printf("ligne : %d nb caract = %d\n",line+1,index_caract);
		index_caract=0;
	}
	printf("nb caracteres : %d \n",index_caract);
}

int main()
{
	// exemple de declaration d'un pointeur image
	struct fichierimage *fichier=NULL;

	//Init init_variables_globales
	init_variables_globales();

	//1- Inversion des Contrastes  de l'image et binarisation de l'image
	fichier = charger ("Document1.bmp");
	inv_contraste(fichier);

	//extraction des index de chaque lignes
	fichier = charger ("image_binaire.bmp");
	extract_index_line(fichier);

	//Mettre toutes els lignes extraites dans un dossier pour chaucunes des lignes
	fichier = charger ("image_binaire.bmp");
	//extract_line(fichier,6);
	recup_lines(fichier);

	fichier = charger ("image_binaire.bmp");
	extract_index_caractere(fichier);
}
