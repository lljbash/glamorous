img = imread('aaaaa_stylized_shuimo3_g.jpg');
[R, C, ~] = size(img);
lab = rgb2lab(img);
AA = lischinski(log(lab(:,:,1)+1), 'Lambda', 0.02, 'Alpha', 0.8);
I = edge(rgb2gray(img), 'Canny', 0.3);

w1 = 0.9 * reshape(I, R*C, 1) + 0.1 * ones(R*C,1); %ok
w = reshape(I, R*C, 1); %ok
% w = ones(R*C,1);
% w = reshape((1-lab(:,:,1)/100), R*C, 1);
% g1 = reshape(lab(:,:,1), R*C, 1);
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
%%%%%%%%%%%%%
% g1 = reshape((~I)*50, R*C, 1); %ok
% g1 = reshape(lab(:,:,1), R*C, 1); %oldtry
g1 = reshape(I .* t + (~I) .* lab(:,:,1), R*C, 1);
g2 = reshape(lab(:,:,2), R*C, 1); %ok
g3 = reshape(lab(:,:,3), R*C, 1); %okim
% w = ~w * 0 + (w).*g;
%%%%%%%
% logicI = true(R, C);
% logicI(2:R-1,2:C-1) = false(R-2,C-2);
% myI = logical(I + logicI);
% myW = reshape(myI, R*C, 1);
%%%%%%%
b1 = w .* g1;
b2 = w .* g2;
b3 = w .* g3;

A1 = AA + spdiags(w1, 0, R*C, R*C);
A = AA + spdiags(w, 0, R*C, R*C);
% A = AA + spdiags(myW, 0, R*C, R*C);
% M = AA + eye(R*C, 'like', AA);
L = ichol(A,struct('michol','on'));

tol = 1e-3;
maxit = 1000;
sprintf('Part 1!')
[x1, f1, r1, i1] = pcg(A,b1,tol,maxit,L,L');
sprintf('Part 2!')
[x2, f2, r2, i2] = pcg(A,b2,tol,maxit,L,L');
sprintf('Part 3!')
[x3, f3, r3, i3] = pcg(A,b3,tol,maxit,L,L');

lab2 = lab;
lab2(:,:,1) = reshape(x1, R, C);
lab2(:,:,2) = reshape(x2, R, C);
lab2(:,:,3) = reshape(x3, R, C);
img2 = lab2rgb(lab2);
figure; imshow(img2);
% figure; imshow(lab2(:,:,1),[0 100]);

% newimg = lab2(:,:,1)/100*img + (1-lab2(:,:,1)/100)*img2;
% figure; imshow(newimg);