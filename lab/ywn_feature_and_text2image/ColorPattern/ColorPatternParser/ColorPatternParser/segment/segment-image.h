/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#ifndef SEGMENT_IMAGE
#define SEGMENT_IMAGE

#include "misc.h"
#include "CommonLib.h"
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
IplImage* segment_image(IplImage* pImg8U3C, float sigma, float c, int min_size, vector<cv::Vec3b>& colors, /*out*/IntImage& imgInd);

#endif
