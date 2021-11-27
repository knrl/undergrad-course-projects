import numpy as np

import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import backend as K
from scipy.ndimage import distance_transform_edt as distance

class LossFunctions:
    def __init__(self):
        pass

    def dice_coef(self, y_true, y_pred, smooth=1):
        # reference:
        # ref: https://arxiv.org/pdf/1606.04797v1.pdf
        intersection = K.sum(K.abs(y_true * y_pred), axis=-1)
        return (2. * intersection + smooth) / (K.sum(K.square(y_true),-1) + K.sum(K.square(y_pred),-1) + smooth)

    def dice_loss(self, y_true, y_pred, smooth=1):
        return 1 -  self.dice_coef(y_true, y_pred, smooth)

    def binary_focal_loss(self, y_true, y_pred):
        gamma, alpha = 2., 0.25
        alpha = tf.constant(alpha, dtype=tf.float32)
        gamma = tf.constant(gamma, dtype=tf.float32)

        y_true = tf.cast(y_true, tf.float32)
        alpha_t = y_true*alpha + (K.ones_like(y_true)-y_true)*(1-alpha)
    
        p_t = y_true*y_pred + (K.ones_like(y_true)-y_true)*(K.ones_like(y_true)-y_pred) + K.epsilon()
        focal_loss = - alpha_t * K.pow((K.ones_like(y_true)-p_t),gamma) * K.log(p_t)
        return K.mean(focal_loss)

    def combo_loss(self, targets, inputs):
        # reference:
        # https://github.com/asgsaeid/ComboLoss/blob/master/combo_loss.py
        smooth = 1
        e = 1e-07
        ALPHA = 0.5
        CE_RATIO = 0.
        targets = K.flatten(targets)
        inputs = K.flatten(inputs)

        intersection = K.sum(K.abs(targets * inputs), axis=-1)
        dice = (2. * intersection + smooth) / (K.sum(K.square(targets),-1) + K.sum(K.square(inputs),-1) + smooth)
        inputs = K.clip(inputs, e, 1.0 - e)
        out = - (ALPHA * ((targets * K.log(inputs)) + ((1 - ALPHA) * (1.0 - targets) * K.log(1.0 - inputs))))
        weighted_ce = K.mean(out, axis=-1)
        combo = (CE_RATIO * weighted_ce) - ((1 - CE_RATIO) * dice)
        return combo

    def tversky(self, y_true, y_pred):
        # reference:
        # https://github.com/nabsabraham/focal-tversky-unet/blob/master/losses.py
        epsilon = 1e-5
        smooth = 1
        y_true_pos = K.flatten(y_true)
        y_pred_pos = K.flatten(y_pred)
        true_pos = K.sum(y_true_pos * y_pred_pos)
        false_neg = K.sum(y_true_pos * (1-y_pred_pos))
        false_pos = K.sum((1-y_true_pos)*y_pred_pos)
        alpha = 0.7
        return (true_pos + smooth)/(true_pos + alpha*false_neg + (1-alpha)*false_pos + smooth)

    def tversky_loss(self, y_true, y_pred):
        # reference:
        # https://github.com/nabsabraham/focal-tversky-unet/blob/master/losses.py
        return 1 - self.tversky(y_true,y_pred)

    def focal_tversky(self, y_true,y_pred):
        pt_1 = self.tversky(y_true, y_pred)
        gamma = 0.75
        return K.pow((1-pt_1), gamma)

    def calc_dist_map(self, seg):
        res = np.zeros_like(seg)
        posmask = seg.astype(np.bool)
        if posmask.any():
            negmask = ~posmask
            res = distance(negmask) * negmask - (distance(posmask) - 1) * posmask
        return res

    def calc_dist_map_batch(self, y_true):
        y_true_numpy = y_true.numpy()
        return np.array([self.calc_dist_map(y)
                        for y in y_true_numpy]).astype(np.float32)

    def surface_loss(self, y_true, y_pred):
        # reference:
        # https://github.com/LIVIAETS/boundary-loss
        y_true_dist_map = tf.py_function(func=self.calc_dist_map_batch,
                                        inp=[y_true],
                                        Tout=tf.float32)
        multipled = y_pred * y_true_dist_map
        return K.mean(multipled)

    def focal_loss_with_logits(self, logits, targets, alpha, gamma, y_pred):
        weight_a = alpha * (1 - y_pred) ** gamma * targets
        weight_b = (1 - alpha) * y_pred ** gamma * (1 - targets)
        return (tf.math.log1p(tf.exp(-tf.abs(logits))) + tf.nn.relu(-logits)) * (weight_a + weight_b) + logits * weight_b

    def focal_loss(self, y_true, y_pred):
        """
            ratio = (np.sum(y_true.numpy()==0)) / 131072
            gamma = 2 * (1 - ratio)
            alpha = 0.75 * ratio + 0.25
        """
        gamma, alpha = 2., 0.25
        y_pred = tf.clip_by_value(y_pred, tf.keras.backend.epsilon(), 1 - tf.keras.backend.epsilon())
        logits = tf.math.log(y_pred / (1 - y_pred))
        loss = self.focal_loss_with_logits(logits=logits, targets=y_true, alpha=alpha, gamma=gamma, y_pred=y_pred)
        return tf.reduce_mean(loss)

    def bce_dice_loss(self, y_true, y_pred):
        loss = keras.losses.binary_crossentropy(y_true, y_pred) + \
               self.dice_loss(y_true, y_pred)
        return loss / 2.0

    def log_cosh_dice_loss(self, y_true, y_pred):
        # reference:
        # https://github.com/shruti-jadon/Semantic-Segmentation-Loss-Functions
        x = self.dice_loss(y_true, y_pred)
        return tf.math.log((tf.exp(x) + tf.exp(-x)) / 2.0)

    def sensitivity(self, y_true, y_pred):
        true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
        possible_positives = K.sum(K.round(K.clip(y_true, 0, 1)))
        return true_positives / (possible_positives + K.epsilon())

    def specificity(self, y_true, y_pred):
        true_negatives = K.sum(
            K.round(K.clip((1 - y_true) * (1 - y_pred), 0, 1)))
        possible_negatives = K.sum(K.round(K.clip(1 - y_true, 0, 1)))
        return true_negatives / (possible_negatives + K.epsilon())

    def ssl(self, y_true, y_pred):
        w = 0.5
        return w * self.sensitivity + (1 - w) * self.specificity

    def convert_to_logits(self, y_pred):
        y_pred = tf.clip_by_value(y_pred, tf.keras.backend.epsilon(),1 - tf.keras.backend.epsilon())
        return tf.math.log(y_pred / (1 - y_pred))

    def weighted_cross_entropyloss(self, y_true, y_pred):
        beta = 0.25
        y_pred = self.convert_to_logits(y_pred)
        pos_weight = tf.constant(beta / (1 - beta))
        loss = tf.nn.weighted_cross_entropy_with_logits(logits=y_pred,targets=y_true,pos_weight=pos_weight)
        return tf.reduce_mean(loss)

    def balanced_binary_loss(self, y_true, y_pred): 
        # https://github.com/tensorflow/tensorflow/blob/v2.3.1/tensorflow/python/keras/backend.py#L4826
        beta = 1 - tf.reduce_mean(y_true / (512 * 512))
        y_pred = K.clip(y_pred, K.epsilon(), 1 - K.epsilon())

        term_0 = (1 - y_true) * K.log(1 - y_pred + K.epsilon())  # Cancels out when target is 1 
        term_1 = y_true * K.log(y_pred + K.epsilon()) # Cancels out when target is 0

        return -K.mean(beta * term_0 + (1 - beta) * term_1, axis=1)

    def custom_loss(self, y_true, y_pred):
        beta = 0.25
        y_pred = self.convert_to_logits(y_pred)
        pos_weight = tf.constant(beta / (1 - beta))
        loss = tf.nn.weighted_cross_entropy_with_logits(logits=y_pred,targets=y_true,pos_weight=pos_weight)

        res = distance(y_true)
        res = 1 - res
        res = np.multiply(res, loss)
        return tf.reduce_mean(res)

"""
    # https://arxiv.org/pdf/1908.03679.pdf
    def custom_loss(self, y_true, y_test):
        edt_map = euclidean_dist_transform(y_true)
        fi = (1 + edt_map) / N
        # revert the map
"""