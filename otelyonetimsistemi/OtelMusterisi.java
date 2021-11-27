package otelyonetimsistemi;

/**
 *
 * @author mehmetkaanerol
 */
import java.util.Scanner;
import otelyonetimsistemi.FileManager;

public class OtelMusterisi extends Kullanici {
    private String musteriNo;
    
    /**
    * OtelMusterisi sinifinin baslangic degerlerini verir
    */
    OtelMusterisi(){
        this("ad", "soyad", "0", 0);
    }
    
    /**
    * OtelMusterisi sinifinin baslangic degerlerini verir
    *
    * @param  ad        OtelMusterisi ad bilgisi alinir
    * @param  soyad     OtelMusterisi soyad bilgisi alinir
    * @param  musteriNo OtelMusterisi musteriNo bilgisi alinir
    * @param yas        OtelMusterisi yas bilgisi alinir
    */
    OtelMusterisi(String ad, String soyad, String musteriNo, int yas){
        super(ad, soyad, yas);
        this.musteriNo = musteriNo;
    }

    /**
    * Oda rezervasyon isleminin yapildigi metottur
    * 
    * @param  odaNumarasi   rezervasyon yapilacak oda numarasi alinir
    */
    @Override
    public void odaRezervasyon(int odaNumarasi){
        System.out.println("\n*** \tRezervasyon Sistemi \t***");
        FileManager fm = new FileManager();
        String respond = fm.fileSearch(0, Integer.toString(odaNumarasi));
        if (!respond.equals("0")) {
            System.out.println(Integer.toString(odaNumarasi) + " numarali oda durumu: " + respond.split(" ", 0)[3]);

            if (respond.split(" ", 0)[3].equals("bos")) {
                System.out.println("Sayin " + super.getAd() + " kaydiniz yapilmistir.");
                fm.fileChangeLine(odaNumarasi, this.musteriNo, "waiting", "rezerv");
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda check-in durumu: waiting olarak guncellendi.");
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda durumu: rezerv olarak guncellendi.");
            }
            else {
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda doludur. Baska oda seciniz: ");
                Scanner scan = new Scanner(System.in);
                odaNumarasi = scan.nextInt();
                this.odaRezervasyon(odaNumarasi);
            }
        }
        else System.out.println("Oda bulunamadı. Otelimiz oda numaralari 1 - 100 arasındadir.");
    }

    /**
    * Oda rezervasyon iptal isleminin yapildigi metottur
    * 
    * @param  odaNumarasi   rezervasyon yapilacak oda numarasi alinir
    */
    @Override
    public void odaRezervasyonIptal(int odaNumarasi){
        System.out.println("\n*** \tRezervasyon Iptal Sistemi \t***");
        FileManager fm = new FileManager();
        String respond = fm.fileSearch(0, Integer.toString(odaNumarasi));
        if (!respond.equals("0")) {
            if (respond.split(" ", 0)[3].equals("rezerv")) {
                if (respond.split(" ", 0)[2].equals("in")) System.out.println(Integer.toString(odaNumarasi) + " numarali oda check-in durumu: in. Bu durumda rezervasyon iptali yapilamaz. Ancak check-out yapabilirsiniz.");
                else {
                    System.out.println("Sayin " + super.getAd() + " kaydiniz silinmistir.");
                    fm.fileChangeLine(odaNumarasi, "0", "out", "bos");
                    System.out.println(Integer.toString(odaNumarasi) + " numarali oda durumu: bos olarak guncellendi.");
                }
            }
            else {
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda durumu: " + respond.split(" ", 0)[3]);
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda icin henuz kayit yapilmamistir.");
            }
        }
        else System.out.println("Oda bulunamadı. Otelimiz oda numaralari 1 - 100 arasındadir.");
    }

    /**
     * @return the musteriNo
     */
    public String getMusteriNo() {
        return musteriNo;
    }

    /**
     * @param musteriNo the musteriNo to set
     */
    public void setMusteriNo(String musteriNo) {
        this.musteriNo = musteriNo;
    }
    
    /**
     * @return otel musterisi sinifinin bilgileri gonderilir
     */
    @Override
    public String toString() { 
        return "Otel musterisi: " + super.toString() + " musteri no " + this.musteriNo;
    }
    
}
