import cv2
import numpy as np

class PostprocessImage:
    def __init__(self,image_path,filename,width,height):
        self.image_path = image_path
        self.filename   = filename
        self.width      = width
        self.height     = height

    def convert2Image(self, image_1d):
        image_1d = image_1d.reshape(self.width,self.height,1)
        return image_1d

    def readFromTxtFile(self):
        image_1d = []
        lines    = []

        f = open(self.filename, "r")        
        lines  = f.readlines()
        f.close()

        for line in lines:
            image_1d.append(int(line.split('\\')[0]))

        return np.array(image_1d).reshape(self.height,self.width,1)

    def writeImage(self, image):
        cv2.imwrite(self.image_path,image)

if (__name__ == '__main__'):
    # parameters
    processObj = PostprocessImage(
        "lenna_processed.bmp",
        "result_gray.txt",
        64,
        64
    )

    # process
    image_1d      = processObj.readFromTxtFile()
    image         = processObj.convert2Image(image_1d)

    # write image
    processObj.writeImage(image)
