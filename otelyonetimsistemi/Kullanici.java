package otelyonetimsistemi;

/**
 *
 * @author mehmetkaanerol
 */
public class Kullanici implements IRezervasyon{
    private String ad;
    private String soyad;
    private int yas;
       
    /**
    * Kullanici sinifinin baslangic degerlerini verir
    */
    Kullanici(){
        this("ad", "soyad", 0);
    }
    
    /**
    * Kullanici sinifinin baslangic degerlerini verir
    *
    * @param  ad        Kullanici ad bilgisi alinir
    * @param  soyad     Kullanici soyad bilgisi alinir
    * @param yas        Kullanici yas bilgisi alinir
    */
    Kullanici(String ad, String soyad, int yas){
        this.ad = ad;
        this.soyad = soyad;
        this.yas = yas;
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
     * @return the ad
     */
    public String getAd() {
        return ad;
    }

    /**
     * @param ad the ad to set
     */
    public void setAd(String ad) {
        this.ad = ad;
    }

    /**
     * @return the soyad
     */
    public String getSoyad() {
        return soyad;
    }

    /**
     * @param soyad the soyad to set
     */
    public void setSoyad(String soyad) {
        this.soyad = soyad;
    }

    /**
     * @return the yas
     */
    public int getYas() {
        return yas;
    }

    /**
     * @param yas the yas to set
     */
    public void setYas(int yas) {
        this.yas = yas;
    }
    
    /**
     * @return kullanici sinifinin bilgileri gonderilir
     */
    @Override
    public String toString() { 
        return String.format("Ad " + this.ad + " soyad " + this.soyad + " yas " + this.yas); 
    }

}
