//
// Created by gaswa on 15/09/2018.
//

#ifndef MASTER2_IMAGERIE_IMAGE_H
#define MASTER2_IMAGERIE_IMAGE_H

// structure d'entete du fichier
struct entete_fichier
{
    char idformat[2];
    int taille_fichier;
    int reserve;
    int debut;
};

// structure d'entete de l'image
struct entete_bmp
{
    int taille_image;
    int largeur;
    int hauteur;
    short plans;
    short profondeur;
    int compression;
    int taille_image_totale;
    int resolutionh;
    int resolutionv;
    int nbrcouleur;
    int nbrcouleuri;
};

// structure d'un pixel
struct pixels
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
};

// structure du fichier
struct fichierimage
{
    struct entete_fichier entetefichier;
    struct entete_bmp entetebmp;
    struct pixels **image;
};

struct fichierimage *clone(struct fichierimage *fichier);
void setPixel(struct pixels *pixel, unsigned char r, unsigned char g, unsigned char b);
void setPixelGrey(struct pixels *pixel, unsigned char g);
void supprimer(struct fichierimage *fichier);
struct fichierimage *nouveau(int largeur,int hauteur);
int enregistrer(char *nom,struct fichierimage *fichier);
struct fichierimage *charger(char *nom);


#endif //MASTER2_IMAGERIE_IMAGE_H