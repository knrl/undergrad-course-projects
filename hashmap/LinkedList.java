package com.mycompany.hashodev;

/**
 *
 * @author mehmetkaanerol
 */
public class LinkedList {
    private int counter;
    private Node head;
    private Node tail;

    public LinkedList() { 
        head = null;
        tail = null;
        counter = 0;
    }

    public void add(rehberVeri data) {
        if (head == null) {
            head = new Node(data);
            tail = null;
            head.next = tail;
        }
        else {
            Node temp = new Node(data);
            if (tail != null)
                tail.next = temp;
            tail = temp;
            tail.next = null;
        }
        counter++;
    }

    public boolean remove(int index) {
        if (index < 0 || index > size()) return false;

        if (head != null) {
            Node current = head;
            if (index == 0) {
                if (size() == 1) head = null;
                else head = current.next;
            }
            else {
                for (int i = 0; i < index - 1; i++) {
                    if (current.next == null) return false;
                    current = current.next;
                }
                if (index == size() - 1){
                    tail = current;
                    tail.next = null;
                }
                else current.next = current.next.next;
            }
            counter--;
            return true;
        }
        return false;
    }

    public rehberVeri get(int index) {
        if (index < 0 || index > size()) return null;

        if (head != null) {
            Node current = head;
            for (int i = 0; i < index; i++) {
                if (current.next == null) return null;
                current = current.next;
            }
            return current.data;
        }
        return null;
    }

    public int size() {
        return this.counter;
    }

}