#include "photo2sketch_component.h"

#include <Python.h>
#include <string>

#include "component/component_factory.h"
#include "log.h"
#include "photo2sketch.h"

using namespace glamorous;

ComponentPointer Photo2SketchComponentFactory::create(std::string name) {
    return std::make_shared<Photo2SketchComponent>(name);
}

Photo2SketchComponent::Photo2SketchComponent(std::string name) : Component(name) {}

void Photo2SketchComponent::process(RequestStatusPointer request) {
    std::string rcc = request->id + "_RCC.jpg";
    Py_Initialize();
    initphoto2sketch();
    photo2sketch(request->src_attr.filename.c_str(), rcc.c_str());
    Py_Finalize();
}
