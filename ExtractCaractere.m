function caractere= ExtractCaractere(img,num_caract)

caracteres=bwlabel(img);
nb_caract=max(caracteres(:));

buff_caract=zeros(32);

[abs,ord]=find(caracteres ==num_caract);
min_x=min(abs);
min_y=min(ord);

max_x=max(abs);
max_y=max(ord);


caractere=caracteres(min_x:max_x,min_y:max_y);
[indx,indy]=find(caractere~=0);
%Centrer dans l'image 32*32
ecart_x=fix((32-(max_x-min_x))/2);
ecart_y=fix((32-(max_y-min_y))/2);

indx= ecart_x+indx;
indy= ecart_y+indy;
% for i=1:size(indx)
%     for j=1:size(indy)
%         if(caractere(indx(i),indy(j))~=0)
%             buff_caract(ecart_x+(indx(i)-1),ecart_y+(indy(j)-1))=1;
%         end
%     end
% end



%Soit cela ou bien au dessous avec for
%[indx,indy]=find(caractere~=0);
indice=sub2ind(size(buff_caract),indx,indy);
buff_caract(indice)=1;
caractere=buff_caract;
size(buff_caract);
%buff_caract(indx,indy)=1;

% figure()
% imshow(buff_caract)

     
