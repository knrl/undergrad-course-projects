package com.mycompany.hashodev;

/**
 *
 * @author mehmetkaanerol
 */
public class HashMap {
    private final int numList = 26;
    private LinkedList array[] = new LinkedList[numList];
    private int size;

    public HashMap() {
        this.size = 0;
        for (int i=0;i<this.numList;i++)
            array[i] = new LinkedList();
    }

    private int getIndex(String key) {
        return (((int) key.toLowerCase().charAt(0)) - 97) % numList;
    }

    public void put(String key, rehberVeri value) {
        int index = getIndex(key);
        array[index].add(value);
    }

    public boolean remove(String key) {
        int index = getIndex(key);
        if (array[index].size() != 0) {
            for (int i=0;i<array[index].size();i++)
                if (array[index].get(i).isim == key) {
                    array[index].remove(i);
                    return true;
                }
        }
        return false;
    }

    public String search(String key) {
        int index = getIndex(key);
        if (array[index].size() != 0) {
            for (int i=0;i<array[index].size();i++)
                if (array[index].get(i).isim == key) 
                    return array[index].get(i).toString();
        }
        return null;
    }

    public int size() { 
        return this.size; 
    }

    public boolean isEmpty() { 
        return this.size() == 0; 
    }

}
