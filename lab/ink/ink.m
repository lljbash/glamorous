%%%%%%%%%%%%%%%%%%%%%%
% Ink Style Modefier %
%%%%%%%%%%%%%%%%%%%%%%

img = imread('target.jpg');
%%
% Sec. 1 Saliency Map
map_itti = ittikochmap(img);
% show_imgnmap(img, map_itti);

%%
% Sec. 2 Abstraction
