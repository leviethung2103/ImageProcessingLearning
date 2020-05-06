clc; clear all;

%switch RGB image to Gray scale image
I = imread('lena.jpg');
G = rgb2gray(I);


%Bayer-5 Dither Array
bayer=[.513 .272 .724 .483 .543 .302 .694 .453;
 .151 .755 .091 .966 .181 .758 .121 .936;
.634 .392 .574 .332 .664 .423 .604 .362;
.060 .875 .211 .815 .030 .906 .241 .845;
.543 .302 .694 .453 .513 .272 .724 .483;
.181 .758 .121 .936 .151 .755 .091 .966;
.664 .423 .604 .362 .634 .392 .574 .332;
.030 .906 .241 .845 .060 .875 .211 .815]*255;

%Classical-4 Dither Array
classical = [.567 .635 .608 .514 .424 .365 .392 .486;
.847 .878 .910 .698 .153 .122 .090 .302;
.820 .969 .941 .667 .180 .031 .059 .333;
.725 .788 .757 .545 .275 .212 .243 .455;
.424 .365 .392 .486 .567 .635 .608 .514;
.153 .122 .090 .302 .847 .878 .910 .698;
.180 .031 .059 .333 .820 .969 .941 .667;
.275 .212 .243 .455 .725 .788 .757 .545]*255;

%Round the values
bayer = round(bayer);
classical = round(classical);

%Get the value of row and column of image then assign to variable 'row',
%'col'
[row col] = size(G);
size_R=floor(row/8);
size_C=floor(col/8);
k=1;
l=1;
if mod(row,8)==0
 k=0 ;  
end
if mod(col,8)==0 
l=0;
end
diff_C=0;
diff_R=0;

for r=1:size_R+k
    if r>size_R
        diff_R=8-mod(row,8); 
    end 
    for c=1:size_C+l    
        if c>size_C
            diff_C=8-mod(col,8);
        end 
            r1=0;
         for R=(r*8)-7:8*r-diff_R
             r1=r1+1;
             c1=0;
             for C=(c*8)-7:8*c-diff_C
             c1=c1+1;
                    if G(R,C) > bayer(r1,c1)
                        Bayer_Image(R,C) = 255; 
                    else 
                        Bayer_Image(R,C) = 0;
                    end
             end
         end 
    end
end
diff_C=0;
diff_R=0;
for r=1:size_R+k
    if r>size_R
        diff_R=8-mod(row,8); 
    end 
    for c=1:size_C+l    
        if c>size_C
            diff_C=8-mod(col,8);
        end 
            r1=0;
         for R=(r*8)-7:8*r-diff_R
             r1=r1+1;
             c1=0;
             for C=(c*8)-7:8*c-diff_C
             c1=c1+1;
                    if G(R,C) > classical(r1,c1)
                        Class_Image(R,C) = 255; 
                    else 
                        Class_Image(R,C) = 0;
                    end
             end
         end 
    end
end

%Show the desired image
figure
imshow(G);
title('Gray Scale Image')

figure 
imshow(Bayer_Image);
title('Ordered Dithering | Halftone Image by using Bayer-5 Dither Array')

figure 
imshow(Class_Image);
title('Ordered Dithering | Halftone Image by using Classical-4 Dither Array')