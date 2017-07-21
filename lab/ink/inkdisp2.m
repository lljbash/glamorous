function [ img2 ] = inkdisp2( img, lambda, alpha, canny )
%INKDSIP 墨水扩散2
%   墨水扩散函数
%   推荐 lambda 1.2 alpha 1.2 canny 0.25
% img = img5;
% lambda = 1.2;
% alpha = 1.2;
% canny = 0.25;

[R, C, ~] = size(img);
lab = rgb2lab(img);
AA = lischinski(log(lab(:,:,1)+1), 'Lambda', lambda, 'Alpha', alpha);
I = edge(rgb2gray(img), 'Canny', canny);

w1 = 0.8 * reshape(I, R*C, 1) + 0.1 * ones(R*C,1); %newtest

%%%%%%%%%%%%%
g1 = reshape((~I)*100, R*C, 1); %newtest
g2 = reshape(lab(:,:,2), R*C, 1); %ok
g3 = reshape(lab(:,:,3), R*C, 1); %okim

b1 = w1 .* g1;%newtest
b2 = w1 .* g2;%alltest
b3 = w1 .* g3;%alltest

A2 = AA + spdiags(w1, 0, R*C, R*C);%newtest, alltest
L2 = ichol(A2,struct('michol','on'));%newtest, alltest

tol = 1e-3;
maxit = 1000;
x1 = pcg(A2,b1,tol,maxit,L2,L2');%newtest
x2 = pcg(A2,b2,tol,maxit,L2,L2');
x3 = pcg(A2,b3,tol,maxit,L2,L2');

lab2 = lab;
lab2(:,:,1) = reshape(x1, R, C);
lab2(:,:,2) = reshape(x2, R, C);
lab2(:,:,3) = reshape(x3, R, C);
img2 = lab2rgb(lab2);

end

