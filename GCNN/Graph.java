package com.mycompany.dnn;

/**
 *
 * @author mehmetkaanerol
 */
import java.util.List;
import java.util.ArrayList;
import java.lang.Math;

/**
 *
 * @author mehmetkaanerol
 */
public class Graph {
    List <List <Node>> adjList = new ArrayList<>();
    int num_of_node = 5;
    double [][] A = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
    double [][] I = {{1,0,0,0,0},{0,1,0,0,0},{0,0,1,0,0},{0,0,0,1,0},{0,0,0,0,1}};
    double [][] Amod = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
    double [][] Dmod = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
    double [][] Dmod_inv = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

    Graph() { }

    /**
     *
     * @param edges edge'lerden oluşan bir listeden graf veri yapısına çevir
     */
    Graph(List<Edge> edges) {
        for (int i = 0; i < edges.size(); i++)
            adjList.add(i, new ArrayList<>());
 
        for (Edge e : edges){
            adjList.get(e.src).add(new Node(e.dest, e.value));
        }
        
        for(int i=0;i < adjList.size();i++) {
            for (Node edge : adjList.get(i)){
                A[i][edge.dest] = edge.value;
                A[edge.dest][i] = edge.value;
            }
        }
    }

    /**
     *
     * @return komşuluk matrisini döndür
     */
    public double [][] get_AdjMat(){
        return this.A;
    }

    /**
     *
     * @return Komşuluk matrisinin ön işlenmiş hali
     */
    public double [][] get_AdjMat_hat(){
        // Dmod hesaplanması, diagonal matris
        for (int i=0;i<num_of_node;i++){
            double total = 0;
            for (int k=0;k<num_of_node;k++)
                total += A[i][k];
            Dmod[i][i] = total;
        }

        // komşuluk matrisi ile birim matrisin toplanması
        for (int i=0;i<num_of_node;i++){
            for (int k=0;k<num_of_node;k++)
                Amod[i][k] = A[i][k] + I[i][k];
        }

        // Dmod matrisinin tersi alınmadan önce elemanların karekökü alınır
        for (int i=0;i<num_of_node;i++){
            for (int k=0;k<num_of_node;k++)
                Dmod_inv[i][k] = Math.sqrt(Dmod[i][k]);
        }
        // Karekökü alınmış Dmod matrisinin tersi alınır
        Dmod_inv = inverse(Dmod_inv);

        // Dmod_inv ile A matrisinin çarpımı
        double sum = 0;
        double temp[][] = new double[num_of_node][num_of_node];
        for(int i=0;i<num_of_node;i++){
            for(int j=0;j<num_of_node;j++){
                sum = 0;
                for(int k=0;k<num_of_node;k++)
                    sum += Dmod_inv[i][k] * Amod[k][j];
                temp[i][j]=sum;
            }
        }

        // Dmod_inv ile A matrisinin çarpımı
        for(int i=0;i<num_of_node;i++){
            for(int j=0;j<num_of_node;j++){
                sum = 0;
                for(int k=0;k<num_of_node;k++)
                    sum += Dmod_inv[i][k] * temp[k][j];
                Amod[i][j]=sum;
            }
        }
        // return d_mod_invroot @ a_mod @ d_mod_invroot
        return Amod;
    }

    /**
     *
     * @param A matris
     * @param p geçici satir sayısı
     * @param q geçici sütun sayısı
     * @param n eleman sayısı
     * @return verilen matrisin kofaktör matrisini döndürür
     */
    public double[][] getCofactor(double A[][], int p, int q, int n){
        int i = 0, j = 0;
        double temp[][] = new double[num_of_node][num_of_node];
        for (int row = 0; row < n; row++)
            for (int col = 0; col < n; col++)
                if (row != p && col != q){
                    temp[i][j++] = A[row][col];
                    if (j == n - 1){
                        j = 0;
                        i++;
                    }
                }
        return temp;
    }

    /**
     *
     * @param A matris
     * @param n eleman sayisi
     * @return verilen matrisin determinantını döndürür
     */
    public double determinant(double A[][], int n){
        if (n == 1) return A[0][0];

        double [][]temp = new double[num_of_node][num_of_node];
        double D = 0;
        int sign = 1;

        for (int f = 0; f < n; f++){
            temp = getCofactor(A, 0, f, n);
            D += sign * A[0][f] * determinant(temp, n - 1);
            sign = -sign;
        }
        return D;
    }

    /**
     *
     * @param A matris
     * @return verilen matrisin adjoint matrsini döndürür
     */
    public double[][] adjoint(double A[][]){
        int sign = 1;
        double [][]temp = new double[num_of_node][num_of_node];
        double [][]adj = new double[num_of_node][num_of_node];

        for (int i = 0; i < num_of_node; i++)
            for (int j = 0; j < num_of_node; j++)
            {
                temp = getCofactor(A, i, j, num_of_node);
                sign = ((i + j) % 2 == 0) ? 1: -1;
                adj[j][i] = (sign)*(determinant(temp, num_of_node-1));
            }
        return adj;
    }

    /**
     *
     * @param A matris
     * @return verilen metrisin tersinin döndürür
     */
    public double[][] inverse(double A[][]){
        double [][]inverse = new double[num_of_node][num_of_node];
        double det = determinant(A, num_of_node);

        if (det == 0){
            System.out.print("Singular matrisin teri yoktur.");
            return inverse;
        }

        double [][] adj = new double[num_of_node][num_of_node];
        adj = adjoint(A);

        for (int i = 0; i < num_of_node; i++)
            for (int j = 0; j < num_of_node; j++)
                inverse[i][j] = adj[i][j] / det;

        return inverse;
    }

    /**
     *
     * @param graph graf veri yapısının içeriğini yazdırır
     */
    public static void print(Graph graph) {
        int src_vertex = 0;
        int list_size = graph.adjList.size();
 
        while (src_vertex < list_size) {
            for (Node edge : graph.adjList.get(src_vertex))
                System.out.print("Vertex:" + src_vertex + " -> " + edge.dest + " vertex degeri: " + edge.value + "\t");
            
            System.out.println();
            src_vertex++;
        }
    }
}

class Edge {
    int src, dest, value;

    /**
     *
     * @param src kaynak
     * @param dest hedef
     * @param value kenara ait değer
     */
    Edge(int src, int dest, int value) {
        this.src = src;
        this.dest = dest;
        this.value = value;
    }
}

class Node {
    int value, dest;

    /**
     *
     * @param dest hedef düğüm
     * @param value düğüme ait değer
     */
    Node(int dest, int value)  {
        this.dest = dest;
        this.value = value;
    }
}
