#pragma once

#include <string>

#include "pipeline.h"
#include "component/component.h"

class GlamorousApp {
public:
    void initialize(const char *db_path);
    
    // [style]
    // 1: Abstract
    // 2: PostImpressionism
    // 3: Pointillism
    // 4: Shuimo
    // 5: Suprematism
    // 6: Impressionism
    std::string transfer(const char *text, int style);

private:
    glamorous::Pipeline pipeline_;
    std::vector<glamorous::ComponentPointer> cps_;
};

