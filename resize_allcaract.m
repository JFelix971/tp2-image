clear all, close all, clc

%Creation dun vecteur pour les images de la bdd
num=[char(48:57)];%Chiffres
ABC= [char(65:90)];%Majuscules
abc=[char(97:122)];%Minuscules

bdd = [num ABC abc];

%Redimensionnement avec une hauteur de 40 puis on va tout agrandir à 128*40
for i=1 : 62
  if i<10
    nom=["BDD_caracteres/caracteres_bmp/img00" num2str(i) "-01002.bmp" ];
    pic=imread(nom);
    buff=imresize(pic,[128 128]);
    bm=["BDD_caracteres/caracteres_bmp/img00" num2str(i) "-01002.bmp" ];
    imwrite(buff,bm,'bmp');
  else
    nom=["BDD_caracteres/caracteres_bmp/img0" num2str(i) "-01002.bmp" ];
    pic=imread(nom);
    [hauteur , largeur]= size(pic);
    buff=imresize(pic,[128 128]);
    imwrite(buff,nom,'bmp');
    end

endfor

rep = dir('Text')
nb_rep=numel(rep)

for i=1 : nb_rep-2
  rep_ligne = dir(["Text/ligne_" num2str(i)] );
  nb_carac=length(find([rep_ligne.isdir]==0))
  for j=1 : nb_carac-1
    nom=["Text/ligne_" num2str(i) "/" num2str(j) ".bmp"];
    pic=imread(nom);
    buff=imresize(pic,[128 128]);
    imwrite(buff,nom,'bmp');
  endfor
  
endfor

%On va comparer ligne par ligne caractere par caractere 
hauteur = 128;
largeur = 128;
fid=fopen('caracteres_rec.txt','w');

for i=1 : nb_rep-2
  rep_ligne = dir(["Text/ligne_" num2str(i)] );
  nb_carac=length(find([rep_ligne.isdir]==0))
  for j=1 : nb_carac-1
    nom_txt=["Text/ligne_" num2str(i) "/" num2str(j) ".bmp"];
    caract_txt=imread(nom_txt);
    tab_p = zeros(1,length(bdd));
   for k=1 : 62
    if k<10
      nom=["BDD_caracteres/caracteres_bmp/img00" num2str(k) "-01002.bmp" ];
      caract_bdd=imread(nom);
      buff_bdd = caract_bdd > 0;
      buff_txt = caract_txt>0;
      compar = buff_bdd + buff_txt;
      C1 = sum( compar(:) == 2);
      C2 = sum(compar(:) == 1);
      tab_p(k)  = (C1)/(C1+C2);
    else
      nom=["BDD_caracteres/caracteres_bmp/img0" num2str(k) "-01002.bmp" ];
      caract_bdd=imread(nom);
      buff_bdd = caract_bdd > 0;
      buff_txt = caract_txt>0;
      compar = buff_bdd + buff_txt;
      C1 = sum( compar(:) == 2);
      C2 = sum(compar(:) == 1);
      tab_p(k)  = (C1)/(C1+C2);
      end
    endfor
  [val car_rec]=max(tab_p);
  fprintf(fid,'%s',bdd(car_rec));
endfor
  fprintf(fid,'\n');
endfor
fclose(fid);
