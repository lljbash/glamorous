#! /usr/bin/env python3
#-------------------------------------------------------------------------------
#  Ink Simulation
#  Author:  Gao Tong
#-------------------------------------------------------------------------------
import sys
import cv2
import numpy as np
import matplotlib.pyplot as plt
from pySaliencyMap import pySaliencyMap
from imageAbstraction.src.cartoon import cartoonize

def main():
    if len(sys.argv) < 2:
        print("Please pass image file name as argument!")
        exit()

    img = cv2.imread(sys.argv[1])
    if img is None:
        print("Open image error!")
        exit()

    # 1. Saliency Map
    # initialize
    imgsize = img.shape
    img_width  = imgsize[1]
    img_height = imgsize[0]
    # sm = pySaliencyMap.pySaliencyMap(img_width, img_height)
    # # computation
    # saliency_map = sm.SMGetSM(img)

    # # 2. Abstraction
    # # 2.1 Denoising
    # blur = cv2.bilateralFilter(img, 9, 75, 75)
    #
    # # 2.2
    print('Now Abstracting...')
    cat = cartoonize(img)
    img = cat
    print('Abstraction Done!')

    # img = np.multiply(np.tile(saliency_map, (3,1)), img) + np.multiply(np.tile(1-saliency_map), cat)
    # plt.imshow(img)
    # plt.show()

    # 3. Diffusion
    print('Now Diffusing...')
    R = 0.00001 * img.shape[0] * img.shape[1]
    rndx = np.rint(np.random.random(img.shape[0:2]) * R * 2 - R).astype(int)
    rndy = np.rint(np.random.random(img.shape[0:2]) * R * 2 - R).astype(int)
    img2 = img
    for i in range(0, img.shape[0]):
        for j in range(0, img.shape[1]):
            dx = max(min(rndx[i,j], img.shape[0]-1-i),-i)
            dy = max(min(rndy[i,j], img.shape[1]-1-j),-j)
            img2[i,j,:] = img[i+dx, j+dy, :]
    print('Diffusion Done!')

    print('Now Smoothing...')
    img3 = img2
    for i in range(0, img.shape[0]):
        for j in range(0, img.shape[1]):
            x1 = max(0, i-2)
            x2 = min(img.shape[0], i+3)
            y1 = max(0, j-2)
            y2 = min(img.shape[1], j+3)
            img3[i,j,:] = np.median(img2[x1:x2,y1:y2,:],axis=(0,1))
    print('Smoothing Done!')

    plt.imshow(img3, 'gray')
    plt.show()

    cv2.imwrite('target_step3.jpg', img3)

if __name__ == '__main__':
    main()
