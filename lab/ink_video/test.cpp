#include <string>
#include "ink_video.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    InkVideo iv;
    std::string filename("../a.jpg");
    std::string videoname("video.avi");
    iv.get_video(filename, videoname);
    return 0;
}
