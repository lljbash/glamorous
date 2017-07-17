#pragma once

#include "component.h"

namespace glamorous {

class ComponentFactory {
public:
    virtual ComponentPointer create(std::string name) = 0;
};

class SimpleComponentFactory : ComponentFactory {
public:
    ComponentPointer create(std::string name = "SimpleComponent") override;
};

class ColorTransferComponentFactory : ComponentFactory {
public:
    ColorTransferComponentFactory(float level = 0.4);
    ComponentPointer create(std::string name = "ColorTransferComponent") override;
private:
    float level_;
};

class MeanContrastTransferComponentFactory : ComponentFactory {
public:
    ComponentPointer create(std::string name = "MeanContrastTransferComponent") override;
};

class OilpaintTransferComponentFactory : ComponentFactory {
public:
    ComponentPointer create(std::string name = "OilpaintTransferComponent") override;
};

class ColorAttributeExtractorComponentFactory : ComponentFactory {
public:
    ComponentPointer create(std::string name = "ColorAttributeExtractorComponent") override;
};

class InkTransferComponentFactory : ComponentFactory {
public:
    ComponentPointer create(std::string name = "InkTransferComponent") override;
};

class Word2ImageComponentFactory : ComponentFactory {
public:
    ComponentPointer create(std::string name = "Word2ImageComponent") override;
};

class DatabaseMatchComponentFactory : ComponentFactory {
public:
    DatabaseMatchComponentFactory(std::string root_dir);
    ComponentPointer create(std::string name = "DatabaseMatchComponent") override;
private:
    std::string root_dir_;
}

class SimilarPic5colorComponentFactory : ComponentFactory {
public:
	ComponentPointer create(std::string name = "SimilarPic5colorComponent") override;
};

} // namespace glamorous
