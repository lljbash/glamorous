// Include the C++ shared library header
#include <opencv2/opencv.hpp>
#include "libinkdisp.h"

using namespace cv;

int run_main(int argc, char **argv)
{
    // Set up the application state for the MATLAB Runtime instance created in the application.
    if (!mclInitializeApplication(NULL,0)) {
        std::cerr << "could not initialize the application properly"
                  << std::endl;
        return -1;
    }

    // Load the required MATLAB code into the MATLAB Runtime.
    if( !libinkdispInitialize() ) {
        std::cerr << "could not initialize the library properly"
                  << std::endl;
        return -1;
    }

    try {
        // TODO
        std::cout << "fuck" << std::endl;
        Mat img = imread("../../aaaaa_stylized_shuimo3_g.jpg", 1);
        Size s = img.size();
        int c = img.channels();
        mwArray imgArray;
        if (c == 3) {
            std::cout << "fuck" << std::endl;
            mwSize dims[3] = {s.height, s.width, c};
            std::cout << s.height << "fuck" << s.width << std::endl;
            imgArray = mwArray(c, dims, mxUINT8_CLASS, mxREAL);
            std::cout << "fuck" << std::endl;
            for (int i = 0; i < s.height; i++) {
                for (int j = 0; j < s.width; j++) {
                    for (int ch = 0; ch < c; ch++) {
                         imgArray.Get(c, i+1, j+1, ch+1).SetData(&img.at<Vec3b>(i, j)[ch], 1);
                    }
                }
            }
            std::cout << "fuck" << std::endl;
            mwArray lambda(mxDouble(0.02));
            mwArray alpha(mxDouble(0.8));
            mwArray canny(mxDouble(0.3));
            mwArray img2Array;

            inkdisp(1, img2Array, imgArray, lambda, alpha, canny);

            Mat img2(img);
            for (int i = 0; i < s.height; i++) {
                for (int j = 0; j < s.width; j++) {
                    for (int ch = 0; ch < c; ch++) {
                        double d = img2Array.Get(c, i+1, j+1, ch+1);
                        img2.at<Vec3b>(i, j)[ch] = d * 255;
                    }
                }
            }

            imshow("image", img2);
            waitKey();
        }

        // Catch the MATLAB generated mwException
    } catch (const mwException& e) {
        std::cerr << e.what() << std::endl;
        return -2;

        // Catch any other exceptions that may be thrown
    } catch (...) {
        std::cerr << "Unexpected error thrown" << std::endl;
        return -3;
    }

    // Release the resources used by the generated MATLAB code
    libinkdispTerminate();

    // Release all state and resources used by the MATLAB Runtime for the application
    mclTerminateApplication();
    return 0;
}

int main()
{
    // Initialize the MATLAB Runtime
    mclmcrInitialize();

    // Create a new thread and run the MATLAB generated code in it.
    return mclRunMain((mclMainFcnType)run_main,0,NULL);
}
