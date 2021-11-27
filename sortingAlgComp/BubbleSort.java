package com.mycompany.odev6;

/**
 *
 * @author mehmetkaanerol
 */
public class BubbleSort {
    BubbleSort() { }
    
    /**
     *
     * @param arr dizi nesnesi alınır
     * @return dizi nesnesi döndürülür
     */
    public int [] sort(int [] arr){
        int temp, check, sizeofArr=arr.length;
        for (int i=0;i<sizeofArr;i++ ){
            check=1;
            for (int k=0;k<sizeofArr-i-1;k++ ){
                if (arr[k] > arr[k+1]){
                    temp = arr[k];
                    arr[k] = arr[k+1];
                    arr[k+1] = temp;
                    check = 0;
                }
            }
//            if (check == 1) break;
        }
        return arr;
    }

    /**
     *
     * @param arr LinkedList nesnesi alınır
     * @return LinkedList nesnesi döndürülür
     */
    public LinkedList sort(LinkedList arr){
        int temp, check, sizeofArr = arr.size();
        for (int i=0;i<sizeofArr;i++){
            check=1;
            for (int k=0;k<sizeofArr-i-1;k++ ){
                if (arr.get(k) > arr.get(k+1)){
                    temp = arr.get(k);
                    arr.set(arr.get(k+1), k);
                    arr.set(temp, k+1);
                    check = 0;
                }
            }
            if (check == 1) break;
        }
        return arr;
    }

}
