package com.mycompany.dnn;

/**
 *
 * @author mehmetkaanerol
 */
import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import java.lang.Math;

/**
 *
 * @author mehmetkaanerol
 */
public class Test {
    // Parametereler
    public static String train_path = "/Users/mehmetkaanerol/NetBeansProjects/DNN/train_data.txt";
    public static String test_path = "/Users/mehmetkaanerol/NetBeansProjects/DNN/test_data.txt";
    public static final int num_of_graphs = 9,
    num_of_test_graphs = 3,
    num_of_nodes = 5,
    num_of_classes = 3;

    public static int epochs = 50000;

    // Ground Truths
    static double [][] Y_train = {
        {1,0,0}, {0,1,0}, {0,0,1}, {1,0,0}, {0,1,0}, {0,0,1}, {1,0,0}, {0,1,0}, {0,0,1}
    };
    
    static double [][] Y_test = {
        {1,0,0}, {0,1,0}, {0,0,1}
    };

    /**
     *
     * @param args
     */
    public static void main(String[] args) {
        // egitim verisini al
        double [][]X_train = read_data_and_preprocess(train_path, true);

        // modeli verilere uydur
        NeuralNetwork nn = new NeuralNetwork(num_of_nodes*num_of_nodes, num_of_graphs, num_of_classes);
        // Eğtim ve etiket verilerini kullanarak veriyi modele uydur
        nn.fit(X_train, Y_train, epochs);

        // Test verisini al
        double [][]X_test = read_data_and_preprocess(test_path, false);

        // Test verisinin tahminlerini al
        List <Double> output;
        for (double d[]: X_test){
            // tahmin çıktısı
            output = nn.predict(d);
            // çıktıyı yazdır
            System.out.println(output.toString());
        }
    }

    /**
     *
     * @param path eğitim ya da test dosyasının yolu
     * @param train eğitim verisi ise true, test verisi ise false
     * @return X dizisi geri döndürülür
     */
    public static double[][] read_data_and_preprocess(String path, boolean train){
        // dosya yönetim objesi oluştur
        FileManagement fm = new FileManagement();
        // graf veri yapısını oku
        List<Graph> graphList = fm.read(path);

        // grafları normalize edilmiş komşuluk matrisine çevir, gönder
        if (train){ // Eğitim verisi
            double [][][] array = new double[num_of_graphs][num_of_nodes][num_of_nodes];
            // graf veri yapısının formatını değiştir
            for (int index=0;index<num_of_graphs;index++)
                array[index] = graphList.get(index).get_AdjMat_hat();

            double X[][] = new double[num_of_graphs][num_of_nodes*num_of_nodes];
            for (int index=0;index<num_of_graphs;index++)
                for (int i=0;i<num_of_nodes;i++)
                    for (int k=0;k<num_of_nodes;k++)
                        X[index][num_of_nodes * i + k] = array[index][i][k];   
            return X;
        }
        else{   // Test verisi
            double [][][] array = new double[num_of_test_graphs][num_of_nodes][num_of_nodes];
            // graf veri yapısının formatını değiştir
            for (int index=0;index<num_of_test_graphs;index++)
                array[index] = graphList.get(index).get_AdjMat_hat();

            double X[][] = new double[num_of_test_graphs][num_of_nodes*num_of_nodes];
            for (int index=0;index<num_of_test_graphs;index++)
                for (int i=0;i<num_of_nodes;i++)
                    for (int k=0;k<num_of_nodes;k++)
                        X[index][num_of_nodes * i + k] = array[index][i][k];  
            return X;
        }
    }

}
