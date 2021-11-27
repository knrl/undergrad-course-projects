package com.mycompany.odev6;

/**
 *
 * @author mehmetkaanerol
 */
public class LinkedList {
    private int counter;
    private Node head;
    private Node tail;

    /**
     *
     */
    public LinkedList() { }

    /**
     *
     * @param data
     */
    public void add(int data) {
        if (head == null) {
            head = new Node(data);
            tail = head;
            head.next = tail;
            tail.next = head;
        }
        else {
            Node temp = new Node(data);
            if (size() != 0) {
                tail.next = temp;
                tail = temp;
                tail.next = head;
            }
        }
        counter++;
    }

    /**
     *
     * @param data
     * @param index
     */
    public void insert(int data, int index) {
        if (head == null) this.add(data);
        else {
            if (index >= 0 && index <= size()) {
                Node temp = new Node(data);
                Node current = head;
                if (index == 0) {
                    temp.next = head;
                    head = temp;
                    tail.next = head;
                }
                else if (index == size()){
                    tail.next = temp;
                    tail = temp;
                    tail.next = head;
                }
                else {
                    for (int i = 0; i < index - 1 && current.next != null; i++)
                        current = current.next;
                    temp.next = current.next;
                    current.next = temp;
                }
                counter++;
            }
        }
    }

    /**
     *
     * @param index
     * @return
     */
    public int get(int index) {
        if (index < 0 && index > size()) return -1;

        Node current = head;
        if (head != null && size() > index) {
            for (int i = 0; i < index; i++) {
                if (current.next == null) return -1;
                current = current.next;
            }
            return current.data;
        }
        return -1;
    }

    /**
     *
     * @param value
     * @param index
     */
    public void set(int value, int index) {
        if (index < 0 && index > size()){
            
        }
        else{
            Node current = head;
            if (head != null && size() > index) {
                for (int i = 0; i < index; i++) {
                    if (current.next == null) break;
                    current = current.next;
                }
                current.data = value;
            }

        }
    }

    /**
     *
     * @return
     */
    public int size() {
        return this.counter;
    }

}

class Node{
    Node next;
    int data;
 
    /**
     *
     * @param dataValue
     */
    public Node(int dataValue) {
        next = null;
        data = dataValue;
    }

    /**
     *
     * @param dataValue
     * @param nextValue
     */
    public Node(int dataValue, Node nextValue) {
        next = nextValue;
        data = dataValue;
    }

}