clear all, close all, clc

%Redimensionnement avec une hauteur de 40 puis on va tout agrandir à 128*128
for i=1 : 62
  if i<10
    nom=["BDD_caracteres/caracteres_bmp/img00" num2str(i) "-01002.bmp" ];
    pic=imread(nom);
    buff=imresize(pic,[128 40]);
    bm=["BDD_caracteres/caracteres_bmp/img00" num2str(i) "-01002.bmp" ];
    imwrite(buff,bm,'bmp');
  else
    nom=["BDD_caracteres/caracteres_bmp/img0" num2str(i) "-01002.bmp" ];
    pic=imread(nom);
    [hauteur , largeur]= size(pic);
    buff=imresize(pic,[128 40]);
    imwrite(buff,nom,'bmp');
    end
endfor


%rep = dir('Text')
%nb_rep=numel(rep)
%
%for i=1 : nb_rep-2
%  rep_ligne = dir(["Text/ligne_" num2str(i)] );
%  nb_carac=length(find([rep_ligne.isdir]==0));
%  for j=1 : nb_carac
%    nom=["Text/ligne_" num2str(i) "/" num2str(j) ".bmp"];
%    pic=imread(nom);
%    buff=imresize(pic,[128 128]);
%    imwrite(buff,nom,'bmp');
%  endfor
%  
%endfor