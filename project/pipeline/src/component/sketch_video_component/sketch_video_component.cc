#include "sketch_video_component.h"
#include "component/component_factory.h"
#include "log.h"
#include "sketch_video.h"

using namespace glamorous;

ComponentPointer SketchVideoComponentFactory::create(std::string name) {
    return std::make_shared<SketchVideoComponent>(name);
}

SketchVideoComponent::SketchVideoComponent(std::string name) : Component(name) {}

void SketchVideoComponent::process(RequestStatusPointer request) {
    std::string src_name = request->id + "-src.jpg";
    std::string video_name = request->id + "-sketch_video.avi";
    cv::imwrite(src_name.c_str(), request->src_img);
    const char *argv[4] = {"", src_name.c_str(), video_name.c_str(), "25"};
    sketch_video::generateMain(4, argv);
    request->video_name = video_name;
}

