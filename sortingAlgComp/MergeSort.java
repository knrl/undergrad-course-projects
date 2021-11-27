package com.mycompany.odev6;

/**
 *
 * @author mehmetkaanerol
 */
public class MergeSort {
    MergeSort() { }

    /**
     *
     * @param arr dizi alınır
     * @param l dizinin sol indisi
     * @param r dizinin sağ indisi
     */
    public void mergeSort(int arr[], int l, int r)
    {
        if (l < r) {
            int m = l + (r - l) / 2;

            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);
 
            int sira1 = m - l + 1;
            int sira2 = r - m;

            int lArr[] = new int[sira1];
            int rArr[] = new int[sira2];

            for (int i = 0; i < sira1; ++i)
                lArr[i] = arr[l + i];
            for (int j = 0; j < sira2; ++j)
                rArr[j] = arr[m + 1 + j];

            int i = 0, j = 0, k = l;
            while (i < sira1 && j < sira2) {
                if (lArr[i] <= rArr[j]) {
                    arr[k] = lArr[i];
                    i++;
                }
                else {
                    arr[k] = rArr[j];
                    j++;
                }
                k++;
            }

            while (i < sira1) {
                arr[k] = lArr[i];
                i++;
                k++;
            }

            while (j < sira2) {
                arr[k] = rArr[j];
                j++;
                k++;
            }
        }
    }
    
    /**
     *
     * @param arr LinkedList alınır
     * @param l dizinin sol indisi
     * @param r dizinin sağ indisi
     */
    public void mergeSort(LinkedList arr, int l, int r)
    {
        if (l < r) {
            int m = l + (r - l) / 2;

            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);
 
            int sira1 = m - l + 1;
            int sira2 = r - m;

            int lArr[] = new int[sira1];
            int rArr[] = new int[sira2];

            for (int i = 0; i < sira1; ++i)
                lArr[i] = arr.get(l + i);
            for (int j = 0; j < sira2; ++j)
                rArr[j] = arr.get(m + 1 + j);

            int i = 0, j = 0, k = l;
            while (i < sira1 && j < sira2) {
                if (lArr[i] <= rArr[j]) {
                    arr.set(lArr[i], k);
                    i++;
                }
                else {
                    arr.set(rArr[j], k);
                    j++;
                }
                k++;
            }

            while (i < sira1) {
                arr.set(lArr[i], k);
                i++;
                k++;
            }

            while (j < sira2) {
                arr.set(rArr[j], k);
                j++;
                k++;
            }
        }
    }

    /**
     *
     * @param arr dizi alınır
     * @return dizi döndürülür
     */
    public int[] sort(int []arr){
        mergeSort(arr, 0, getLength(arr) - 1);
        return arr;
    }
    
    /**
     *
     * @param arr LinkedList alınır
     * @return LinkedList döndürülür
     */
    public LinkedList sort(LinkedList arr){
        mergeSort(arr, 0, arr.size() - 1);
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
        mergeSort(tempArr, 0, n - 1);
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
