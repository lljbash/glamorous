#include "stdafx.h"
//#include "../CommonLib.h"

#include <cstdlib>
#include "image.h"
#include "filter.h"
#include "pnmfile.h"
#include "segment-graph.h"
#include "segment-image.h"

// dissimilarity measure between pixels
static inline float diff(image<float> *r, image<float> *g, image<float> *b,
						 int x1, int y1, int x2, int y2) 
{
	return sqrt(square(imRef(r, x1, y1)-imRef(r, x2, y2)) +
		square(imRef(g, x1, y1)-imRef(g, x2, y2)) +
		square(imRef(b, x1, y1)-imRef(b, x2, y2)));
}

/*
* Segment an image
*
* Returns a color image representing the segmentation.
* 
* Input:
*	im: image to segment.
*	sigma: to smooth the image.
*	c: constant for threshold function.
*	min_size: minimum component size (enforced by post-processing stage).
*	num_ccs: number of connected components in the segmentation.
* Output:
*	colors: colors assigned to each components
*	pImgInd: index of each components
*/
IplImage* segment_image(IplImage* pImg8U3C, float sigma, float c, int min_size, vector<cv::Vec3b>& colors, /*out*/IntImage& imgInd) 
{
	image<rgb> *im = Ipl2Image(pImg8U3C);

	int width = im->width();
	int height = im->height();

	imgInd = IntImage::zeros(height,width);

	image<float> *r = new image<float>(width, height);
	image<float> *g = new image<float>(width, height);
	image<float> *b = new image<float>(width, height);

	// smooth each color channel  
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			imRef(r, x, y) = imRef(im, x, y).r;
			imRef(g, x, y) = imRef(im, x, y).g;
			imRef(b, x, y) = imRef(im, x, y).b;
		}
	}
	image<float> *smooth_r = smooth(r, sigma);
	image<float> *smooth_g = smooth(g, sigma);
	image<float> *smooth_b = smooth(b, sigma);
	delete r;
	delete g;
	delete b;

	// build graph
	edge *edges = new edge[width*height*4];
	int num = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x < width-1) {
				edges[num].a = y * width + x;
				edges[num].b = y * width + (x+1);
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y);
				num++;
			}

			if (y < height-1) {
				edges[num].a = y * width + x;
				edges[num].b = (y+1) * width + x;
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x, y+1);
				num++;
			}

			if ((x < width-1) && (y < height-1)) {
				edges[num].a = y * width + x;
				edges[num].b = (y+1) * width + (x+1);
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y+1);
				num++;
			}

			if ((x < width-1) && (y > 0)) {
				edges[num].a = y * width + x;
				edges[num].b = (y-1) * width + (x+1);
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y-1);
				num++;
			}
		}
	}
	delete smooth_r;
	delete smooth_g;
	delete smooth_b;

	// segment
	universe *u = segment_graph(width*height, num, edges, c);

	// post process small components
	for (int i = 0; i < num; i++) {
		int a = u->find(edges[i].a);
		int b = u->find(edges[i].b);
		if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
			u->join(a, b);
	}
	delete [] edges;

	//*num_ccs = u->num_sets();

	// pick random colors for each component
	map<int, int> marker;
	IplImage* pImgShow = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int comp = u->find(y * width + x);

			if (marker.find(comp) == marker.end())
			{
				marker[comp] = (int)colors.size();
				colors.push_back(random_rgb());
			}
			int idx = marker[comp];
			imgInd(y,x) = idx;
			CV_IMAGE_ELEM(pImgShow, cv::Vec3b, y, x) = colors[idx];
		}
	}
	delete u;
	delete im;
	return pImgShow;
}
