#include "ink_video_component.h"

#include <string>
#include "component/component_factory.h"
#include "log.h"
#include "ink_video.h"

using namespace glamorous;

ComponentPointer InkVideoComponentFactory::create(std::string name) {
    return std::make_shared<InkVideoComponent>(name);
}

InkVideoComponent::InkVideoComponent(std::string name) : Component(name) {}

void InkVideoComponent::process(RequestStatusPointer request) {
    InkVideo iv;
    std::string videoname = request->id + "-ink_video.avi";
    iv.get_video(request->res_img, videoname);
    request->video_name = videoname;
    Log::info("Ink Video : %s", videoname.c_str());
}

