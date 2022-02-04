#
#   @author: mehmetkaanerol
#
import numpy as np

class NeuralNetwork:
    def __init__(self, initializer=None, learning_rate=0.001):
        self.intializer = initializer
        self.input_shape= None
        self.l_r        = learning_rate
        self.layers     = [] 
        self.weights    = [] 
        self.dWeights   = [] 
        self.output     = None

    def sequential_model(self, input_shape, hidLayerUnitDist=None):
        self.input_shape = input_shape
        if (hidLayerUnitDist != None):
            hidden_counter = 1
            self.weights.append(self.dense_layer(input_shape[1], units=hidLayerUnitDist[0], kernel_initializer=self.intializer))
            self.layers.append(np.ones((input_shape[0],self.weights[0].shape[1])))
            if (len(hidLayerUnitDist) > 1):
                for hidden_units in hidLayerUnitDist[1:]:
                    self.weights.append(self.dense_layer(self.weights[hidden_counter-1].shape[1], units=hidden_units, kernel_initializer=self.intializer))
                    self.layers.append(np.ones((self.layers[hidden_counter-1].shape[0], self.weights[hidden_counter].shape[1])))
                    hidden_counter += 1
            self.weights.append(self.dense_layer(self.weights[hidden_counter-1].shape[1], units=1, kernel_initializer=self.intializer))
            self.output = np.ones((self.layers[hidden_counter-1].shape[0], self.weights[hidden_counter].shape[1]))

    def dense_layer(self, X, units=None, activation=None, kernel_initializer=None, use_bias=True):
        if (kernel_initializer == None):
            layer = np.ones((X, units))
        elif (kernel_initializer == 'randomNormal'):
            mu, sigma = 0, 0.1
            layer = np.random.normal(mu,sigma,(X, units))
        elif (kernel_initializer == 'randomUniform'):
            low, high = 0.0, 1.0
            layer = np.random.uniform(low,high,(X, units))
        return layer

    def feedforward(self, X):
        self.layers[0] = self.sigmoid(np.dot(X, self.weights[0]))
        for index in range(0,len(self.layers)-1):
            self.layers[index+1] = self.sigmoid(np.dot(self.layers[index], self.weights[index+1]))
        self.output = self.sigmoid(np.dot(self.layers[index+1], self.weights[index+2]))

    def backpropagation(self, X, Y):
        # 
        N, M = 123, 123
        
        loss = 2*(Y - self.output)
        A = loss * self.sigmoid_derivative(self.output)
        self.dWeights[N] = np.dot(self.layers[M].T, A)
        for i in range(N-1,0,-1):
            A = np.dot(A, self.dWeights[i+1].T)*self.sigmoid_derivative(self.layers[M])
            self.dWeights[i] = np.dot(self.layers[M-1].T, A)
            M -= 1

        A = np.dot(A, self.dWeights[i].T)*self.sigmoid_derivative(self.layers[M])
        self.dWeights[i] = np.dot(X.T, A)

        # update weights
        for i in range(len(self.weights)):
            self.weights[i] += self.l_r * self.dWeights[i]

    def train_one_batch(self, X, Y):
        self.feedforward(X)
        self.backpropagation(X, Y)

    def sigmoid(self, t):
        return 1/(1+np.exp(-t))

    def sigmoid_derivative(self, p):
        return p * (1 - p)

    def summary(self):
        counter = 0
        print("\nModel Summary\nInput:\t\t",self.input_shape)
        print("Weight [",counter,"]:\t",self.weights[counter].shape)
        for (layer,weight) in zip(self.layers,self.weights[1:]):
            print("Layer  [",counter,"]:\t",layer.shape,end="\n")
            print("Weight [",(counter+1),"]:\t",weight.shape,end="\n")
            counter += 1
        print("Output:\t\t",self.output.shape,end="\n")

if (__name__ == '__main__'):
    NN = NeuralNetwork(initializer='randomNormal', 
                       learning_rate = 0.001)
    X      = np.ones((5,6))
    Y      = np.ones((5,1))

    NN.sequential_model(X.shape, hidLayerUnitDist = [2,3,4])
    NN.train_one_batch(X, Y)
    NN.summary()


#    NN.train_one_batch(X, Y)


""""
class NeuralNetwork:
    def __init__(self, x,y):
        self.input = x
        self.weights1 = np.random.rand(self.input.shape[1],2)
        self.weights2 = np.random.rand(2,1)
        self.y = y
        self.output = np. zeros(y.shape)

    def feedforward(self):
        self.layer1 = self.sigmoid(np.dot(self.input, self.weights1))
        self.layer2 = self.sigmoid(np.dot(self.layer1, self.weights2))
        return self.layer2

    def backprop(self):
        d_weights2 = np.dot(self.layer1.T, 2*(self.y -self.output)*self.sigmoid_derivative(self.output))
        d_weights1 = np.dot(self.input.T, np.dot(2*(self.y -self.output)*self.sigmoid_derivative(self.output), self.weights2.T)*self.sigmoid_derivative(self.layer1))
    
        self.weights1 += d_weights1
        self.weights2 += d_weights2

    def train(self, X, y):
        self.output = self.feedforward()
        self.backprop()

    def sigmoid(t):
        return 1/(1+np.exp(-t))

    def sigmoid_derivative(p):
        return p * (1 - p)


"""



"""        self.layers.append(self.dense_layer(input, units=2, kernel_initializer=self.intializer))
        self.layers.append(self.dense_layer(np.dot(input,self.layers[0]), units=3, kernel_initializer=self.intializer))
        self.layers.append(self.dense_layer(np.dot(self.layers[0],self.layers[1]), units=4, kernel_initializer=self.intializer))
        self.output      = self.dense_layer(np.dot(self.layers[1],self.layers[2]), units=5, kernel_initializer=self.intializer).T
"""

"""

    def sequential_model(self, input, hidLayerUnitDist=None):
        self.input     = input
        if (hidLayerUnitDist != None):
            hidden_counter = 1
            self.weights.append(self.dense_layer(self.input, units=hidLayerUnitDist[0], kernel_initializer=self.intializer))    # input weights
            self.layers.append(self.sigmoid(np.dot(self.input, self.weights[0])))
            if (len(hidLayerUnitDist) > 1):                                                                                     # and other layers weights
                for hidden_units in hidLayerUnitDist[1:]:
                    self.weights.append(self.dense_layer(self.weights[hidden_counter-1], units=hidden_units, kernel_initializer=self.intializer))
                    self.layers.append(self.sigmoid(np.dot(self.layers[hidden_counter-1], self.weights[hidden_counter])))
                    hidden_counter += 1
            self.weights.append(self.dense_layer(self.weights[hidden_counter-1], units=1, kernel_initializer=self.intializer))
            self.output = self.sigmoid(np.dot(self.layers[hidden_counter-1], self.weights[hidden_counter]))


"""