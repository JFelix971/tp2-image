#include "image.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int nb_lignes_txt=0;
int  nb_caract=100;
int nb_caract_by_line[30];
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
		nb_caract_by_line[i]=0;
		for(j=0;j<nb_caract;j++)
		{
			debut_caracteres[i][j]=0;
			fin_caracteres[i][j]=0;
		}
	}
}

int minVoisin(int *tab)
{
	int i, min=0;
	for (i=0;i<9;i++)
	{
		if (tab[i] < min && tab[i] > 0 || min ==0)
		{
			min = tab[i];
		}

	}

	return min;
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
void erosion (struct fichierimage * fichier)
{
	int i, j, k, minvois=0;
	int voisins[9]={0,0,0,0,0,0,0,0,0};

	struct fichierimage *buff;
	//buff=nouveau(fichier->entetebmp.largeur,fichier->entetebmp.hauteur);
	buff=clone(fichier);
	for(j=1; j<fichier->entetebmp.hauteur-1; j++)
	{
		for(i=1; i<fichier->entetebmp.largeur-1; i++)
		{
			if( fichier->image[i][j].r > 0)
			{
				voisins[0]=fichier->image[i-1][j-1].r;
				voisins[1]=fichier->image[i-1][j].r;
				voisins[2]=fichier->image[i-1][j+1].r;
				voisins[3]=fichier->image[i][j-1].r;
				voisins[4]=fichier->image[i][j+1].r;
				voisins[5]=fichier->image[i+1][j-1].r;
				voisins[6]=fichier->image[i+1][j].r;
				voisins[7]=fichier->image[i+1][j+1].r;
				voisins[8]=fichier->image[i][j].r;
				//On utilise un filtre manuel pour eroder l image , on verifie chaque voisin
				for(k=0;voisins[k] == 0;k++)
				{
					buff->image[i][j].r = 0;
					buff->image[i][j].g = 0;
					buff->image[i][j].b = 0;
				}

			}
		}
	}

	enregistrer("image_erode.bmp",buff);
	supprimer(buff);
	//ouvrir_image(fichier->entetebmp.hauteur,fichier->entetebmp.largeur,"image_erode.bmp");
}

void dilatation (struct fichierimage * fichier)
{
	int i, j, k;
	int x,y;

	struct fichierimage *buff;
	buff=nouveau(fichier->entetebmp.largeur,fichier->entetebmp.hauteur);

	for(j=1; j<fichier->entetebmp.hauteur-1; j++)
	{
		for(i=1; i<fichier->entetebmp.largeur-1; i++)
		{
			if( fichier->image[i][j].r > 0)
			{
				for(x=0;x<3;x++)
					for(y=0;y<3;y++)
					{
						buff->image[i+(x-1)][j+(y-1)].r = 255;
						buff->image[i+(x-1)][j+(y-1)].g = 255;
						buff->image[i+(x-1)][j+(y-1)].b = 255;
					}
			}
		}
	}

	enregistrer("image_dilate.bmp",buff);
	free(buff);

}

int etiquetage(struct fichierimage* fichier)/*OK*/
{
	int i, j, t;
	int etiquette = 1 ;
	int test = 1;
	int voisins[9]={0,0,0,0,0,0,0,0,0};
	int minvois = 0;
	int T[fichier->entetebmp.largeur][fichier->entetebmp.hauteur];

	struct fichierimage *buff;
	buff=nouveau(fichier->entetebmp.largeur,fichier->entetebmp.hauteur);

	for(j=0;j<fichier->entetebmp.hauteur;j++)
		for(i=0;i<fichier->entetebmp.largeur;i++)
			T[i][j]=0;

	while( test == 1)
	{
		test = 0;
		for(j=1; j<fichier->entetebmp.hauteur-1; j++)
		{
			for(i=1; i<fichier->entetebmp.largeur-1; i++)
			{
				if(fichier->image[i][j].r > 0)
				{
					voisins[0]=T[i-1][j-1];
					voisins[1]=T[i-1][j];
					voisins[2]=T[i-1][j+1];
					voisins[3]=T[i][j-1];
					voisins[4]=T[i][j+1];
					voisins[5]=T[i+1][j-1];
					voisins[6]=T[i+1][j];
					voisins[7]=T[i+1][j+1];
					voisins[8]=T[i][j] == 0 ? etiquette : T[i][j];
					minvois=T[i][j]=minVoisin(voisins);
					etiquette += T[i][j] == etiquette;
				}//finsi
			}//fin pour2
		}//finpour1

		//second parours bas en haut, droite gauche
		for(j=fichier->entetebmp.hauteur-1; j>1; j--)
		{
			for(i=fichier->entetebmp.largeur-1; i>1; i--)
			{
				if(fichier->image[i][j].r > 0)
				{
					voisins[0]=T[i-1][j-1];
					voisins[1]=T[i-1][j];
					voisins[2]=T[i-1][j+1];
					voisins[3]=T[i][j-1];
					voisins[4]=T[i][j+1];
					voisins[5]=T[i+1][j-1];
					voisins[6]=T[i+1][j];
					voisins[7]=T[i+1][j+1];
					voisins[8]=T[i][j] == 0 ? etiquette : T[i][j];
					minvois=T[i][j]=minVoisin(voisins);
					etiquette += T[i][j] == etiquette;
				}//finsi
			}//fin pour2
		}//finpour1
	}//fintantque
	//Affichage de l image etiquetter en mettant des couleurs correspondant a chaque etiquette
	for(j=0;j<fichier->entetebmp.hauteur;j++)
		for(i=0;i<fichier->entetebmp.largeur;i++)
		{

				srand(T[i][j]);
				buff->image[i][j].r=rand()%256;
				buff->image[i][j].g=rand()%256;
				buff->image[i][j].b=rand()%256;
		}
	enregistrer("image_etique.bmp",buff);
	supprimer(buff);
	return etiquette;
}

void amelioration_eti(struct fichierimage *fichier)
{
	int i, j, k;
	int etiquette=0;
	int nb_eti[3]={rand()%256,rand()%256,rand()%256};
	int nb_action=0;
	struct fichierimage *buff;
	buff=clone(fichier);

	//for(i=0;i<3;i++)
	//	nb_eti[i]=0;

	while(nb_eti[0]!=nb_eti[1] && nb_eti[1]!=nb_eti[2])
	{
		nb_eti[nb_action%3]=etiquetage(fichier);
		printf(" etiquette = %d \n",nb_eti[nb_action%3]);
		erosion(fichier);
		nb_action++;
		fichier=charger("image_erode.bmp");
		printf(" nb action = %d\n",nb_action);
	}
	for(k=0;k<nb_action;k++)
	{
		dilatation(fichier);
		fichier=charger("image_dilate.bmp");
	}
	fichier=charger("image_dilate.bmp");
	etiquette=etiquetage(fichier);
	printf("nb etiquette = %d\n",etiquette);
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
	for(line=0;line<nb_lignes_txt-1;line++)
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
		nb_caract_by_line[line]=index_caract;
		printf("ligne : %d nb caract = %d\n",line+1,nb_caract_by_line[line]);
		index_caract=0;
	}
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

	//Recup les index des caracters pour chaque ligne et compte le nb de caracteres par ligne
	fichier = charger ("image_binaire.bmp");
	extract_index_caractere(fichier);

	//2e methode pour compter et etiqueter les caracteres mais moins precises que extract_index_caractere
	/*fichier = charger ("lignes/ligne_1.bmp");
	amelioration_eti(fichier);*/
}
