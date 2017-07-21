import pyimgsaliency as psal
import cv2
import sys

# path to the image
cdef public void photo2sketch(const char * inPath, const char * outPath):
	rbd = psal.get_saliency_rbd(inPath.decode("utf-8")).astype('uint8')
	th = 120
	binary_sal = (rbd > th)
	cv2.imwrite(outPath.decode("utf-8"), 255 *binary_sal.astype('uint8'))

# if __name__ == '__main__':
# 	a1, a2 = sys.argv[1], sys.argv[2]
# 	photo2sketch(a1, a2)
