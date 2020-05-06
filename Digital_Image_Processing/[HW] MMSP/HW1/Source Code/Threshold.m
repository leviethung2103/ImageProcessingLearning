clc
clear all

%Read the image and change to gray scale image
I = imread('lena.JPG');
G = rgb2gray(I);

%Show the image
figure
imshow(G);
title('Grayscale Image')

figure 
imshow( G > 128)
title('Binary Image with threshold 128')