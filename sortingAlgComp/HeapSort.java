package com.mycompany.odev6;

/**
 *
 * @author mehmetkaanerol
 */
public class HeapSort {
    public int len;

    HeapSort() { }

    /**
     *
     * @param arr dizi nesnesi alınır
     * @return dizi nesnesi döndürülür
     */
    public int [] sort(int [] arr){
        len = getLength(arr);
        int n = len;

        for (int i=(n/2)-1;i>=0;i--) 
            heapify(arr, n, i);

        for (int i=n-1;i>0;i--){
            int temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;

            heapify(arr, i, 0);
        }

        return arr;
    }

    /**
     *
     * @param arr LinkedList nesnesi alınır
     * @return LinkedList nesnesi döndürülür
     */
    public LinkedList sort(LinkedList arr){
        len = arr.size();
        int n = len;

        for (int i=(n/2)-1;i>=0;i--) 
            heapify(arr, n, i);

        for (int i=n-1;i>0;i--){
            int temp = arr.get(0);
            arr.set(arr.get(i), 0);
            arr.set(temp, i);

            heapify(arr, i, 0);
        }

        return arr;
    }

    /**
     *
     * @param arr dizi nesnesi alınır
     * @return dizi nesnesi döndürülür
     */
    public int [] sort(char [] arr){
        len = getLength(arr);
        int n = len;
        int tempArr[] = new int[n];
        for (int i=0;i<n;i++)
            tempArr[i] = (int) arr[i];

        return sort(tempArr);
    }

    /**
     *
     * @param arr dizi alınır
     * @param n bitiş indisi
     * @param i en büyük değer alınır
     */
    public void heapify(int [] arr, int n, int i){
        int enBuyuk = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && arr[l] > arr[enBuyuk]) enBuyuk = l;
        if (r < n && arr[r] > arr[enBuyuk]) enBuyuk = r;

        if (i != enBuyuk){
            int temp = arr[i];
            arr[i] = arr[enBuyuk];
            arr[enBuyuk] = temp;

            heapify(arr, n, enBuyuk);
        }
    }
    
    /**
     *
     * @param arr LinkedList alınır
     * @param n bitiş indisi
     * @param i en büyük değer alınır
     */
    public void heapify(LinkedList arr, int n, int i){
        int enBuyuk = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && arr.get(l) > arr.get(enBuyuk)) enBuyuk = l;
        if (r < n && arr.get(r) > arr.get(enBuyuk)) enBuyuk = r;

        if (i != enBuyuk){
            int temp = arr.get(i);
            arr.set(arr.get(enBuyuk), i);
            arr.set(temp, enBuyuk);

            heapify(arr, n, enBuyuk);
        }
    }

    /**
     *
     * @param arr dizi alınır
     * @return dizi boyutu
     */
    public int getLength(int arr[]){
        int counter = 0;
        for (int a: arr)
            counter++;
        return counter;
    }
    
    /**
     *
     * @param arr dizi alınır
     * @return dizi boyutu
     */
    public int getLength(char arr[]){
        int counter = 0;
        for (char a: arr)
            counter++;
        return counter;
    }

}
