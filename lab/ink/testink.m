img = imread('/media/psf/Home/glamorous/lab/AdaIN-style/output/guilin_stylized_shuimo6.jpg');

for i=1:5
    for j=1:5
        img2 = inkdisp3(img, i/2, j/2, 0.3);
    end
end
