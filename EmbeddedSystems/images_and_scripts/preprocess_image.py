import cv2
import numpy as np

if (__name__ == '__main__'):
    # parameters
    image_path = "satellite.bmp"
    rgb_txt_filename  = "rgb.txt"
    gray_txt_filename = "gray.txt"

    width  = 64
    height = 64    

    # process
    image = cv2.imread(image_path)
    resized_image = cv2.resize(image, (height,width), interpolation = cv2.INTER_AREA)
    
    image_1d      = resized_image.reshape(height*width*3,1)
    f = open(rgb_txt_filename, "w")
    for index in range(height*width*3):
        f.write(str(image_1d[index][0]))
        f.write("\n")
    f.close()

    resized_image = cv2.resize(image, (height,width), interpolation = cv2.INTER_AREA)
    gray_image = np.ones((height,width))
    for row in range(height):
        for col in range(width):
            gray_image[row,col] = resized_image[row,col,0]*0.3 + resized_image[row,col,1]*0.59 + resized_image[row,col,2]*0.11

    image_1d      = np.array(gray_image.reshape(height*width,1), dtype=np.uint8)
    f = open(gray_txt_filename, "w")
    for index in range(height*width):
        f.write(str(image_1d[index][0]))
        f.write("\n")
    f.close()
