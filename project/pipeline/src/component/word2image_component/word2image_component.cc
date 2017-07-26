#include "word2image_component.h"
#include <Python.h>
#include "component/component_factory.h"
#include "log.h"
#include "text2image.h"
#include "Txt2Img_GAN.h"

using namespace glamorous;

ComponentPointer Word2ImageComponentFactory::create(std::string name) {
    return std::make_shared<Word2ImageComponent>(name);
}

Word2ImageComponent::Word2ImageComponent(std::string name)
    : Component(name) {}

void Word2ImageComponent::process(RequestStatusPointer request) {
    if (request->text.find("flower") == std::string::npos) {
        std::string google_img_path = request->id + "-google.jpg";
        Py_Initialize();
        inittext2image();
        text2image(request->text.c_str(), google_img_path.c_str());
        Py_Finalize();
        cv::Mat google_img = cv::imread(google_img_path.c_str(), 1);
        int origin_width = google_img.cols;
        if (origin_width > 600) {
            double scale = 600. / static_cast<double>(origin_width);
            int scaled_width = static_cast<double>(google_img.cols) * scale;
            int scaled_height = static_cast<double>(google_img.rows) * scale;
            cv::resize(google_img, google_img, cv::Size(scaled_width, scaled_height));
        }
        request->src_img = google_img.clone();
        request->res_img = google_img.clone();
    }
    else {
        Txt2Img_GAN gan(request->text.c_str(), "flower");
        cv::Mat dst = gan.generate();
        cv::resize(dst, dst, {512, 512});
        request->src_img = dst.clone();
        request->res_img = dst.clone();
    }
}
