#include <string>
#include "ink_video.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    InkVideo iv;
    cv::Mat image = cv::imread(argv[1], 0);
    iv.get_video(image, "video.avi");
    return 0;
}
