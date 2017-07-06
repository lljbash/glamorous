img = imread('target.jpg');
lab = rgb2lab(img);
A = lischinski(log(lab(:,:,1)+1), 'Lambda', 0.2, 'Alpha', 0.8);
I = edge(rgb2gray(img));
edges = zeros(1080,1440,3);
edges = edges + I .* lab;
b = sparse(reshape(edges(:,:,1), 1080*1440,1));
