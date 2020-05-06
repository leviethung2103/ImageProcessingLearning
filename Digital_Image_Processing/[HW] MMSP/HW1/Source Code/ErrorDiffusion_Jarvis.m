% Matlab Implementation of Error diffusion using Jarvis
clc
clear all 

%Read the image and change to gray scale image
I = imread('lena.JPG');
G = rgb2gray(I);

T = 128; % Threshold

%Apply Zeropadding to matrix
OLD = ZeroPadding_Jarvis(G);


%Get the value of row and column of image then assign to variable 'row',
%'col'
[row col] = size(OLD);
NEW = zeros(size(OLD));


 for R =3: (row-2)
     for C = 3: (col-2)
            if (OLD(R,C) < T)
                 NEW(R,C) = 0; 
            else 
                 NEW(R,C) = 255;
            end;
            
            error = OLD(R,C) - NEW(R,C);
          
             OLD(R,C+1) = round(OLD(R,C+1) + (7/48 *error));
             OLD(R,C+2) = round(OLD(R,C+2) + (5/48 *error));
             
             OLD(R+1,C-2) = round(OLD(R+1,C-2) + (3/48 *error));
             OLD(R+1,C-1) = round(OLD(R+1,C-1) + (5/48 *error));
             OLD(R+1,C+0) = round(OLD(R+1,C+0) + (7/48 *error));
             OLD(R+1,C+1) = round(OLD(R+1,C+1) + (5/48 *error));
             OLD(R+1,C+2) = round(OLD(R+1,C+2) + (3/48 *error));
             
             OLD(R+2,C-2) = round(OLD(R+2,C-2) + (1/48 *error));
             OLD(R+2,C-1) = round(OLD(R+2,C-1) + (3/48 *error));
             OLD(R+2,C+0) = round(OLD(R+2,C+0) + (5/48 *error));
             OLD(R+2,C+1) = round(OLD(R+2,C+1) + (3/48 *error));
             OLD(R+2,C+2) = round(OLD(R+2,C+2) + (1/48 *error));
                    
     end
 end

%DePadding Jarvis
NEW = DePadding_Jarvis(NEW);
 
%Show the desired image
figure
imshow(G);
title('Gray Scale Image')

%Show the halftoning image
figure
imshow(NEW);
title('Error diffusion using Jarvis');

NEW = uint8(NEW);

[ssimval, ssimmap] = ssim(G,NEW);
fprintf('The SSIM value is %0.4f.\n',ssimval);
figure, imshow(ssimmap,[]);
title(sprintf('ssim Index Map - Mean ssim Value is %0.4f',ssimval));