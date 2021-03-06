#include "image.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<SDL/SDL.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

int nb_lignes_txt=0;
int  nb_caract=100;
int nb_caract_by_line[30];
int debut_ligne[30];
int fin_ligne[30];
int debut_caracteres[30][100];
int fin_caracteres[30][100];
int ACTIVE=0; // Active= 1  c'est pour lancer la procedure sur lextraction des caracteres
int DEB = 1;
int FIN = 4;//Fin pour extraction caractere, il ne doit jamais avoir plus de 4 de diff avec DEB

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

/*Initialisation des variables globales*/
void init_variables_globales()/*OK*/
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

/*Detecte le plus petit voisin */
int minVoisin(int *tab)/*OK*/
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

/*Detecte la position du premier pixel blanc synonyme debut texte*/
int debut_text(struct fichierimage *fichier)/*OK*/
{
	int i, j;
	for(j=0;j<fichier->entetebmp.largeur; j++)
		for(i=0;i<fichier->entetebmp.hauteur; i++)
			if(fichier->image[i][j].r == 255)
				return j;
}

/*Permet de mettre l'image en niv gris*/
void niv_gris(struct fichierimage *fichier)/*OK*/
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
void inv_contraste(struct fichierimage *fichier)/*OK*/
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

void erosion (struct fichierimage * fichier)/*OK*/
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

void dilatation (struct fichierimage * fichier)/*OK*/
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

void amelioration_eti(struct fichierimage *fichier)/*OK*/
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

/*Extrait les indices debut et fin de chaque caractere par ligne -- Methode imbrique avec extraction ligne*/
void extract_index_caractere2(struct fichierimage *fichier,int line)
{
	int i, j, k;
	int index_caract=0;
	int fullcaract[fichier->entetebmp.largeur];

	for(i=0;i<fichier->entetebmp.largeur;i++)
		fullcaract[i]=0;

	//On parcours limage et si cest allume on incremente full line pour determiner les lignes en j
	for(j=debut_ligne[line];j<fin_ligne[line]; j++)
	{
		for(i=0;i<fichier->entetebmp.largeur;i++)
		{
			if(fichier->image[i][j].r > 0)
			{
				fullcaract[i]+=1;//songer a creer une structure line qui aurait le tableau de i pour chaque ligneou bien un tableau de pointeurs qui pointe sur un tableau
			}
		}
	}
	//Ajout des index de fin et debut de chaque ligne
	for(k=0;k<fichier->entetebmp.largeur; k++)
	{
		if(fullcaract[k] > 0 && fullcaract[k-1]==0)
		{
			debut_caracteres[line][index_caract]=k-2;
			//printf("carac : %d debut : %d ",index_caract,k);
		}
		else if(fullcaract[k] > 0 && fullcaract[k+1]==0)
		{
			fin_caracteres[line][index_caract]=k+2;
			//printf("fin : %d \n",k);
			index_caract+=1;
		}
	}
	nb_caract_by_line[line]=index_caract;
	//printf("ligne : %d nb caract = %d\n",line+1,nb_caract_by_line[line]);
}

/*Extraction des caractere et enregistrement dans les dossiers ligne*/
void extract_caract2(struct fichierimage *fichier,int num_line,int index_caract)
{
	int i=0,j=0,j_bis=0,i_bis=0;;
	int hauteur_ligne=0,largeur_caract=0;
	char nom[50];
	struct fichierimage *buff=NULL, *ligne=NULL;
	hauteur_ligne=fin_ligne[num_line-1] - debut_ligne[num_line-1];
	largeur_caract=fin_caracteres[num_line-1][index_caract] - debut_caracteres[num_line-1][index_caract];

	ligne=clone(fichier);
	//ligne=charger(nom_rep);
	buff=nouveau(largeur_caract,hauteur_ligne);
	//buff=nouveau(ligne->entetebmp.largeur,ligne->entetebmp.hauteur);
	//printf("debut: %d fin: %d \n",debut_ligne[num_line-1],fin_ligne[num_line-1]);
	for(j=debut_ligne[num_line-1]; j<fin_ligne[num_line-1]; j++)
	{
		for(i=debut_caracteres[num_line-1][index_caract]; i<fin_caracteres[num_line-1][index_caract]; i++)
		{
			buff->image[i_bis][j_bis].b=ligne->image[i][j].b;
			buff->image[i_bis][j_bis].g=ligne->image[i][j].g;
			buff->image[i_bis][j_bis].r=ligne->image[i][j].r;
			i_bis++;
		}
		j_bis++;
		i_bis=0;
	}
	sprintf(nom,"Text2/ligne_%d/%d.bmp",num_line,index_caract+1);
	enregistrer(nom,buff);
	supprimer(buff);
}

