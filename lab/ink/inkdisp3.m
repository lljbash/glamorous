function [ img2 ] = inkdisp3( img, lambda, alpha, canny )
%INKDSIP 墨水扩散2
%   墨水扩散函数
%   推荐 lambda 1 alpha 0.8 canny 0.25
% img = img5;
% lambda = 1.2;
% alpha = 1.2;
% canny = 0.25;

[R, C, ~] = size(img);
lab = rgb2lab(img);
AA = lischinski(log(lab(:,:,1)+1), 'Lambda', lambda, 'Alpha', alpha);
I = edge(rgb2gray(img), 'Canny', canny);

w1 = 0.8 * reshape(I, R*C, 1) + 0.1 * ones(R*C,1); %newtest
w = reshape(I, R*C, 1); %ok
%%%%%%%%%%%%% Deal with edges: g1
t = lab(:,:,1);
for i=1:R
    for j=1:C
        if (I(i,j))
            x1=max(0, i-1);
            x2=min(R-1,i+1);
            y1=max(0, j-1);
            y2=min(C-1, j+1);
            t(i,j) = sum(sum(lab(x1:x2,y1:y2,1).*I(x1:x2,y1:y2))) /  sum(sum(I(x1:x2,y1:y2)));
        end
    end
end
%%%%%%%%%%%%% Change in g1;

%%%%%%%%%%%%%
g1 = reshape((~I)*100, R*C, 1); %newtest
%g1 = reshape(I .* t + (~I) .* lab(:,:,1), R*C, 1);
g2 = reshape(lab(:,:,2), R*C, 1); %ok
g3 = reshape(lab(:,:,3), R*C, 1); %okim

b1 = w1 .* g1;%newtest
b2 = w1 .* g2;%alltest
b3 = w1 .* g3;%alltest
% b1 = w .* g1;
% b2 = w .* g2;
% b3 = w .* g3;

% A = AA + spdiags(w, 0, R*C, R*C);
A2 = AA + spdiags(w1, 0, R*C, R*C);%newtest, alltest
% L = ichol(A,struct('michol','on'));
L2 = ichol(A2,struct('michol','on'));%newtest, alltest

tol = 1e-3;
maxit = 1000;
x1 = pcg(A2,b1,tol,maxit,L2,L2');%newtest
x2 = pcg(A2,b2,tol,maxit,L2,L2');
x3 = pcg(A2,b3,tol,maxit,L2,L2');
% x2 = pcg(A,b2,tol,maxit,L,L');
% x3 = pcg(A,b3,tol,maxit,L,L');

lab2 = lab;
lab2(:,:,1) = reshape(x1, R, C);
lab2(:,:,2) = reshape(x2, R, C);
lab2(:,:,3) = reshape(x3, R, C);
img2 = lab2rgb(lab2);

end

