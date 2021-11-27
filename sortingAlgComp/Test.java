package com.mycompany.odev6;

/**
 *
 * @author mehmetkaanerol
 */
import java.util.Random;

/**
 *
 * @author mehmetkaanerol
 */
public class Test {
    public static void main(String[] args) {
        // SORU 1 //
        soru1TestCalistir("heap");
        soru1TestCalistir("merge");
        soru1TestCalistir("quick");

        // SORU 2 //
        // Linked list ile Quick ve merge sort implementasyonu
        soru2LinkedListTestCalistir();

        // Dizi yapısı üzerinde quick, merge, insertion, bubble, selection sort
        // sıralama algoritmalarının karşılaştırılması
        System.out.println("10K Test: ");
        soru2KarsilastirmaTestCalistir(10000);

        System.out.println("40K Test: ");
        soru2KarsilastirmaTestCalistir(40000);

        System.out.println("100K Test: ");
        soru2KarsilastirmaTestCalistir(100000);

        System.out.println("150K Test: ");
        soru2KarsilastirmaTestCalistir(150000);

        System.out.println("180K Test: ");
        soru2KarsilastirmaTestCalistir(180000);
    }

    /**
     *
     * @param isim sıralama algoritmasının adı
     */
    static public void soru1TestCalistir(String isim){
        int A[] = {0,1,2,3,4,5,6,7,8,9};
        int B[] = {9,8,7,6,5,4,3,2,1,0};
        int C[] = {5,2,13,9,1,7,6,8,1,15,4,11};
        char D[] = {'S','B','I','M','H','Q','C','L','R','E','P','K'};

        // Sıralama algoritmasını seç
        System.out.println("\n" + isim + ": \nA:");
        if (isim.equals("heap")){
            HeapSort heap = new HeapSort();
            printArr(heap.sort(A), 0);
            System.out.println("B:");
            printArr(heap.sort(B), 0);
            System.out.println("C:");
            printArr(heap.sort(C), 0);
            System.out.println("D:");
            printArr(heap.sort(D), 1);
        }
        else if (isim.equals("merge")){
            MergeSort merge = new MergeSort();
            printArr(merge.sort(A), 0);
            System.out.println("B:");
            printArr(merge.sort(B), 0);
            System.out.println("C:");
            printArr(merge.sort(C), 0);
            System.out.println("D:");
            printArr(merge.sort(D), 1);   
        }
        else if (isim.equals("quick")){
            QuickSort quick = new QuickSort();
            printArr(quick.sort(A), 0);
            System.out.println("B:");
            printArr(quick.sort(B), 0);
            System.out.println("C:");
            printArr(quick.sort(C), 0);
            System.out.println("D:");
            printArr(quick.sort(D), 1);   
        }
    }

    /**
     *
     */
    static public void soru2LinkedListTestCalistir(){
        // Rastgele sayılardan linkes list oluştur
        LinkedList liste1 = new LinkedList();
        LinkedList liste2 = new LinkedList();
        Random rand = new Random();
        for (int i=0;i<20;i++)
            liste1.add(rand.nextInt(1000));
        for (int i=0;i<20;i++)
            liste2.add(liste1.get(i));
        
        QuickSort quick = new QuickSort();
        printArr(quick.sort(liste1), 0);
        

        MergeSort merge = new MergeSort();
        printArr(merge.sort(liste2), 0);
        
        long time_1 = System.currentTimeMillis();
        long time_2 = System.currentTimeMillis();
        long difference = time_2 - time_1;
        System.out.println(difference + "milliseconds" );
    }

