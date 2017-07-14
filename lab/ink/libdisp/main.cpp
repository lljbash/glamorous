// Include the C++ shared library header
#include <opencv2/opencv.hpp>
#include "ink_dispersion.h"

using namespace glamorous;
using namespace cv;

int main() {
    InkDispersion id;

    Mat img = imread("../../aaaaa_stylized_shuimo3_g.jpg", 1);
    if (img.empty())
        return -1;

    id.initialize(img);
    id.apply();
    Mat img2 = id.get_dst();

    imshow("image", img2);
    waitKey();
    return 0;
}
