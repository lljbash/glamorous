import pyimgsaliency as psal
import cv2

# path to the image
def photo2sketch(inPath, outPath):
	rbd = psal.get_saliency_rbd(inPath).astype('uint8')
	th = 120
	binary_sal = (rbd > th)
	cv2.imwrite(outPath, 255 *binary_sal.astype('uint8'))