    /**
     *
     * @param elemanSayisi test için eleman sayisi
     */
    static public void soru2KarsilastirmaTestCalistir(int elemanSayisi){
        long time1, time2;

        // 20 rastgele linked list
        int listeler[][] = new int[20][elemanSayisi];

        // 20 tane elemanSayisi elemanlı rastgele liste
        Random rand = new Random();
        for (int i=0;i<20;i++){
            for (int j=0;j<elemanSayisi;j++)
                listeler[i][j] = rand.nextInt(100);
        }

        // Sirali liste
        int siraliListe[] = new int[elemanSayisi];
        for (int i=0;i<elemanSayisi;i++)
            siraliListe[i] = i;

        int temp[] = new int[elemanSayisi];

        // Merge Sort Test
        MergeSort merge = new MergeSort();
        System.out.println("Merge Sort:");
        for (int i=0;i<20;i++){
            for (int j=0;j<elemanSayisi;j++)
                temp[j] = listeler[i][j];
            time1 = System.currentTimeMillis();
            merge.sort(temp);
            time2 = System.currentTimeMillis();
            System.out.println((i+1) + ". rastgele dizi " + (time2 - time1) + "milliseconds");
        }
        for (int j=0;j<elemanSayisi;j++)
                temp[j] = siraliListe[j];
        time1 = System.currentTimeMillis();
        merge.sort(temp);
        time2 = System.currentTimeMillis();
        System.out.println("sirali dizi " + (time2 - time1) + "milliseconds");

        // Quick Sort Test
        QuickSort quick = new QuickSort();
        System.out.println("Quick Sort:");
        for (int i=0;i<20;i++){
            for (int j=0;j<elemanSayisi;j++)
                temp[j] = listeler[i][j];
            time1 = System.currentTimeMillis();
            quick.sort(temp);
            time2 = System.currentTimeMillis();
            System.out.println((i+1) + ". rastgele dizi " + (time2 - time1) + "milliseconds");
        }
        for (int j=0;j<elemanSayisi;j++)
                temp[j] = siraliListe[j];
        time1 = System.currentTimeMillis();
        //quick.sort(temp);
        time2 = System.currentTimeMillis();
        System.out.println("sirali dizi " + (time2 - time1) + "milliseconds");

        // Heap Sort Test
        System.out.println("Heap Sort:");
        HeapSort heap = new HeapSort();
        for (int i=0;i<20;i++){
            for (int j=0;j<elemanSayisi;j++)
                temp[j] = listeler[i][j];
            time1 = System.currentTimeMillis();
            heap.sort(temp);
            time2 = System.currentTimeMillis();
            System.out.println((i+1) + ". rastgele dizi " + (time2 - time1) + "milliseconds");
        }
        for (int j=0;j<elemanSayisi;j++)
                temp[j] = siraliListe[j];
        time1 = System.currentTimeMillis();
        heap.sort(temp);
        time2 = System.currentTimeMillis();
        System.out.println("sirali dizi " + (time2 - time1) + "milliseconds");

        // Insertion Sort Test
        System.out.println("Insertion Sort:");
        InsertionSort insertion = new InsertionSort();
        for (int i=0;i<20;i++){
            for (int j=0;j<elemanSayisi;j++)
                temp[j] = listeler[i][j];
            time1 = System.currentTimeMillis();
            insertion.sort(temp);
            time2 = System.currentTimeMillis();
            System.out.println((i+1) + ". rastgele dizi " + (time2 - time1) + "milliseconds");
        }
        for (int j=0;j<elemanSayisi;j++)
                temp[j] = siraliListe[j];
        time1 = System.currentTimeMillis();
        insertion.sort(temp);
        time2 = System.currentTimeMillis();
        System.out.println("sirali dizi " + (time2 - time1) + "milliseconds");

        // Bubble Sort Test
        System.out.println("Bubble Sort:");
        BubbleSort bubble = new BubbleSort();
        for (int i=0;i<20;i++){
            for (int j=0;j<elemanSayisi;j++)
                temp[j] = listeler[i][j];
            time1 = System.currentTimeMillis();
            bubble.sort(temp);
            time2 = System.currentTimeMillis();
            System.out.println((i+1) + ". rastgele dizi " + (time2 - time1) + "milliseconds");
        }
        for (int j=0;j<elemanSayisi;j++)
                temp[j] = siraliListe[j];
        time1 = System.currentTimeMillis();
        bubble.sort(temp);
        time2 = System.currentTimeMillis();
        System.out.println("sirali dizi " + (time2 - time1) + "milliseconds");
        
        // Selection Sort Test
        System.out.println("Selection Sort:");
        SelectionSort selection = new SelectionSort();
        for (int i=0;i<20;i++){
            for (int j=0;j<elemanSayisi;j++)
                temp[j] = listeler[i][j];
            time1 = System.currentTimeMillis();
            selection.sort(temp);
            time2 = System.currentTimeMillis();
            System.out.println((i+1) + ". rastgele dizi " + (time2 - time1) + "milliseconds");
        }
        for (int j=0;j<elemanSayisi;j++)
                temp[j] = siraliListe[j];
        time1 = System.currentTimeMillis();
        selection.sort(temp);
        time2 = System.currentTimeMillis();
        System.out.println("sirali dizi " + (time2 - time1) + "milliseconds");

        listeler = null;
        siraliListe = null;
        temp = null;
    }

    /**
     *
     * @param arr dizi alınır
     * @param secim yazdırılacak dizinin tipi seçilir
     */
    static public void printArr(int []arr, int secim){
        if (secim == 0)
            for (int a : arr)
                System.out.print(a + " ");
        else
            for (int a : arr)
                System.out.print((char) a + " ");
        
        System.out.println("");
    }

    /**
     *
     * @param liste LinkedList alınır
     * @param secim yazdırılacak LinkedList nin tipi seçilir
     */
    static public void printArr(LinkedList liste, int secim){
        if (secim == 0)
            for (int i=0;i<liste.size();i++)
                System.out.print(liste.get(i) + " ");
        else
            for (int i=0;i<liste.size();i++)
                System.out.print((char) liste.get(i) + " ");
        
        System.out.println("");
    }

}
