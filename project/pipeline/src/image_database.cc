#include "image_database.h"
#include "log.h"

using namespace glamorous;

int ImageDataBase::load_tsv(std::string db_path, std::string filename) {
    filename = db_path + "/" + filename;
    db_path_ = db_path;
    std::ifstream in(filename);
    std::vector<std::vector<std::string>> strlist;
    if (!in.is_open()) {
        Log::error("Failed to open %s", filename.c_str());
        return OPEN_FAIL;
    }
    else {
        std::string str;
        while (getline(in, str)) {
            std::string split("\t");
            size_t start = 0;
            size_t end = str.find(split);
            std::vector<std::string> substrlist;
            substrlist.clear();
            while (end != std::string::npos) {
                std::string next = str.substr(start, end - start);
                substrlist.push_back(next);
                start = end + split.length();
                end = str.find(split, start);
            }
            substrlist.push_back(str.substr(start, end));
            strlist.push_back(substrlist);
        }
    }
    in.close();
    Log::info("Successfully open %s", filename.c_str());

    for (std::vector<std::string> slist : strlist) {
        if (slist.size() < 28) {
            Log::error("Invalid Input : too few args");
            continue;
        }
        ImageAttr ia;
        ia.filename = slist[0];

        // color pattern
        ia.saturation_mean = atof(slist[2].c_str());
        ia.saturation_contrast = atof(slist[3].c_str());
        ia.brightness_mean = atof(slist[4].c_str());
        ia.brightness_contrast = atof(slist[5].c_str());
        ia.warm_cool = atof(slist[6].c_str());
        ia.clear_dull = atof(slist[7].c_str());
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                ia.five_color[i][j] = stoi(slist[8 + 3 * i + j]);
            }
        }

        // figure ground
        ia.area_difference = atof(slist[24].c_str());
        ia.color_difference = atof(slist[25].c_str());
        ia.texture_complexity[0] = atof(slist[26].c_str());
        ia.texture_complexity[1] = atof(slist[27].c_str());

        // shape
        /*for (int i = 0; i < 14; i++) {*/
            //ia.shape_match[i] = atofslist[29 + i].c_str());
        //}

        //ia.shape_big = stoi(slist[43]);
        /*ia.shape_small = stoi(slist[44]);*/

        img_list_.push_back(ia);
    }
    Log::info("Successfully load %d files", img_list_.size());
    return READ_SUCCESS;
}

const std::vector<ImageAttr> &ImageDataBase::get_image_list() const {
    return img_list_;
}

cv::Mat ImageDataBase::read_image(int index) const {
    return cv::imread(std::string(db_path_ + "/" + img_list_.at(index).filename).c_str(), 1);
}

cv::Mat ImageDataBase::read_rcc(int index) const {
    std::string name = img_list_.at(index).filename;
    std::string rcc_name = name.substr(0, name.find_last_of('.')) + "_RCC.png";
    return cv::imread(std::string(db_path_ + "/" + rcc_name).c_str(), 1);
}
