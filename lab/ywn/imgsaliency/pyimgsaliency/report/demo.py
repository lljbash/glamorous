import pyimgsaliency as psal
import cv2

# path to the image
filenames = ['a.jpg', 'bird.jpg', 'color.png', '2.jpg', '1.jpg']

for i in range(5):
	filename = filenames[i]
	# get the saliency maps using the 3 implemented methods
	rbd = psal.get_saliency_rbd(filename).astype('uint8')

	ft = psal.get_saliency_ft(filename).astype('uint8')

	mbd = psal.get_saliency_mbd(filename).astype('uint8')

	# often, it is desirable to have a binary saliency map

	img = cv2.imread(filename)

	#cv2.imshow('img',img)
	#cv2.imshow('rbd',rbd)
	#cv2.imshow('ft',ft)
	#cv2.imshow('mbd',mbd)

	#openCV cannot display numpy type 0, so convert to uint8 and scale
	th = 120
	binary_sal = (rbd > th)
	cv2.imshow(filename, 255 *binary_sal.astype('uint8'))

#cv2.imshow('binary',255 * binary_sal.astype('uint8'))


cv2.waitKey(0)
