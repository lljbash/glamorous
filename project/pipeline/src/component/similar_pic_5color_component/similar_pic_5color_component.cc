#include "similar_pic_5color_component.h"
#include <opencv2/opencv.hpp>
#include "component/component_factory.h"
#include "log.h"
#include "image_database.h"

using namespace glamorous;

ComponentPointer SimilarPic5colorComponentFactory::create(std::string name) {
    return std::make_shared<SimilarPic5colorComponent>(name);
}

SimilarPic5colorComponent::SimilarPic5colorComponent(std::string name) : Component(name) {}

void SimilarPic5colorComponent::process(RequestStatusPointer request) {
    Log::info("SimilarPic5colorComponent is processing...");

    // get image attr vec
    const std::vector<ImageAttr>& image_attr_vec_pointer = request->image_db->get_image_list();
    
    // use ColorDifferenceHSV()->get_distance(), able to change.
    std::shared_ptr<ColorDifference> color_diff_alg = std::make_shared<ColorDifferenceHSV>();
    
    // iter over vec to find min_distance
    float min_distance = 0.0;
    int selected_attr_index = 0;
    int length = image_attr_vec_pointer.size();
    for (int i = 0; i < length; ++i) {
    	float distance = 0;
	    for (int j=0; j<5; j++) {
	        cv::Vec3f a_bgr = cv::Vec3f(
	        	image_attr_vec_pointer[j].five_color[j][2],
	        	image_attr_vec_pointer[j].five_color[j][1],
	        	image_attr_vec_pointer[j].five_color[j][0]);

	        cv::Vec3f b_bgr = cv::Vec3f(
	        	request->src_attr.five_color[j][2],
	        	request->src_attr.five_color[j][1],
	        	request->src_attr.five_color[j][0]);
	        
	        distance += this->weight[j] * color_diff_alg->get_distance(a_bgr, b_bgr);
	    }

        if ( i == 0 || distance < min_distance){
            // first one
            min_distance = distance;
            selected_attr_index = i;
        }
    }
    request->ref_attr = image_attr_vec_pointer[selected_attr_index];
    request->ref_img = request->image_db->read_image(selected_attr_index);
}

