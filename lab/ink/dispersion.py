#! /usr/bin/env python3
#-------------------------------------------------------------------------------
#  Ink Dispersion
#  Author:  Gao Tong
#-------------------------------------------------------------------------------
import sys
import cv2
import numpy as np
import matplotlib.pyplot as plt

def main():
    if len(sys.argv) < 2:
        print("Please pass image file name as argument!")
        exit()

    img = cv2.imread(sys.argv[1])
    if img is None:
        print("Open image error!")
        exit()

    th = 300
    edges = cv2.Canny(img, th, 3*th)
    print(sum(sum(edges)) / edges.size)



    cv2.imshow('a', edges)
    cv2.waitKey()
    cv2.destroyAllWindows()



if __name__ == '__main__':
    main()
