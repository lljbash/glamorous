#include "similar_pic_5color_component.h"
#include "component/component_factory.h"
#include "log.h"
#include "image_database.h"
#include <utils/color_difference_hsv.h>
#include <utils/color_difference_rgb.h>

using namespace glamorous;

ComponentPointer SimilarPic5colorComponentFactory::create(std::string name) {
    return std::make_shared<SimilarPic5colorComponent>(name);
}

SimilarPic5colorComponent::SimilarPic5colorComponent(std::string name) : Component(name) {}

void SimilarPic5colorComponent::process(RequestStatusPointer request) {
    Log::info("SimilarPic5colorComponent is processing, but it's just an example and will do nothing.");
    // TODO: assign ref_attr according to src_img's 5color(most similar)
    // get image attr vec
    std::vector<ImageAttr>* image_attr_vec_pointer;
    image_attr_vec_pointer = request->image_db->get_image_list();
    
    // use ColorDifferenceHSV, able to change.
    ColorDifference* color_diffInstance = new ColorDifferenceHSV();
    
    // iter over vec to find min_distance
    float min_distance;
    cv::Vec3b* selected_attr;
    cv::Vec3b* cmp_attr = request->src_attr;
    
}
