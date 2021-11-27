package com.mycompany.dnn;

/**
 *
 * @author mehmetkaanerol
 */
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author mehmetkaanerol
 */
public class NeuralNetwork {
    Matrix weights_ih, weights_ho, bias_h, bias_o;
    double l_rate = 0.01;

    NeuralNetwork() { }

    NeuralNetwork(int i, int h, int o) {
        weights_ih = new Matrix(h,i);
        weights_ho = new Matrix(o,h);

        bias_h= new Matrix(h,1);
        bias_o= new Matrix(o,1);
    }

    /**
     *
     * @param X matris alır, girdi matrisini temsil eder
     * @param weights matris alır, ağırlıkları temsil eder
     * @param bias matris alır, yanlılık matrisini temsil eder
     * @return katmana ait işlemlerden geçirilmiş matris çıktısını yine matris olarak döndürür
     */
    public Matrix layer(Matrix X, Matrix weights, Matrix bias){
        Matrix hidden = Matrix.multiply(weights, X);
        hidden.add(bias);
        hidden.sigmoid();
        return hidden;
    }

    /**
     *
     * @param X matris alır, girdi matrisini alır
     * @return eğitilmiş modelden geçirilen girdi matrsinin tahmin sonucunu döndürür
     */
    public List<Double> predict(double[] X){
        Matrix input = Matrix.fromArray(X);
        Matrix hidden = Matrix.multiply(weights_ih, input);
        hidden.add(bias_h);
        hidden.sigmoid();

        Matrix output = Matrix.multiply(weights_ho,hidden);
        output.add(bias_o);
        output.sigmoid();

        return output.toArray();
    }

    /**
     *
     * @param X matris alır, eğitim verisini temsil eder
     * @param Y matris alır, etiket verisini temsil eder
     * @param epochs eğitim sürecinde tüm verinin modelden toplam kaç defa geçirileceğini temsil eder
     */
    public void fit(double[][] X, double[][] Y, int epochs){
        double error = 0.;
        for(int i=0;i < epochs;i++){
            int sampleN = (int) (Math.random() * X.length );
            error += this.train(X[sampleN], Y[sampleN]);
            if (i % 500 == 0){
                System.out.println("Epochs " + i + " : Loss: " + (error/100) + " ");
                error = 0.;
            }
        }
        System.out.println("");
    }

    /**
     *
     * @param X matris alır
     * @param Y matris alır
     * @return hata değerini döndürür
     */
    public double train(double [] X, double [] Y){
        Matrix input = Matrix.fromArray(X);
        Matrix hidden = Matrix.multiply(weights_ih, input);
        hidden.add(bias_h);
        hidden.sigmoid();

        Matrix output = Matrix.multiply(weights_ho, hidden);
        output.add(bias_o);
        output.sigmoid();
        Matrix target = Matrix.fromArray(Y);

        Matrix error = Matrix.subtract(target, output);
        Matrix gradient = output.dsigmoid();
        gradient.multiply(error);
        gradient.multiply(l_rate);

        Matrix hidden_T = Matrix.transpose(hidden);
        Matrix who_delta =  Matrix.multiply(gradient, hidden_T);

        weights_ho.add(who_delta);
        bias_o.add(gradient);

        Matrix who_T = Matrix.transpose(weights_ho);
        Matrix hidden_errors = Matrix.multiply(who_T, error);

        Matrix h_gradient = hidden.dsigmoid();
        h_gradient.multiply(hidden_errors);
        h_gradient.multiply(l_rate);

        Matrix i_T = Matrix.transpose(input);
        Matrix wih_delta = Matrix.multiply(h_gradient, i_T);

        weights_ih.add(wih_delta);
        bias_h.add(h_gradient);

        return error.sum();
    }

}
