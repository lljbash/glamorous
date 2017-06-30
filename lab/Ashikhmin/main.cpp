#include "texture_transfer.h"

using namespace cv;
using namespace glamorous;

int main(int argc, char const *argv[]) {
    Mat src = imread("../b.jpg", 1);
    Mat ref = imread("../g.jpg", 1);
    TextureTransfer tt;
    tt.initialize(src, ref);
    for (int i = 0; i < 5; ++i) {
        tt.scan();
        Mat dst = tt.get_dst();
        imshow("tt", dst);
        waitKey();
    }
    return 0;
}
