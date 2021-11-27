package com.mycompany.odev6;

/**
 *
 * @author mehmetkaanerol
 */
public class QuickSort {
    QuickSort() { }
    
    /**
     *
     * @param arr dizi alınır
     * @param l dizinin sol indisi
     * @param h dizinin sağ indisi
     */
    public void quickSort(int [] arr, int l, int h){
        if (l < h){
            int j = l-1, pivot = arr[h], temp = 0;
            for (int i=l;i<h;i++){
                if (arr[i] < pivot){
                    j++;
                    temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }

            temp = arr[j+1];
            arr[j+1] = arr[h];
            arr[h] = temp;

            quickSort(arr, l, j);
            quickSort(arr, j+2, h);
        }
    }

    /**
     *
     * @param arr LinkedList alınır
     * @param l LinkedListin sol indisi
     * @param h LinkedListin sağ indisi
     */
    public void quickSort(LinkedList arr, int l, int h){
        if (l < h){
            int j = l-1, pivot = arr.get(h), temp = 0;
            for (int i=l;i<h;i++){
                if (arr.get(i) < pivot){
                    j++;
                    temp = arr.get(i);
                    arr.set(arr.get(j), i);
                    arr.set(temp, j);
                }
            }

            temp = arr.get(j+1);
            arr.set(arr.get(h), j+1);
            arr.set(temp, h);

            quickSort(arr, l, j);
            quickSort(arr, j+2, h);
        }
    }

    /**
     *
     * @param arr LinkedList alınır
     * @return LinkedList döndürülür
     */
    public LinkedList sort(LinkedList arr){
        quickSort(arr, 0, arr.size() - 1);
        return arr;
    }

    /**
     *
     * @param arr dizi alınır
     * @return dizi döndürülür
     */
    public int[] sort(int []arr){
        quickSort(arr, 0, getLength(arr) - 1);
        return arr;
    }

    /**
     *
     * @param arr dizi alınır
     * @return dizi döndürülür
     */
    public int[] sort(char []arr){
        int n = getLength(arr);
        int tempArr[] = new int[n];
        for (int i=0;i<n;i++)
            tempArr[i] = (int) arr[i];
        quickSort(tempArr, 0, n - 1);
        return tempArr;
    }

    /**
     *
     * @param arr dizi alınır
     * @return dizi boyutu döndürülür
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
     * @return dizi boyutu döndürülür
     */
    public int getLength(char arr[]){
        int counter = 0;
        for (char a: arr)
            counter++;
        return counter;
    }

}
