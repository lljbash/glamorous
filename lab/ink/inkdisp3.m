function [ img2 ] = inkdisp3( img, lambda, alpha, canny )
%INKDSIP 墨水扩散2
%   墨水扩散函数
%   推荐 lambda 1 alpha 0.5 canny 0.3

[R, C, ~] = size(img);
lab = rgb2lab(img);
AA = lischinski(log(lab(:,:,1)+1), 'Lambda', lambda, 'Alpha', alpha);
I = edge(rgb2gray(img), 'Canny', canny);

w1 = 0.8 * reshape(I, R*C, 1) + 0.1 * ones(R*C,1); %newtest

g1 = reshape((~I)*100, R*C, 1); %newtest

b1 = w1 .* g1;%newtest

A2 = AA + spdiags(w1, 0, R*C, R*C);%newtest, alltest
L2 = ichol(A2,struct('michol','on'));%newtest, alltest

tol = 1e-3;
maxit = 1000;
x1 = pcg(A2,b1,tol,maxit,L2,L2');%newtest

lab2 = lab;
lab2(:,:,1) = reshape(x1, R, C);
lab2(:,:,1) = (1-lab2(:,:,1)./100) .* lab2(:,:,1) + lab2(:,:,1)./100 .* lab(:,:,1);
img2 = lab2rgb(lab2);

end
