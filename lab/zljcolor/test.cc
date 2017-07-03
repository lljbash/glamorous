#include "test.h"

using namespace cv;

// int h[5] = {62, 79, 171, 205, 210};
// int h[5] = {15, 115, 126, 211, 213};

#define RATE 0.03
float interval[5];

void getOrdered(int *h){
    int data[5];
    for(int i = 0; i < 5; i++)
        data[i] = h[i];
    std::sort(data, data + 5);
    for(int i = 0; i < 5; i++)
        h[i] = data[i];
}

void getIntervals(int *h){
    float f1 = (float)(h[0] + h[1]) / 2.0;
    float f2 = (float)(h[1] + h[2]) / 2.0;
    float f3 = (float)(h[2] + h[3]) / 2.0;
    float f4 = (float)(h[3] + h[4]) / 2.0;
    float x = (float)(h[0] + h[4] - 180) / 2.0;

    interval[0] = f1;
    interval[1] = f2;
    interval[2] = f3;
    interval[3] = f4;
    if(x < 0){
        interval[4] = x + 180;
    }else{
        interval[4] = x;
    }
}

int getWeightedAvg(int hue, int target){
    float res = (RATE * hue + target) / (RATE + 1);
    return (int)res;
}

int getResult(int hue, int *h){
    bool ifRound = (interval[0] > interval[4]);
    if(hue > interval[0] && hue <= interval[1]){
        return getWeightedAvg(hue, h[1]);
    }else if(hue > interval[1] && hue <= interval[2]){
        return getWeightedAvg(hue, h[2]);
    }else if(hue > interval[2] && hue <= interval[3]){
        return getWeightedAvg(hue, h[3]);
    }
    if(ifRound){
        if(hue > interval[4] && hue <= interval[0]){
            return getWeightedAvg(hue, h[0]);
        }
        if(hue <= interval[4] || hue > interval[3]){
            if(hue <= interval[4])
                return getWeightedAvg(hue + 180, h[4]);
            else
                return getWeightedAvg(hue, h[4]);
        }
    }else{
        if(hue > interval[3] && hue <= interval[4]){
            return getWeightedAvg(hue, h[4]);
        }
        if(hue <= interval[4] || hue > interval[0]){
            if(hue <= interval[4])
                return getWeightedAvg(hue + 180, h[0]);
            else
                return getWeightedAvg(hue, h[0]);
        }
    }
    return -1;
}

void changeColor(const char *filename, int *h){
    Mat img = imread(filename);
    Mat hsvimg;
    cvtColor(img, hsvimg, CV_BGR2HSV);

    for(int x = 0; x < hsvimg.rows; x++){
        for(int y = 0; y < hsvimg.cols; y++){
            Vec3b hsv = hsvimg.at<Vec3b>(x, y);
            int Hue = hsv.val[0];
            // int Saturation = hsv.val[1];
            // int Value = hsv.val[2];

            int newHue = getResult(Hue, h);

            hsvimg.at<Vec3b>(x, y)[0] = newHue;
        }
    }

    Mat resultimg;
    cvtColor(hsvimg, resultimg, CV_HSV2BGR);
    imshow("Test", resultimg);

    waitKey();
}

// int main(int argc, char* argv[]) {
//     int h[5] = {62, 79, 171, 205, 210};
//
//     getOrdered(h);
//     getIntervals(h);
//     for(int i = 1; i < argc; i++)
//         changeColor(argv[i], h);
//     return 0;
// }
