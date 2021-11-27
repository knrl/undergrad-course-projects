package com.mycompany.odev6;

/**
 *
 * @author mehmetkaanerol
 */
public class SelectionSort {
    SelectionSort() { }
    
    /**
     *
     * @param arr dizi nesnesi alınır
     * @return dizi nesnesi döndürülür
     */
    public int [] sort(int [] arr){
        int minIndex=0, temp=0, sizeofArr=arr.length;
        for (int i=0;i<sizeofArr;i++){
            minIndex = i;
            for (int k=i+1;k<sizeofArr;k++)
                if (arr[minIndex] > arr[k]) minIndex = k;
            if (minIndex != i){
                temp = arr[minIndex];
                arr[minIndex] = arr[i];
                arr[i] = temp;
            }
        }
        return arr;
    }

    /**
     *
     * @param arr LinkedList nesnesi alınır
     * @return LinkedList nesnesi döndürülür
     */
    public LinkedList sort(LinkedList arr){
        int minIndex=0, temp=0, sizeofArr=arr.size();
        for (int i=0;i<sizeofArr;i++){
            minIndex = i;
            for (int k=i+1;k<sizeofArr;k++)
                if (arr.get(minIndex) > arr.get(k)) minIndex = k;
            if (minIndex != i){
                temp = arr.get(minIndex);
                arr.set(arr.get(i), minIndex);
                arr.set(temp, i);
            }
        }
        return arr;
    }

}
