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
    cv::Mat rcc = request->res_rcc_img;
    int length = image_attr_vec_pointer.size();
    
    int selected_attr_index = 0;
    int maxSimilar = 0;
    for (int i = 0; i < length; ++i) {
        cv::Mat image = request->image_db->read_rcc(i);
        cv::resize(image, image, rcc.size());
        
        int cntSimilar = 0;
        for (int x = 0; x < rcc.rows; ++x) {
            for (int y = 0; y < rcc.cols; ++y) {
                int temp = (image.at<uchar>(x, y) > 127) ^ (rcc.at<uchar>(x, y) > 127);
                if (temp == 0) {
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
