package otelyonetimsistemi;

/**
 *
 * @author mehmetkaanerol
 */
import java.util.ArrayList;
import otelyonetimsistemi.FileManager;
import otelyonetimsistemi.OtelMusterisi;
import otelyonetimsistemi.Resepsiyonist;
import otelyonetimsistemi.Kullanici;

public class OtelYonetimSistemi {

    public static void main(String[] args) {
        // Otel Yönetim Sistemi: Test Class
        
        // Oda sayisi, kayit dosyasinin konumu
        FileManager fm = new FileManager("oda_log.txt", 100);
        fm.fileCreateTable();
        /*
        fm.setFilePath("text.txt");
        fm.setOdaSayisi(100);
        */
        
        // Test 1:
        // Musteri, resepsiyonist aracılığıyla rezerv, rezerv iptali, check-in, check-out işlemlerini yapar.
        OtelMusterisi om = new OtelMusterisi("Kaan", "Erol", "1234", 23);
        Resepsiyonist rm = new Resepsiyonist("Mehmet", "Yimaz", "Hilton", 23);
        
        rm.odaRezervasyon(54, om);
        rm.odaRezervasyonIptal(55, om);
        rm.odaRezervasyonIptal(54, om);
        
        rm.odaRezervasyon(12, om);
        rm.checkIn(12, om);
        rm.checkOut(12, om);
        
        rm.odaRezervasyonIptal(12, om);
        
        // Test 2:
        // Başka bir musteri kendi başına rezerv ve rezerv iptali yapar.
        // Ardından aynı (yukarıda tanımlanan) resepsiyonist aracılığıyla rezerv, rezerv iptali, check-in, check-out işlemlerini yapar.
        OtelMusterisi om2 = new OtelMusterisi("Ahmet", "Erol", "1235", 45);
        
        om2.odaRezervasyon(12);
        om2.odaRezervasyonIptal(12);
        
        rm.odaRezervasyon(7, om2);
        rm.checkIn(7, om2);
        rm.checkOut(7, om2);
        
        rm.odaRezervasyonIptal(7, om2);
        
        // Test 3:
        // Var olmayan oda numarasi hem resepsiyonist hem musteri icin verilir.
        om2.odaRezervasyon(12123);
        om2.odaRezervasyonIptal(1212);
        
        rm.odaRezervasyon(12123, om);
        rm.odaRezervasyonIptal(1212, om);
        
        // Test 4:
        // Musteri tarafından check-in yapılmış oda için rezerv iptali denenir.
        om2.odaRezervasyon(5);
        rm.checkIn(5, om2);
        om2.odaRezervasyonIptal(5);
        
        // Polymorphism örneği
        // Upcasting, Downcasting örnekleri
        Kullanici km1 = new OtelMusterisi();
        km1.setAd("Faruk");
        km1.setSoyad("yil");
        km1.setYas(37);
        print(km1);
        
        Kullanici km2 = new Resepsiyonist();
        km2.setAd("Yilmaz");
        km2.setSoyad("tastan");
        km2.setYas(34);
        print(km2);
                
        OtelMusterisi om3 = new OtelMusterisi("ali", "yilmaz", "563", 12);

        Kullanici km = (Kullanici) om3;
        print(km);

        Resepsiyonist rm3 = new Resepsiyonist("veli", "yilmaz", "Madarin Hotel", 12);
        Kullanici km3 = (Kullanici) rm3;
        print(km3);
    }
        
    /**
    * Verilen Object nesnesinin @Override edilmis toString() metodunun donutunu yazdirir
    *
    * @param obj     Object tipinde bir nesne alır
    */
    public static void print(Object obj){
        System.out.println("\n" + obj.toString());
    }
    
}
