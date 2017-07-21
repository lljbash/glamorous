#include "similar_pic_rcc_component.h"
#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

ComponentPointer SimilarPicRccComponentFactory::create(std::string name) {
    return std::make_shared<SimilarPicRccComponent>(name);
}

SimilarPicRccComponent::SimilarPicRccComponent(std::string name) : Component(name) {}

void SimilarPicRccComponent::process(RequestStatusPointer request) {
    const std::vector<ImageAttr>& image_attr_vec_pointer = request->image_db->get_image_list();
    cv::Mat rcc = cv::imread(request->src_attr.rcc_filename.c_str(), 0);
    int length = image_attr_vec_pointer.size();
    
    int selected_attr_index = 0;
    int maxSimilar = 0;
    for (int i = 0; i < length; ++i) {
        std::string fname = image_attr_vec_pointer[i].filename;
        std::string rname = fname.substr(0, fname.size() - 4) + "_RCC.png";
        cv::Mat image = cv::imread(name.c_str(), 0);
        cv::resize(image, image, rcc.size());
        
        int cntSimilar = 0;
        for (int x = 0; x < rcc.cols; ++x) {
            for (int y = 0; y < rcc.rows; ++y) {
                int temp = (image.at(x, y) - 127) ^ (rcc.at(x, y) - 127);
                if (temp < 0) {
                    cntSimilar++;
                }
            }
        }
        
        if (cntSimilar > maxSimilar) {
            maxSimilar = cntSimilar;
            selected_attr_index = i;
        }
    }
    
    request->ref_attr = image_attr_vec_pointer[selected_attr_index];
    request->ref_img = request->image_db->read_image(selected_attr_index);
}
