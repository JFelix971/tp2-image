clear all, close all, clc

for i=1 : 62
  if i<10
    nom=["BDD_caracteres/BDD_caracteres/img00" num2str(i) "-01002.png" ];
    pic=imread(nom);
    bm=["BDD_caracteres/caracteres_bmp/img00" num2str(i) "-01002.bmp" ];
    imwrite(pic,bm,'bmp');
  else
    nom=["BDD_caracteres/BDD_caracteres/img0" num2str(i) "-01002.png" ];
    pic=imread(nom);
    bm=["BDD_caracteres/caracteres_bmp/img0" num2str(i) "-01002.bmp" ];
    imwrite(pic,bm,'bmp');
  end
endfor 