/*Recuperation des indices debut et fin de chaque ligne*/
void extract_index_line(struct fichierimage *fichier)/*OK*/
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

/*Extraction ligne dans chaque image ligne*/
void extract_line(struct fichierimage *fichier,int num_line)/*OK*/
{
	int i=0,j=0,j_bis=0;
	int hauteur_ligne=0;
	char NOM_REP[50];
	int rep;
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
	sprintf(NOM_REP,"Text2/ligne_%d",num_line);//Modifier si on utilise la premiere methode ou la second
	rep=mkdir(NOM_REP,S_IRWXU);
	sprintf(nom,"Text2/ligne_%d/ligne_%d.bmp",num_line,num_line);//Modifier text selon methode
	enregistrer(nom,buff);
	supprimer(buff);

	if(ACTIVE == 1)
	{
		extract_index_caractere2(fichier,num_line-1);//Extraction des index par num_line
		for(i=0;i<nb_caract_by_line[num_line-1];i++)
		{
			extract_caract2(fichier,num_line,i);
		}
	}
	//supprimer(buff);

}

void recup_lines(struct fichierimage *fichier)/*OK*/
{
	int i;
	for(i=1/*1*/;i<nb_lignes_txt;i++)/* par tranche de 4 max sinon ça plante la machine car calcul torplong*/
		extract_line(fichier,i);
}

/*Methhode 2 extraction index caractere*/
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
					fullcaract[line][i]+=1;//songer a creer une structure line qui aurait le tableau de i pour chaque ligneou bien un tableau de pointeurs qui pointe sur un tableau
				}
			}
		}
		//Ajout des index de fin et debut de chaque ligne
		for(k=0;k<fichier->entetebmp.largeur; k++)
		{
			if(fullcaract[line][k] > 0 && fullcaract[line][k-1]==0)
			{
				debut_caracteres[line][index_caract]=k-2;
				//printf("carac : %d debut : %d ",index_caract,k);
			}
			else if(fullcaract[line][k]>0 && fullcaract[line][k+1]==0)
			{
				fin_caracteres[line][index_caract]=k+2;
				//printf("fin : %d \n",k);
				index_caract+=1;
			}
		}
		nb_caract_by_line[line]=index_caract;
		//printf("ligne : %d nb caract = %d\n",line+1,nb_caract_by_line[line]);
		index_caract=0;
	}
}

void extract_caract(char nom_rep[50],int num_line,int index_caract)
{
	int i=0,j=0,j_bis=0,i_bis=0;;
	int hauteur_ligne=0,largeur_caract=0;
	char nom[50];
	struct fichierimage *buff=NULL, *ligne=NULL;
	hauteur_ligne=fin_ligne[num_line-1] - debut_ligne[num_line-1];
	largeur_caract=fin_caracteres[num_line-1][index_caract] - debut_caracteres[num_line-1][index_caract];
	//printf("debut : %d\t fin: %d\t largeur_caract= %d\n", debut_caracteres[num_line-1][index_caract],fin_caracteres[num_line-1][index_caract],largeur_caract );
	//sprintf(nom_ligne,"Text/ligne_%d/ligne_%d.bmp",num_line,num_line);
	ligne=charger("image_binaire.bmp");
	//ligne=charger(nom_rep);
	buff=nouveau(largeur_caract,hauteur_ligne);
	//buff=nouveau(ligne->entetebmp.largeur,ligne->entetebmp.hauteur);
	//printf("debut: %d fin: %d \n",debut_ligne[num_line-1],fin_ligne[num_line-1]);
	for(j=debut_ligne[num_line-1]; j<fin_ligne[num_line-1]; j++)
	{
		for(i=debut_caracteres[num_line-1][index_caract]; i<fin_caracteres[num_line-1][index_caract]; i++)
		{
			buff->image[i_bis][j_bis].b=ligne->image[i][j].b;
			buff->image[i_bis][j_bis].g=ligne->image[i][j].g;
			buff->image[i_bis][j_bis].r=ligne->image[i][j].r;
			i_bis++;
			//buff->image[i][j].b=ligne->image[i][j].b;
			//buff->image[i][j].g=ligne->image[i][j].g;
			//buff->image[i][j].r=ligne->image[i][j].r;
		}
		j_bis++;
		i_bis=0;
	}
	sprintf(nom,"Text/ligne_%d/%d.bmp",num_line,index_caract+1);
	enregistrer(nom,buff);
	supprimer(buff);
}


