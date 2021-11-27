package com.mycompany.hashodev;

/**
 *
 * @author mehmetkaanerol
 */

public class rehberVeri {
    String isim; 
    String telNo;

    rehberVeri() {
        this.isim = "";
        this.telNo = "";
    }
    
    rehberVeri(String isim, String telNo) {
        this.isim = isim;
        this.telNo = telNo;
    }
    
    /**
     *
     * @return rehber veri yapısı için override toString() fonksiyonu
     */
    @Override
    public String toString() {
        return "Isim " + isim + " Tel No " + telNo;
    }

}