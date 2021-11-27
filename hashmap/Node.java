package com.mycompany.hashodev;

/**
 *
 * @author mehmetkaanerol
 */
public class Node {
    Node next;
    rehberVeri data;

    /**
     *
     * @param dataValue
     */
    public Node(rehberVeri dataValue) {
        next = null;
        data = dataValue;
    }

    /**
     *
     * @param dataValue
     * @param nextValue
     */
    public Node(rehberVeri dataValue, Node nextValue) {
        next = nextValue;
        data = dataValue;
    }

}