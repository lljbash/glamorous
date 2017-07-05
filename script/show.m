function show()
    Alldata1 = csvread('1.csv');
    Alldata2 = csvread('2.csv');
    Alldata3 = csvread('3.csv');
    Alldata4 = csvread('4.csv');
    Alldata5 = csvread('5.csv');
    Alldata6 = csvread('6.csv');

    satur_x1 = Alldata1(:,1);
    satur_y1 = Alldata1(:,2);
    bright_x1 = Alldata1(:,3);
    bright_y1 = Alldata1(:,4);

    cool1 = Alldata1(:,5);
    cool1 = cool1(1:91);
    cool2 = Alldata2(:,5);
    cool2 = cool2(1:91);
    cool3 = Alldata3(:,5);
    cool3 = cool3(1:91);
    cool4 = Alldata4(:,5);
    cool4 = cool4(1:91);
    cool5 = Alldata5(:,5);
    cool5 = cool5(1:91);
    cool6 = Alldata6(:,5);

    clr1 = Alldata1(:,6);
    clr1 = clr1(1:91);
    clr2 = Alldata2(:,6);
    clr2 = clr2(1:91);
    clr3 = Alldata3(:,6);
    clr3 = clr3(1:91);
    clr4 = Alldata4(:,6);
    clr4 = clr4(1:91);
    clr5 = Alldata5(:,6);
    clr5 = clr5(1:91);
    clr6 = Alldata6(:,6);

    aarea1 = Alldata1(:,7);
    aarea1 = aarea1(1:91);
    aarea2 = Alldata2(:,7);
    aarea2 = aarea2(1:91);
    aarea3 = Alldata3(:,7);
    aarea3 = aarea3(1:91);
    aarea4 = Alldata4(:,7);
    aarea4 = aarea4(1:91);
    aarea5 = Alldata5(:,7);
    aarea5 = aarea5(1:91);
    aarea6 = Alldata6(:,7);

    colour1 = Alldata1(:,8);
    colour1 = colour1(1:91);
    colour2 = Alldata2(:,8);
    colour2 = colour2(1:91);
    colour3 = Alldata3(:,8);
    colour3 = colour3(1:91);
    colour4 = Alldata4(:,8);
    colour4 = colour4(1:91);
    colour5 = Alldata5(:,8);
    colour5 = colour5(1:91);
    colour6 = Alldata6(:,8);

    texture_x1 = Alldata1(:,9);
    texture_y1 = Alldata1(:,10);
    shape_small1 = Alldata1(:,11);
    shape_big1 = Alldata1(:,12);

    cool = [cool1';cool2';cool3';cool4';cool5';cool6'];
    cool = cool';
    clr = [clr1';clr2';clr3';clr4';clr5';clr6'];
    clr = clr';
    aarea = [aarea1';aarea2';aarea3';aarea4';aarea5';aarea6'];
    aarea = aarea';
    colour = [colour1';colour2';colour3';colour4';colour5';colour6'];
    colour = colour';

    subplot(2,2,1);
    boxplot(cool);
    title('cool');
    subplot(2,2,2);
    boxplot(clr);
    title('clear');
    subplot(2,2,3);
    boxplot(aarea);
    title('area');
    subplot(2,2,4);
    boxplot(colour);
    title('color');

    % subplot(3,3,1);
    % hist(cool);
    % title('cool');
    % subplot(3,3,2);
    % hist(clr);
    % title('clear');
    % subplot(3,3,3);
    % hist(aarea);
    % title('area');
    % subplot(3,3,4);
    % hist(colour);
    % title('color');

    % vec_bin_edges = 0:0.1:1;
    % subplot(3,3,5);
    % hist3([satur_x satur_y], 'Edges', {vec_bin_edges, vec_bin_edges});
    % title('saturation');
    % subplot(3,3,6);
    % hist3([bright_x bright_y], 'Edges', {vec_bin_edges, vec_bin_edges});
    % title('brightness');
    % subplot(3,3,7);
    % hist3([texture_x texture_y], 'Edges', {vec_bin_edges, vec_bin_edges});
    % title('texture');

    % subplot(3,3,8);
    % hist(shape_big);
    % title('shape\_overall');
    % subplot(3,3,9);
    % hist(shape_small);
    % title('shape\_detail');