void recup_caract()
{
	int i,j;
	char nom_rep[50];
	struct fichierimage *ligne=NULL;
	for(i=22;i<nb_lignes_txt;i++)
	{
		ligne=NULL;
		sprintf(nom_rep,"Text/ligne_%d/ligne_%d.bmp",i,i);
		//ligne=charger(nom_rep);
		printf("fichier : %s nb_carac=  %d\n",nom_rep,nb_caract_by_line[i-1]);

		for(j=0;j<nb_caract_by_line[i-1];j++)
		{
			extract_caract(nom_rep,i,j);
			//printf("j=%d\n",j);
		}
		//supprimer(ligne);
	}
}

/*Efface les bordures noires en largeur de la base*/
void new_bdd(char nom_fichier[50],int debut,int fin,int ind)
{
	int i=0,j=0,j_bis=0,i_bis=0;
	int hauteur_ligne=0,largeur_caract=0;
	int num=0;
	char nom[50];
	char nom_file[50];
	struct fichierimage *buff=NULL, *ligne=NULL;
	num=ind;
	hauteur_ligne=128;
	largeur_caract=fin - debut;

	ligne=charger(nom_fichier);
	//ligne=charger(nom_rep);
	buff=nouveau(largeur_caract,hauteur_ligne);

	for(j=0; j<hauteur_ligne; j++)
	{
		for(i=debut; i<fin; i++)
		{
			buff->image[i_bis][j_bis].b=ligne->image[i][j].b;
			buff->image[i_bis][j_bis].g=ligne->image[i][j].g;
			buff->image[i_bis][j_bis].r=ligne->image[i][j].r;
			i_bis++;
		}
		j_bis++;
		i_bis=0;
	}
	if(num<10)
	{
		sprintf(nom_file,"img00%d-01002.bmp",num);
	}
	else
	{
		sprintf(nom_file,"img0%d-01002.bmp",num);
	}
	sprintf(nom,"BDD_caracteres/caracteres_bmp/%s",nom_file);
	enregistrer(nom,buff);
	supprimer(buff);
}

void  resize_bdd(struct fichierimage *fichier,int num,char nom_fichier[50])
{
	int i, j, k;
	int debut=0, fin=0;
	int fullcaract[fichier->entetebmp.largeur];

	for(i=0;i<fichier->entetebmp.largeur;i++)
		fullcaract[i]=0;

	//On parcours limage et si cest allume on incremente full line pour determiner les lignes en j
	for(j=0;j<fichier->entetebmp.hauteur; j++)
	{
		for(i=0;i<fichier->entetebmp.largeur;i++)
		{
			if(fichier->image[i][j].r > 0)
			{
				fullcaract[i]+=1;
			}
		}
	}
	//Ajout des index de fin et debut de chaque ligne
	for(k=0;k<fichier->entetebmp.largeur; k++)
	{
		if(fullcaract[k] > 0 && fullcaract[k-1]==0)
		{
			debut=k-2;
			//printf("carac : %d debut : %d ",index_caract,k);
		}
		else if(fullcaract[k] > 0 && fullcaract[k+1]==0)
		{
			fin=k+2;
			//printf("fin : %d \n",k);
		}
	}
	new_bdd(nom_fichier,debut,fin,num);
}
/*Fonction permettant de changer les dimensions de la base*/
void bdd()
{
	int i;
	char nom_file[50];
	struct fichierimage *fichier=NULL;

	for(i=1;i<63;i++)
	{
		if(i<10)
		{
			sprintf(nom_file,"BDD_caracteres/caracteres_bmp/img00%d-01002.bmp",i);
		}
		else
		{
			sprintf(nom_file,"BDD_caracteres/caracteres_bmp/img0%d-01002.bmp",i);
		}
		fichier=charger(nom_file);
		resize_bdd(fichier,i,nom_file);
		supprimer(fichier);
	}
}

int main()
{
	// exemple de declaration d'un pointeur image
	struct fichierimage *fichier=NULL;

	//Init init_variables_globales
	init_variables_globales();
	bdd();

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
	/*fichier = charger ("image_binaire.bmp");
	extract_index_caractere(fichier);*/

	//fichier = charger("image_binaire.bmp");
	//recup_caract();

	//2e methode pour compter et etiqueter les caracteres mais moins precises que extract_index_caractere
	/*fichier = charger ("lignes/ligne_1.bmp");
	amelioration_eti(fichier);*/

}
