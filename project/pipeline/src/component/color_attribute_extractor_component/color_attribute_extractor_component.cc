#include "color_attribute_extractor_component.h"
#include "component/component_factory.h"
#include "utils/color_attribute/five_color_extractor.h"
#include "utils/color_attribute/simple_color_feature_extractor.h"
#include "log.h"

using namespace glamorous;

ComponentPointer ColorAttributeExtractorComponentFactory::create(std::string name) {
    return std::make_shared<ColorAttributeExtractorComponent>(name);
}

ColorAttributeExtractorComponent::ColorAttributeExtractorComponent(std::string name)
    : Component(name) {}

void ColorAttributeExtractorComponent::process(RequestStatusPointer request) {
    SimpleColorFeatureExtractor scfe;
    SimpleColorFeatureExtractor::SimpleColorFeature scf = scfe.extract(request->src_img);
    request->src_attr.saturation_mean = scf.saturation_mean;
    request->src_attr.saturation_contrast = scf.saturation_contrast;
    request->src_attr.brightness_mean = scf.brightness_mean;
    request->src_attr.brightness_contrast = scf.brightness_contrast;
    request->src_attr.warm_cool = scf.warm_cool;
    request->src_attr.clear_dull = scf.clear_dull;
    
    FiveColorExtractor fce;
    FiveColorExtractor::FiveColor fc = fce.extract(request->src_img);
    memcpy(request->src_attr.five_color, fc.five_color, sizeof(fc.five_color));
}
