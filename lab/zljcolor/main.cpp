#include "test.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace cv;

void readFile(char *filename){
    ifstream fin;
    char dir[200] = "./database/colorReplaceTest/";
    strcat(dir, filename);
    fin.open(dir);
    char s[25][200];
    int h[5];
    int r[5], g[5], b[5];
    for(int i = 0; i < 22; i++){
        fin >> s[i];
    }
    int j = 0;
    Mat rgb(5, 1, CV_8UC3);
    Mat hsv(5, 1, CV_8UC3);
    for(int k = 7; k < 22; k += 3){
        r[j] = atoi(s[k]);
        g[j] = atoi(s[k + 1]);
        b[j] = atoi(s[k + 2]);

        rgb.at<Vec3b>(j, 0)[0] = b[j];
        rgb.at<Vec3b>(j, 0)[1] = g[j];
        rgb.at<Vec3b>(j, 0)[2] = r[j];

        cvtColor(rgb, hsv, CV_BGR2HSV);
        h[j] = hsv.at<Vec3b>(j, 0).val[0];
        j++;
    }
    fin.close();

    for(int i = 0; i < 5; i++)
        cout << h[i] << " ";

    getOrdered(h);
    getIntervals(h);
    const char *name = "a.jpg";
    // char *name = "a.png";
    changeColor(name, h);
}

void readFiles(){
    // char filename[200] = "Claude_Monet_Paintings_-_1525_paintings_The_Galettes,_1882_cp.txt";
    // char filename[200] = "Claude_Monet_Paintings_-_1525_paintings_The_Frost,_1885_cp.txt";
    char filename[200] = "Claude_Monet_Paintings_-_1525_paintings_Meadow_at_Giverny,_1888_cp.txt";
    readFile(filename);
}

int main(){
    readFiles();
    return 0;
}
