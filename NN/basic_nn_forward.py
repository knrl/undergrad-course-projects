#
#   @author: mehmetkaanerol
#
import numpy as np

class Perceptron(object):
    def __init__(self):
        self.w1 = np.array([1.0, -1.0], dtype=np.float32)
        self.w2 = np.array([1.0], dtype=np.float32)

    def act_f(self, X, th=0):
        return np.where(X > th, 1, 0)

    def predict(self, x):
        a11     = x[0] * self.w1[0]
        a12     = x[1] * self.w1[0]
        h11     = self.act_f((a11 + a12), th=0.5)
        
        b11     = x[0] * self.w1[1]
        b12     = x[1] * self.w1[1]
        h12     = self.act_f((b11 + b12), th=-1.5)
        
        a21     = h11 * self.w2
        b21     = h12 * self.w2 
        h21     = self.act_f((a21 + b21), th=1.0)
        return h21

if (__name__ == '__main__'):
    xor_ppn     = Perceptron()

    X           = np.array([[1.0, 0.0], 
                            [0.0, 1.0], 
                            [1.0, 1.0], 
                            [0.0, 0.0]], dtype=np.float32)

    Y           = np.array([[1], [1], [0], [0]], dtype=np.uint8)

    for x, y in zip(X, Y):
        print('Prediction: ',xor_ppn.predict(x), ' Ground Truth: ', y, end='\n')
