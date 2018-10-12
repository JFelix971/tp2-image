#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

struct fichierimage *charger(char *nom)
{
    FILE *f;
    int i,j;
    unsigned char *buffer;
    int position=0;
    long longueur=0;
    struct fichierimage *fichier=(struct fichierimage *)malloc(sizeof(struct fichierimage));

    f=fopen(nom,"rb");
    fseek(f,0,SEEK_END);
    longueur=ftell(f);

    if(longueur < 0)
    {
        printf("Impossible de charger l'image\n");
        free(fichier);
        fclose(f);
        return 0;
    }

    fseek(f,0,SEEK_SET);
    buffer=(unsigned char *)malloc(longueur*sizeof(unsigned char));
    fread(buffer,1,(size_t)longueur,f);
    fclose(f);

// lecture de l'entete du fichier

    memcpy(&fichier->entetefichier.idformat, &buffer[position], 2);
    position+=2;
    memcpy(&fichier->entetefichier.taille_fichier, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetefichier.reserve, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetefichier.debut, &buffer[position], 4);
    position+=4;

/*
printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
printf("reserve                  :%d\n",fichier->entetefichier.reserve);
printf("debut                    :%d\n",fichier->entetefichier.debut);
*/

// lecture de l'entete de l'image

    memcpy(&fichier->entetebmp.taille_image, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetebmp.largeur, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetebmp.hauteur, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetebmp.plans, &buffer[position], 2);
    position+=2;
    memcpy(&fichier->entetebmp.profondeur, &buffer[position], 2);
    position+=2;
    memcpy(&fichier->entetebmp.compression, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetebmp.taille_image_totale, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetebmp.resolutionh, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetebmp.resolutionv, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetebmp.nbrcouleur, &buffer[position], 4);
    position+=4;
    memcpy(&fichier->entetebmp.nbrcouleuri, &buffer[position], 4);
    position+=4;

/*
printf("taille image             :%d\n",fichier->entetebmp.taille_image);
printf("largeur                  :%d\n",fichier->entetebmp.largeur);
printf("hauteur                  :%d\n",fichier->entetebmp.hauteur);
printf("plans                    :%d\n",fichier->entetebmp.plans);
printf("profondeur               :%d\n",fichier->entetebmp.profondeur);
printf("compression              :%d\n",fichier->entetebmp.compression);
printf("taille image totale      :%d\n",fichier->entetebmp.taille_image_totale);
printf("resolution h             :%d\n",fichier->entetebmp.resolutionh);
printf("resulution v             :%d\n",fichier->entetebmp.resolutionv);
printf("nbr couleurs             :%d\n",fichier->entetebmp.nbrcouleur);
printf("nbr couleurs importantes :%d\n",fichier->entetebmp.nbrcouleuri);
*/

    if(fichier->entetebmp.plans!=1||fichier->entetebmp.profondeur!=24||fichier->entetebmp.compression!=0||fichier->entetebmp.nbrcouleur!=0||fichier->entetebmp.nbrcouleuri!=0)
    {
        printf("Impossible de charger l'image\n");
        free(fichier);
        free(buffer);
        return 0;
    }

// mise de l'image en memoire
    fichier->image = (struct pixels**)malloc(fichier->entetebmp.largeur * sizeof(struct pixels*));

    for(j = fichier->entetebmp.hauteur - 1;j >= 0; j--)
    {
        for(i = 0; i < fichier->entetebmp.largeur; i++)
        {
            if(j == fichier->entetebmp.hauteur - 1)
            {
                fichier->image[i] = (struct pixels *) malloc(fichier->entetebmp.hauteur * sizeof(struct pixels));
            }

            memcpy(&fichier->image[i][j].b, &buffer[position], 1);
            position+=1;
            memcpy(&fichier->image[i][j].g, &buffer[position], 1);
            position+=1;
            memcpy(&fichier->image[i][j].r, &buffer[position], 1);
            position+=1;
        }

        position += fichier->entetebmp.largeur%4;
    }

    free(buffer);
    return fichier;
}

