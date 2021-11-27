package otelyonetimsistemi;

/**
 *
 * @author mehmetkaanerol
 */
import java.util.Scanner;
import otelyonetimsistemi.FileManager;
import otelyonetimsistemi.OtelMusterisi;

public class Resepsiyonist extends Kullanici{
    private String bagliOtel;
    
    /**
    * Resepsiyonist sinifinin baslangic degerlerini verir
    */
    Resepsiyonist(){
        this("ad", "soyad", "bos", 0);
    }
    
    /**
    * Resepsiyonist sinifinin baslangic degerlerini verir
    *
    * @param  ad        resepsiyonistin ad bilgisi alinir
    * @param  soyad     resepsiyonistion soyad bilgisi alinir
    * @param  bagliOtel resepsiyonistin bagliOtel bilgisi alinir
    * @param yas        resepsiyonistin yas bilgisi alinir
    */
    Resepsiyonist(String ad, String soyad, String bagliOtel, int yas){
        super(ad, soyad, yas);
        this.bagliOtel = bagliOtel;
    }

    /**
    * Check-out isleminin yapildigi metottur
    * 
    * @param  odaNumarasi   check-out yapilacak oda numarasi alinir
    * @param  musteri       check-out islemini talep eden musteri objesi alinir
    */
    public void checkOut(int odaNumarasi, OtelMusterisi musteri){
        System.out.println("\n*** \tCheck-Out Sistemi \t***");
        FileManager fm = new FileManager();
        String respond = fm.fileSearch(0, Integer.toString(odaNumarasi));
        if (!respond.equals("0")) {
            System.out.println(Integer.toString(odaNumarasi) + " numarali oda check-in durumu: " + respond.split(" ", 0)[2]);

            if (respond.split(" ", 0)[2].equals("in")) {
                System.out.println("Sayin " + musteri.getAd() + " check-out isleminiz yapilmistir.");
                fm.fileChangeLine(odaNumarasi, musteri.getMusteriNo(), "out", "bos");
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda check-in durumu: out olarak guncellendi.");
            }
        }
        else System.out.println("Oda bulunamadı. Otelimiz oda numaralari 1 - 100 arasındadir.");
    }

    /**
    * Check-in isleminin yapildigi metottur
    * 
    * @param  odaNumarasi   check-in yapilacak oda numarasi alinir
    * @param  musteri       check-in islemini talep eden musteri objesi alinir
    */
    public void checkIn(int odaNumarasi, OtelMusterisi musteri){
        System.out.println("\n*** \tCheck-In Sistemi \t***");
        FileManager fm = new FileManager();
        String respond = fm.fileSearch(0, Integer.toString(odaNumarasi));
        if (!respond.equals("0")) {
            System.out.println("Check-in durumu: " + respond.split(" ", 0)[2]);

            if (respond.split(" ", 0)[2].equals("waiting")) {
                System.out.println("Sayin " + musteri.getAd() + " check-in isleminiz yapilmistir.");
                fm.fileChangeLine(odaNumarasi, musteri.getMusteriNo(), "in", "rezerv");
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda check-in durumu: in olarak guncellendi.");
            }
            else System.out.println(musteri.getAd() + " adina rezervasyon bulunamamaktadir.");
        }
        else System.out.println("Oda bulunamadı. Otelimiz oda numaralari 1 - 100 arasındadir.");
    }
    
    /**
     * @param  odaNumarasi   rezervasyon yapilacak oda numarasi alinir
     */
    @Override
    public void odaRezervasyon(int odaNumarasi) {}
    
    /**
     * @param  odaNumarasi   rezervasyon yapilacak oda numarasi alinir
     */
    @Override
    public void odaRezervasyonIptal(int odaNumarasi) {}
    
    /**
    * Oda rezervasyon isleminin yapildigi metottur
    * 
    * @param  odaNumarasi   rezervasyon yapilacak oda numarasi alinir
    * @param  musteri       rezervasyon islemini talep eden musteri objesi alinir
    */
    public void odaRezervasyon(int odaNumarasi, OtelMusterisi musteri){
        System.out.println("\n*** \tRezervasyon Sistemi \t***");
        FileManager fm = new FileManager();
        String respond = fm.fileSearch(0, Integer.toString(odaNumarasi));
        if (!respond.equals("0")) {
            System.out.println(Integer.toString(odaNumarasi) + " numarali oda durumu: " + respond.split(" ", 0)[3]);

            if (respond.split(" ", 0)[3].equals("bos")) {
                System.out.println("Sayin " + musteri.getAd() + " kaydiniz yapilmistir.");
                fm.fileChangeLine(odaNumarasi, musteri.getMusteriNo(), "waiting", "rezerv");
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda check-in durumu: waiting olarak guncellendi.");
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda durumu: rezerv olarak guncellendi.");
            }
            else {
                System.out.println(Integer.toString(odaNumarasi) + " numarali oda doludur. Baska oda seciniz: ");
                Scanner scan = new Scanner(System.in);
                odaNumarasi = scan.nextInt();
                this.odaRezervasyon(odaNumarasi, musteri);
            }
        }
        else System.out.println("Oda bulunamadı. Otelimiz oda numaralari 1 - 100 arasındadir.");
    }

    /**
    * Oda rezervasyon iptal isleminin yapildigi metottur
    * 
    * @param  odaNumarasi   rezervasyon yapilacak oda numarasi alinir
    * @param  musteri       rezervasyon islemini talep eden musteri objesi alinir
    */
    public void odaRezervasyonIptal(int odaNumarasi, OtelMusterisi musteri){
        System.out.println("\n*** \tRezervasyon Iptal Sistemi \t***");
        FileManager fm = new FileManager();
        String respond = fm.fileSearch(0, Integer.toString(odaNumarasi));
        if (!respond.equals("0")) {
            if (respond.split(" ", 0)[3].equals("rezerv")) {
                if (respond.split(" ", 0)[2].equals("in")) System.out.println(Integer.toString(odaNumarasi) + " numarali oda check-in durumu: in. Bu durumda rezervasyon iptali yapilamaz. Ancak check-out yapabilirsiniz.");
                else {
                    System.out.println("Sayin " + musteri.getAd() + " kaydiniz silinmistir.");
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
     * @return the bagliOtel
     */
    public String getBagliOtel() {
        return bagliOtel;
    }

    /**
     * @param bagliOtel the bagliOtel to set
     */
    public void setBagliOtel(String bagliOtel) {
        this.bagliOtel = bagliOtel;
    }
    
    /**
     * @return resepsiyonist sinifinin bilgileri gonderilir
     */
    @Override
    public String toString() { 
        return "Resepsiyonist: " + super.toString() + " bagli otel " + this.bagliOtel;
    }
    
}
