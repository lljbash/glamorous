#include "etf.h"

using namespace cv;
using namespace glamorous;

// visualize the ETF
void visualize(Mat &flowfield, Mat &dis) {
	Mat noise = Mat::zeros(cv::Size(flowfield.cols / 2, flowfield.rows / 2), CV_32F);
	dis = Mat::zeros(flowfield.size(), CV_32F);
	randu(noise, 0, 1.0f);
	resize(noise, noise, flowfield.size(), 0, 0, INTER_NEAREST);

	int s = 10;
	int nRows = noise.rows;
	int nCols = noise.cols;
	float sigma = 2 * s*s;

	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			float w_sum = 0.0;
			float x = i;
			float y = j;
			for (int k = 0; k < s; k++) {
				Vec3f v = normalize(flowfield.at<Vec3f>(int(x + nRows) % nRows, int(y + nCols) % nCols));
				if (v[0] != 0) x = x + (abs(v[0]) / float(abs(v[0]) + abs(v[1])))*(abs(v[0]) / v[0]);
				if (v[1] != 0) y = y + (abs(v[1]) / float(abs(v[0]) + abs(v[1])))*(abs(v[1]) / v[1]);
				float r2 = k*k;
				float w = (1 / (M_PI*sigma))*exp(-(r2) / sigma);
				int xx = (int(x) + nRows) % nRows;
				int yy = (int(y) + nCols) % nCols;
				dis.at<float>(i, j) += w*noise.at<float>(xx, yy);
				w_sum += w;
			}

			x = i;
			y = j;
			for (int k = 0; k < s; k++) {
				Vec3f v = -normalize(flowfield.at<Vec3f>(int(x + nRows) % nRows, int(y + nCols) % nCols));
				if (v[0] != 0) x = x + (abs(v[0]) / float(abs(v[0]) + abs(v[1])))*(abs(v[0]) / v[0]);
				if (v[1] != 0) y = y + (abs(v[1]) / float(abs(v[0]) + abs(v[1])))*(abs(v[1]) / v[1]);

				float r2 = k*k;
				float w = (1 / (M_PI*sigma))*exp(-(r2) / sigma);
				dis.at<float>(i, j) += w*noise.at<float>(int(x + nRows) % nRows, int(y + nCols) % nCols);
				w_sum += w;
			}
			dis.at<float>(i, j) /= w_sum;
		}
	}
}

void antialias(cv::Mat & src, cv::Mat & dst) {
	const int BLUR_SIZE = 3;

	normalize(src, dst, 60, 255, NORM_MINMAX);
	GaussianBlur(dst, dst, Size(BLUR_SIZE, BLUR_SIZE), 0, 0);
}

int main(int argc, char const *argv[]) {
    Mat image = imread("../e.png", 1), image_gray;
    cvtColor(image, image_gray, cv::COLOR_BGR2GRAY);
    ETF etf;
    etf.init_etf(image_gray);
    etf.refine_etf(6, 3);
    Mat dis;
    Mat f = etf.get_etf();
    visualize(f, dis);
    imshow("etf", dis);
    waitKey();
    return 0;
}
