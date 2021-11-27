package com.mycompany.hashodev;

/**
 *
 * @author mehmetkaanerol
 */
public class Test {
    public static void main(String[] args) {
        rehberVeri kisi1 = new rehberVeri("ayse", "51236543433");
        rehberVeri kisi2 = new rehberVeri("mehmet", "55632893422");
        rehberVeri kisi3 = new rehberVeri("kaan", "55236142412");
        
        HashMap map = new HashMap();
        map.put(kisi1.isim, kisi1);
        map.put(kisi2.isim, kisi2);
        map.put(kisi3.isim, kisi3);
        map.remove(kisi3.isim);
        System.out.println(map.search(kisi2.isim));
    }

}
