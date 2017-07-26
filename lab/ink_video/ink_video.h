#pragma once

#include <string>

namespace glamorous {

class InkVideo {
public:
    void get_video(std::string filename, std::string videoname);
private:
    static const int ITER_TIME = 400;
    static const int IMAGE_SIZE = 1000;
    int cnt_paint_[IMAGE_SIZE][IMAGE_SIZE];
    
    bool check_neighbor(int i, int j);
};

}
