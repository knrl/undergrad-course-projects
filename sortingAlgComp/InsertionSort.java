package com.mycompany.odev6;

/**
 *
 * @author mehmetkaanerol
 */
public class InsertionSort {
    InsertionSort() { }
    
    /**
     *
     * @param arr dizi nesnesi alınır
     * @return dizi nesnesi döndürülür
     */
    public int [] sort(int [] arr){
        int temp, sizeofArr=arr.length;
        for (int i=1;i<sizeofArr;i++){
            for (int k=i;k>0 && arr[k-1] >= arr[k];k--) {
                temp = arr[k];
                arr[k] = arr[k-1];
                arr[k-1] = temp; 
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
        int temp, sizeofArr=arr.size();
        for (int i=1;i<sizeofArr;i++){
            for (int k=i;k>0 && arr.get(k-1) >= arr.get(k);k--) {
                temp = arr.get(k);
                arr.set(arr.get(k-1), k);
                arr.set(temp, k-1);
            }
        }
        return arr;
    }

}
