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
public class Matrix {
    double [][] data;
    int rows, cols;

    /**
     *
     * @param rows satir sayısı
     * @param cols sütun sayısı
     */
    public Matrix(int rows, int cols){
        data = new double[rows][cols];
        this.rows = rows;
        this.cols = cols;
        for (int i=0;i<rows;i++){
            for (int j=0;j<cols;j++)
                data[i][j] = Math.random() * 2 - 1;
        }
    }

    /**
     * sınıfa ait global data[][] dizisini yazdir
     */
    public void print(){
        for (int i=0;i<rows;i++){
            for (int j=0;j<cols;j++)
                System.out.print(this.data[i][j]+" ");
            System.out.println();
        }
    }
    
    /**
     *
     * @param scaler data[][] dizisine ait tüm elemanların toplam değerini yazdır
     */
    public void add(int scaler){
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++)
                this.data[i][j] += scaler;
        }
    }

    /**
     *
     * @param m matris sınıfına ait obje alır
     */
    public void add(Matrix m){
        if(cols != m.cols || rows != m.rows) {
            System.out.println("Shape Mismatch");
            return;
        }

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++)
                this.data[i][j] += m.data[i][j];
        }
    }

    /**
     *
     * @param x tek boyutlu dizi alır
     * @return tek boyutlu diziyi matris objesine çevirir
     */
    public static Matrix fromArray(double[]x){
        Matrix temp = new Matrix(x.length,1);
        for(int i =0;i<x.length;i++)
            temp.data[i][0] = x[i];
        return temp;
    }

    /**
     *
     * @return data[][] dizisini tek boyutlu diziye çevirir, döndürür
     */
    public List<Double> toArray() {
        List<Double> temp = new ArrayList<Double>();

        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++)
                temp.add(data[i][j]);
        }
        return temp;
    }

    /**
     *
     * @param a matris alır
     * @param b matris alır
     * @return verilen iki matrisin her eleman için farklarının matrisini döndürür
     */
    public static Matrix subtract(Matrix a, Matrix b) {
        Matrix temp=new Matrix(a.rows,a.cols);
        for(int i=0;i<a.rows;i++)
        {
            for(int j=0;j<a.cols;j++)
                temp.data[i][j] = a.data[i][j]-b.data[i][j];
        }
        return temp;
    }
    
    /**
     *
     * @param a matris alır
     * @return verilen metrisin transpoz matrisini döndürür
     */
    public static Matrix transpose(Matrix a) {
        Matrix temp = new Matrix(a.cols, a.rows);
        for (int i = 0; i < a.rows; i++) {
            for (int j = 0; j < a.cols; j++) {
                temp.data[j][i] = a.data[i][j];
            }
        }
        return temp;
    }

    /**
     *
     * @return data dizisinin toplamını döndürür
     */
    public double sum(){
        double total = 0;
        for(int i=0;i<rows;i++)
            for(int j=0;j<cols;j++)
                total += data[i][j];

        return total;
    }
    
    /**
     *
     * @param a matris alır
     * @param b matris alır
     * @return verilen iki matrisin çarpımlarından oluşan matrisi döndürür
     */
    public static Matrix multiply(Matrix a, Matrix b) {
        Matrix temp = new Matrix(a.rows,b.cols);
        for(int i=0;i<temp.rows;i++){
            for(int j=0;j<temp.cols;j++){
                double sum = 0;
                for(int k=0;k<a.cols;k++)
                    sum += a.data[i][k] * b.data[k][j];
                temp.data[i][j]=sum;
            }
        }
        return temp;
    }

    /**
     *
     * @param a matris alır
     */
    public void multiply(Matrix a) {
        for(int i=0;i<a.rows;i++){
            for(int j=0;j<a.cols;j++)
                this.data[i][j]*=a.data[i][j];
        }
    }

    /**
     *
     * @param a matris alır
     */
    public void multiply(double a) {
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++)
                this.data[i][j] *= a;
        }
    }
    
    /**
     * data dizisinnin sigmoid fonksiyonundan geçirilmiş hali
     */
    public void sigmoid() {
        for(int i=0;i<rows;i++)
            for(int j=0;j<cols;j++)
                this.data[i][j] = 1/(1+Math.exp(-this.data[i][j])); 
    }

    /**
     *
     * @return data dizisinnin sigmoid fonksiyonundan geçirilmiş halini döndürür
     */
    public Matrix dsigmoid() {
        Matrix temp=new Matrix(rows,cols);
        for(int i=0;i<rows;i++)
            for(int j=0;j<cols;j++)
                temp.data[i][j] = this.data[i][j] * (1-this.data[i][j]);
        return temp;
    }
    
}
