#include "ink_dispersion.h"

#include <functional>

#include "non-cpp/matlab/ink/libinkdisp.h"
#include "log.h"

using namespace glamorous;
using namespace cv;

InkDispersion::InkDispersion()
    : lambda_(DEFAULT_LAMBDA), alpha_(DEFAULT_ALPHA), canny_(DEFAULT_CANNY) {

}

InkDispersion::~InkDispersion() {

}

void InkDispersion::setLambda(double l) {
    lambda_ = l;
}

void InkDispersion::setAlpha(double a) {
    alpha_ = a;
}

void InkDispersion::setCanny(double c) {
    canny_ = c;
}

void InkDispersion::initialize(const cv::Mat &src) {
    src_ = src.clone();
    dst_ = src_.clone();
}

int InkDispersion::apply() {
    // Initialize the MATLAB Runtime
    mclmcrInitialize();

    // Create a new thread and run the MATLAB generated code in it.
    // std::function<int(int, const char **)> static_main = [this](int argc, const char **argv) { return this->run_main(argc, argv); };
    // return mclRunMain((mclMainFcnType)static_main, 0, NULL);
    return run_main(0, NULL);
}

cv::Mat InkDispersion::get_dst() const {
    return dst_.clone();
}

int InkDispersion::run_main(int argc, const char **argv)
{
    // Set up the application state for the MATLAB Runtime instance created in the application.
    if (!mclInitializeApplication(NULL, 0)) {
        Log::error("could not initialize the application properly");
        return -1;
    }

    // Load the required MATLAB code into the MATLAB Runtime.
    if(!libinkdispInitialize()) {
        Log::error("could not initialize the library properly");
        return -1;
    }

    try {
        cv::Size s = src_.size();
        int c = src_.channels();
        mwArray imgArray;
        if (c == 3) {
            mwSize dims[3] = {s.height, s.width, c};
            imgArray = mwArray(c, dims, mxUINT8_CLASS, mxREAL);
            for (int i = 0; i < s.height; i++) {
                for (int j = 0; j < s.width; j++) {
                    for (int ch = 0; ch < c; ch++) {
                         imgArray.Get(c, i+1, j+1, ch+1).SetData(&src_.at<Vec3b>(i, j)[ch], 1);
                    }
                }
            }

            mwArray lambda(lambda_);
            mwArray alpha(alpha_);
            mwArray canny(canny_);
            mwArray img2Array;

            inkdisp(1, img2Array, imgArray, lambda, alpha, canny);

            for (int i = 0; i < s.height; i++) {
                for (int j = 0; j < s.width; j++) {
                    for (int ch = 0; ch < c; ch++) {
                        double d = img2Array.Get(c, i+1, j+1, ch+1);
                        dst_.at<Vec3b>(i, j)[ch] = d * 255;
                    }
                }
            }
        }

        // Catch the MATLAB generated mwException
    } catch (const mwException& e) {
        Log::error(e.what());
        return -2;

        // Catch any other exceptions that may be thrown
    } catch (...) {
        Log::error("Unexpected error thrown");
        return -3;
    }

    // Release the resources used by the generated MATLAB code
    libinkdispTerminate();

    // Release all state and resources used by the MATLAB Runtime for the application
    mclTerminateApplication();
    return 0;
}
