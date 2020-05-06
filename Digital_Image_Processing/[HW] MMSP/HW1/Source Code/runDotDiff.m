clc 
clear all

%Read the image and change to gray scale image
I = imread('lena.JPG');
G = rgb2gray(I);


%Run Dot Diffusion
R = dotdiff(G,2);
%Show the desired image
figure
imshow(G);
title('Gray Scale Image')


%Show the resutl image
figure
imshow(R);
title('Dot Diffusion Image')