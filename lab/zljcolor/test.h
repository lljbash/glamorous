#ifndef TEST_H_
#define TEST_H_

#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>

using namespace cv;

void getOrdered(int *h);
void getIntervals(int *h);
int getWeightedAvg(int hue, int target);
int getResult(int hue, int *h);
void changeColor(const char *filename, int *h);

#endif
