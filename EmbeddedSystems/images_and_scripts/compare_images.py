import cv2
import numpy as np

fark_vektor = np.zeros((255,1))

def readFromTxtFile(filename):
    image_1d = []
    lines    = []

    f = open(filename, "r")        
    lines  = f.readlines()
    f.close()

    for line in lines:
        image_1d.append(int(line.split('\\')[0]))

    return np.array(image_1d).reshape(64*64,1)

def comparePixels(image1_v, image2_v):
    diff_counter = 0
    for (image1_pixel,image2_pixel) in zip(image1_v,image2_v):
        if (image1_pixel != image2_pixel):
            fark_vektor[abs(image1_pixel-image2_pixel)] += 1
            diff_counter += 1
    return diff_counter

if (__name__ == '__main__'):
    # parameters
    txt1_filename = "gray.txt"
    txt2_filename = "result_gray_satellite.txt"

    # process
    image1_v = readFromTxtFile(txt1_filename)
    image2_v = readFromTxtFile(txt2_filename)
    status = comparePixels(image1_v, image2_v)

    print("Toplam fark: ", status)
    print("\nOrtalama fark dağılımı: ")
    frequency, bins = np.histogram(fark_vektor, bins=10, range=[0, 100])
    print(frequency)
