% extract five colors
% K �Ǿ�����
% realHSV are K * 3 matrix, value 0-1

function [realHSV] = Kmeans_ColorThemeExtraction(root, fileName, K, isFull) 
    % resize ÿһά�����200
    A = imread([root, fileName]);
% 	figure;
% 	imshow(A);
    [sm, sn, ~] = size(A);
    maxRatio = max(round(sm / 200), round(sn / 200)); % ����������
    A = imresize(A, [round(sm / maxRatio), round(sn / maxRatio)]);
    A_RCC = imread(['mask\', fileName(1:length(fileName) - 4), '_RCC.png']);
    A_RCC = imresize(A_RCC, [round(sm / maxRatio), round(sn / maxRatio)]);
    [sm, sn, ~] = size(A);
    if isFull == 0 || isFull == 2
        A_new = [];
        for i = 1:sm
            for j = 1:sn
                if A_RCC(i, j) > 0
                    A_new = [A_new, A(i, j, :)];
                end
            end
        end
        if size(A_new, 2) < K
            realHSV = zeros(K, 3);
        else
            B = reshape(A_new, [], 3);

            % ת��LAB�����
            cform = makecform('srgb2lab');
            B_lab = double(applycform(uint8(B), cform));

            [idx, ~] = kmeans(double(B_lab), K, 'distance', 'sqEuclidean', 'Replicates', 3, 'emptyaction', 'singleton');

            % ͳ�ƾ����ÿ������ص���
            for j=1:K
                idxsum(j) = sum(idx==j);
            end
            [~, pos] = sort(idxsum, 'descend');  % t��������֮���ֵ��pos����idxsum�е�λ��

            % �õ�ÿ�������ĵ���ɫRGB
            realRGB = zeros(K,3);
            for j=1:K
                 realRGB(j,:) = round(mean(B(idx == pos(j), :)));  % ���մӶൽ������
            end

            % show color theme
%             area = round(300 / K);  % ÿһ������ص�����
%             img = zeros(100,300,3);
%             for tt = 1:K
%                 m = area * (tt - 1) + 1;
%                 n = tt * area;
%                 img(:,m:n,1) = realRGB(tt,1);
%                 img(:,m:n,2) = realRGB(tt,2);
%                 img(:,m:n,3) = realRGB(tt,3);
%             end
%             img = uint8(img);
%             figure;
%             imshow(img);

            realHSV = rgb2hsv(realRGB / 255);
            %realHSV(:,3) = realHSV(:,3) /255;
        end
    else
        A_new = [];
        for i = 1:floor(sm/2)
            for j = 1:sn
                if A_RCC(i, j) > 0
                    A_new = [A_new, A(i, j, :)];
                end
            end
        end
        if size(A_new, 2) < K
            realHSV = zeros(K, 3);
        else
            B = reshape(A_new, [], 3);

            % ת��LAB�����
            cform = makecform('srgb2lab');
            B_lab = double(applycform(uint8(B), cform));

            [idx, ~] = kmeans(double(B_lab), K, 'distance', 'sqEuclidean', 'Replicates', 3, 'emptyaction', 'singleton');

            % ͳ�ƾ����ÿ������ص���
            for j=1:K
                idxsum(j) = sum(idx==j);
            end
            [~, pos] = sort(idxsum, 'descend');  % t��������֮���ֵ��pos����idxsum�е�λ��

            % �õ�ÿ�������ĵ���ɫRGB
            realRGB = zeros(K,3);
            for j=1:K
                 realRGB(j,:) = round(mean(B(idx == pos(j), :)));  % ���մӶൽ������
            end

            % show color theme
%             area = round(300 / K);  % ÿһ������ص�����
%             img = zeros(100,300,3);
%             for tt = 1:K
%                 m = area * (tt - 1) + 1;
%                 n = tt * area;
%                 img(:,m:n,1) = realRGB(tt,1);
%                 img(:,m:n,2) = realRGB(tt,2);
%                 img(:,m:n,3) = realRGB(tt,3);
%             end
%             img = uint8(img);
%             imshow(img);

            realHSV = rgb2hsv(realRGB / 255);
            %realHSV(:,3) = realHSV(:,3) /255;
        end
        A_new = [];
        for i = floor(sm/2)+1:sm
            for j = 1:sn
                if A_RCC(i, j) > 0
                    A_new = [A_new, A(i, j, :)];
                end
            end
        end
        if size(A_new, 2) < K
            realHSV = [realHSV; zeros(K, 3)];
        else
            B = reshape(A_new, [], 3);

            % ת��LAB�����
            cform = makecform('srgb2lab');
            B_lab = double(applycform(uint8(B), cform));

            [idx, ~] = kmeans(double(B_lab), K, 'distance', 'sqEuclidean', 'Replicates', 3, 'emptyaction', 'singleton');

            % ͳ�ƾ����ÿ������ص���
            for j=1:K
                idxsum(j) = sum(idx==j);
            end
            [~, pos] = sort(idxsum, 'descend');  % t��������֮���ֵ��pos����idxsum�е�λ��

            % �õ�ÿ�������ĵ���ɫRGB
            realRGB = zeros(K,3);
            for j=1:K
                 realRGB(j,:) = round(mean(B(idx == pos(j), :)));  % ���մӶൽ������
            end

            % show color theme
%             area = round(300 / K);  % ÿһ������ص�����
%             img = zeros(100,300,3);
%             for tt = 1:K
%                 m = area * (tt - 1) + 1;
%                 n = tt * area;
%                 img(:,m:n,1) = realRGB(tt,1);
%                 img(:,m:n,2) = realRGB(tt,2);
%                 img(:,m:n,3) = realRGB(tt,3);
%             end
%             img = uint8(img);
%             imshow(img);

            realHSV = [realHSV; rgb2hsv(realRGB / 255)];
            %realHSV(:,3) = realHSV(:,3) /255;
        end
    end
end