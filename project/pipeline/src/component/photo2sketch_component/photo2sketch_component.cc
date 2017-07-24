#include "photo2sketch_component.h"

#include <string>

#include <Python.h>

#include "component/component_factory.h"
#include "log.h"
#include "photo2sketch.h"

using namespace glamorous;

ComponentPointer Photo2SketchComponentFactory::create(std::string name) {
    return std::make_shared<Photo2SketchComponent>(name);
}

Photo2SketchComponent::Photo2SketchComponent(std::string name) : Component(name) {}

void Photo2SketchComponent::process(RequestStatusPointer request) {
    std::string origin_filename = request->id + "_origin.jpg";
    std::string rcc_filename = request->id + "_RCC.jpg";
    cv::imwrite(origin_filename.c_str(), request->src_img);
    Py_Initialize();
    initphoto2sketch();
    photo2sketch(origin_filename.c_str(), rcc_filename.c_str());
    request->res_rcc_img = cv::imread(rcc_filename.c_str());
    Py_Finalize();
}
