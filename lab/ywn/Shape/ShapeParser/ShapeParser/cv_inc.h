#pragma once

//#pragma warning(push,0)
//#include <cv_210/cv.h>
//#include <cv_210/highgui.h>
//#include <cv_210/cv.hpp>
//#include <cv_210/cvaux.hpp>
//#include <cv_210/ml.h>
//#pragma warning(pop)
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/legacy/compat.hpp>

#undef EXIT //well, it is really ugly for opencv defining EXIT -> goto exit
//#pragma warning(pop)
#ifdef _DEBUG
	#pragma comment(lib, "opencv_highgui2413d.lib")
	#pragma comment(lib, "opencv_core2413d.lib")
	#pragma comment(lib, "opencv_legacy2413d.lib")
	#pragma comment(lib,"opencv_features2d2413d.lib")
	#pragma comment(lib,"opencv_imgproc2413d.lib")
	#pragma comment(lib,"opencv_video2413d.lib")
#else
	#pragma comment(lib, "opencv_highgui2413.lib")
	#pragma comment(lib, "opencv_core2413.lib")
	#pragma comment(lib, "opencv_legacy2413.lib")
	#pragma comment(lib,"opencv_features2d2413.lib")
	#pragma comment(lib,"opencv_imgproc2413.lib")
	#pragma comment(lib,"opencv_video2413.lib")
#endif