int enregistrer(char *nom,struct fichierimage *fichier)
{
    FILE *f;
    int i,j,k;
    unsigned char zero=(unsigned char)0;
    unsigned char *buffer;
    int position=0;
    int longueur=14+fichier->entetefichier.taille_fichier;
    buffer=(unsigned char *)malloc(longueur*sizeof(unsigned char));

// ecriture de l'entete du fichier

/*
printf("id format                :%c%c\n",fichier->entetefichier.idformat[0],fichier->entetefichier.idformat[1]);
printf("taille fichier           :%d\n",fichier->entetefichier.taille_fichier);
printf("reserve                  :%d\n",fichier->entetefichier.reserve);
printf("debut                    :%d\n",fichier->entetefichier.debut);
*/

    memcpy(&buffer[position], &fichier->entetefichier.idformat, 2);
    position+=2;
    memcpy(&buffer[position], &fichier->entetefichier.taille_fichier, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetefichier.reserve, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetefichier.debut, 4);
    position+=4;

// ecriture de l'entete de l'image

/*
printf("taille image             :%d\n",fichier->entetebmp.taille_image);
printf("largeur                  :%d\n",fichier->entetebmp.largeur);
printf("hauteur                  :%d\n",fichier->entetebmp.hauteur);
printf("plans                    :%d\n",fichier->entetebmp.plans);
printf("profondeur               :%d\n",fichier->entetebmp.profondeur);
printf("compression              :%d\n",fichier->entetebmp.compression);
printf("taille image totale      :%d\n",fichier->entetebmp.taille_image_totale);
printf("resolution h             :%d\n",fichier->entetebmp.resolutionh);
printf("resulution v             :%d\n",fichier->entetebmp.resolutionv);
printf("nbr couleurs             :%d\n",fichier->entetebmp.nbrcouleur);
printf("nbr couleurs importantes :%d\n",fichier->entetebmp.nbrcouleuri);
*/

    memcpy(&buffer[position], &fichier->entetebmp.taille_image, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetebmp.largeur, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetebmp.hauteur, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetebmp.plans, 2);
    position+=2;
    memcpy(&buffer[position], &fichier->entetebmp.profondeur, 2);
    position+=2;
    memcpy(&buffer[position], &fichier->entetebmp.compression, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetebmp.taille_image_totale, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetebmp.resolutionh, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetebmp.resolutionv, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetebmp.nbrcouleur, 4);
    position+=4;
    memcpy(&buffer[position], &fichier->entetebmp.nbrcouleuri, 4);
    position+=4;

    if(fichier->entetebmp.plans!=1||fichier->entetebmp.profondeur!=24||fichier->entetebmp.compression!=0||fichier->entetebmp.nbrcouleur!=0||fichier->entetebmp.nbrcouleuri!=0)
    {
        printf("Impossible d'enregistrer l'image\n");
        free(buffer);
        return 0;
    }

    for(j = fichier->entetebmp.hauteur-1; j >= 0;j--)
    {
        for(i = 0;i < fichier->entetebmp.largeur; i++)
        {
            memcpy(&buffer[position], &fichier->image[i][j].b, 1);
            position+=1;
            memcpy(&buffer[position], &fichier->image[i][j].g, 1);
            position+=1;
            memcpy(&buffer[position], &fichier->image[i][j].r, 1);
            position+=1;
        }

        for(k=0;k<fichier->entetebmp.largeur%4;k++)
        {
            memcpy(&buffer[position], &zero, 1);
            position++;
        }
    }

    f=fopen(nom,"wb");
    fwrite(buffer,1,(size_t)longueur,f);
    fclose(f);
    free(buffer);
    return 1;
}

void supprimer(struct fichierimage *fichier)
{
    int i = 0;

    for(i = 0;i < fichier->entetebmp.largeur; i++) {
        free(fichier->image[i]);
    }

    free(fichier->image);

    free(fichier);
}

struct fichierimage *nouveau(int largeur,int hauteur)
{
    int i,j;
    struct fichierimage *fichier;

    if(largeur<=0||hauteur<=0)
    {
        return(NULL);
    }

    fichier=(struct fichierimage *)malloc(sizeof(struct fichierimage));

// ecriture de l'entete du fichier

    fichier->entetefichier.idformat[0] = 'B';
    fichier->entetefichier.idformat[1] = 'M';
    fichier->entetefichier.taille_fichier = 40+3*largeur*hauteur+(largeur%4)*hauteur;
    fichier->entetefichier.reserve = 0;
    fichier->entetefichier.debut = 54;

// ecriture de l'entete de l'image

    fichier->entetebmp.taille_image = 40;
    fichier->entetebmp.largeur = largeur;
    fichier->entetebmp.hauteur = hauteur;
    fichier->entetebmp.plans = 1;
    fichier->entetebmp.profondeur = 24;
    fichier->entetebmp.compression = 0;
    fichier->entetebmp.taille_image_totale= 40 + 3 * largeur * hauteur + (largeur % 4) * hauteur;
    fichier->entetebmp.resolutionh = 1;
    fichier->entetebmp.resolutionv = 1;
    fichier->entetebmp.nbrcouleur = 0;
    fichier->entetebmp.nbrcouleuri = 0;

    fichier->image = (struct pixels**)malloc(fichier->entetebmp.largeur * sizeof(struct pixels*));

    for(i = 0;i < fichier->entetebmp.largeur; i++)
    {
        fichier->image[i] = (struct pixels *) malloc(fichier->entetebmp.hauteur * sizeof(struct pixels));

        for(j = 0; j < fichier->entetebmp.hauteur; j++)
        {
            fichier->image[i][j].b = 0;
            fichier->image[i][j].g = 0;
            fichier->image[i][j].r = 0;
        }
    }

    return fichier;
}

struct fichierimage *clone(struct fichierimage *fichier)
{
    int i;
    struct fichierimage *fichierClone = nouveau(fichier->entetebmp.largeur, fichier->entetebmp.hauteur);

    for(i = 0; i < fichier->entetebmp.largeur; i++)
    {
        memcpy(fichierClone->image[i], fichier->image[i], sizeof(struct pixels) * fichier->entetebmp.hauteur);
    }

    return fichierClone;
}

void setPixelGrey(struct pixels *pixel, unsigned char g) {
    setPixel(pixel, g, g, g);
}

void setPixel(struct pixels *pixel, unsigned char r, unsigned char g, unsigned char b){
    pixel->r = r;
    pixel->g = g;
    pixel->b = b;
}

char egal(struct fichierimage * fichier1, struct fichierimage * fichier2)
{
    int i, j;

    if(fichier1->entetebmp.hauteur == fichier2->entetebmp.hauteur && fichier1->entetebmp.largeur == fichier2->entetebmp.largeur)
    {
        for(i = 0; i < fichier1->entetebmp.largeur; i++)
        {
            for(j = 0; j < fichier1->entetebmp.hauteur; j++)
            {
                if(fichier1->image[i][j].r != fichier2->image[i][j].r || fichier1->image[i][j].g != fichier2->image[i][j].g || fichier1->image[i][j].b != fichier2->image[i][j].b)
                {
                    return 0;
                }
            }
        }

        return 1;
    }

    return 0;
}
