clear all, close all, clc

%test=imread("1.bmp");
%buff=imresize(test,[128 128]);
%imwrite(buff,'1newpic.bmp','bmp');

%%Script qui permet de changer le format png en bmp - Impossible ou trop galere a faire en C
for i=1 : 62
  if i<10
    nom=["BDD_caracteres/BDD_caracteres/img00" num2str(i) "-01002.png" ];
    pic=imread(nom);
    [hauteur , largeur]= size(pic);
    for j=1 : hauteur
      for k=1 : largeur
        if pic(j,k,1)>128
          pic(j,k,:)=0;
        else
          pic(j,k,:)=255;
          end
        endfor
      endfor
    bm=["BDD_caracteres/caracteres_bmp/img00" num2str(i) "-01002.bmp" ];
    imwrite(pic,bm,'bmp');
  else
    nom=["BDD_caracteres/BDD_caracteres/img0" num2str(i) "-01002.png" ];
    pic=imread(nom);
    [hauteur , largeur]= size(pic);
    for j=1 : hauteur
      for k=1 : largeur
        if pic(j,k,1)>128
          pic(j,k,:)=0;
        else
          pic(j,k,:)=255;
          end
        endfor
      endfor
    bm=["BDD_caracteres/caracteres_bmp/img0" num2str(i) "-01002.bmp" ];
    imwrite(pic,bm,'bmp');
  end
endfor 
