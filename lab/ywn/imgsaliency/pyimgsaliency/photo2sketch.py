import pyimgsaliency as psal
import cv2
import sys

# path to the image
def photo2sketch(inPath, outPath):
	rbd = psal.get_saliency_rbd(inPath).astype('uint8')
	th = 120
	binary_sal = (rbd > th)
	cv2.imwrite(outPath, 255 *binary_sal.astype('uint8'))

if __name__ == '__main__':
	a1, a2 = sys.argv[1], sys.argv[2]
	photo2sketch(a1, a2)