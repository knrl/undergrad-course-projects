import os
import sys
import cv2
import numpy as np
import albumentations as A
import matplotlib.pyplot as plt
from skimage.transform import resize
from skimage.io import imread, imshow

class DataManager:
    def __init__(self, path='/content/drive/MyDrive/datasets/dataset2'):
        self.path = path
        self.images_path = self.path + '/' + 'Data'
        self.masks_path = self.path + '/' + 'Masks'

        self.images_size = None
        self.height = 512
        self.width = 512
        self.channels = 1

        self.normalize_data = True

    def read_data(self, augmentation=False):
        train_list = os.listdir(self.images_path)
        mask_list = os.listdir(self.masks_path)
        self.images_size = len(mask_list)

        data = np.zeros((self.images_size, self.height, self.width, self.channels), dtype=np.float32)
        masks = np.zeros((self.images_size, self.height, self.width, self.channels), dtype=np.float32)

        index = 0
        for img_name in train_list:
            mask_name = img_name.split('.')[0] + '_mask.png'
            if (mask_name in mask_list):
                image = cv2.imread(self.images_path + '/' + img_name, cv2.IMREAD_GRAYSCALE)
                image = cv2.resize(image, dsize=(self.width, self.height), interpolation=cv2.INTER_CUBIC)
                data[index] = np.expand_dims(image, axis=-1)

                mask = cv2.imread(self.masks_path + '/' + mask_name, cv2.IMREAD_GRAYSCALE)
                mask = cv2.resize(mask, dsize=(self.width, self.height), interpolation=cv2.INTER_CUBIC)
                mask = (mask != 0).astype(np.float32)
                masks[index] = np.expand_dims(mask, axis=-1)
                index += 1
        if (self.normalize_data):
            data = data / 255
        if (augmentation):
            data, masks = self.augment_data_and_masks(data, masks)
        print('Dataset: read_data():\nimage dataset: ', data.shape, '\nmask dataset: ', masks.shape, '\n')
        return data, masks

    def augment_data_and_masks(self, data, masks):
        IMG_HEIGHT, IMG_WIDTH, IMG_CH = self.height, self.width, 1
        aug_data = np.zeros((6 * len(data), IMG_HEIGHT, IMG_WIDTH, IMG_CH), dtype=np.float32)
        aug_masks = np.zeros((6 * len(data), IMG_HEIGHT, IMG_WIDTH, IMG_CH), dtype=np.float32)
        n = 0
        for index in range(len(data)):
            img = data[index]
            mask_img = masks[index]

            transformed_data = self.transform_data(img, mask_img, IMG_HEIGHT, IMG_WIDTH, IMG_CH)
            for i in range(6):
                aug_data[n] = np.array(transformed_data[0][i])
                aug_masks[n] = np.array(transformed_data[1][i])
                n += 1
        return aug_data, aug_masks

    def transform_data(self, image, mask, IMG_HEIGHT, IMG_WIDTH, IMG_CH):
        transformed_image = np.zeros((6, IMG_HEIGHT, IMG_WIDTH, IMG_CH), dtype=np.float32)
        transformed_mask = np.zeros((6, IMG_HEIGHT, IMG_WIDTH, IMG_CH), dtype=np.float32)

        transform1 = A.Compose([A.RandomContrast(always_apply=False, p=1.0, limit=(-0.45, 0.45))])
        transform2 = A.Compose([A.RandomRotate90(p=1.0)])
        transform3 = A.Compose([A.VerticalFlip(p=1.0)])
        transform4 = A.Compose([A.GaussNoise(always_apply=False, p=1.0, var_limit=(40., 150.))])
        transform5 = A.Compose([A.Cutout(always_apply=False, p=1.0, num_holes=4, max_h_size=50, max_w_size=50)])

        transformed_image[0] = image
        transformed_mask[0] = mask
        for i in range(5):
            if (i == 0):
                transformed = transform1(image=image, mask=mask)
            elif (i == 1):
                transformed = transform2(image=image, mask=mask)
            elif (i == 2):
                transformed = transform3(image=image, mask=mask)
            elif (i == 3):
                transformed = transform4(image=image, mask=mask)
            elif (i == 4):
                transformed = transform5(image=image, mask=mask)
            transformed_image[i+1] = transformed['image']
            transformed_mask[i+1] = transformed['mask']
        return transformed_image, transformed_mask

    def read_single_image(self, path):
        data = np.zeros((1, self.height, self.width, 1), dtype=np.float32)
        img = imread(path, 0)
        data[0] = np.expand_dims(resize(img, (self.height, self.width), mode='constant', preserve_range=True), axis=-1)
        if (self.normalize_data):
            data = data / 255.
        return data

    def shuffle_data(self, data, masks):
        from numpy import random

        len_of_data = len(data)
        for i in range(len_of_data//2):
            x = random.randint(len_of_data)
            temp = data[x]
            data[x] = data[i]
            data[i] = temp

            temp = masks[x]
            masks[x] = masks[i]
            masks[i] = temp
        return data, masks
