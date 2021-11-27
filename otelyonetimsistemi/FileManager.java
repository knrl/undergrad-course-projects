package otelyonetimsistemi;

/**
 *
 * @author mehmetkaanerol
 */
import java.util.ArrayList;
import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class FileManager {
    private String filePath = "oda_log.txt";
    private int odaSayisi;
    
    /**
    * FileManager sinifinin baslangic degerlerini verir
    */
    FileManager() {
        this("oda_log.txt", 100);
    }
    
    /**
    * FileManager sinifinin baslangic degerlerini verir
    *
    * @param  filePath      Kullanici filePath bilgisi alinir
    * @param  odaSayisi     Kullanici odaSayisi bilgisi alinir
    */
    FileManager(String filePath, int odaSayisi) {
        this.filePath = filePath;
        this.odaSayisi = odaSayisi;
    }
    
    /**
    * Kayit dosyasina tek satir yazan metottur
    *
    * @param  odaNumarasi       odaNumarasi bilgisi alinir
    * @param  musterID          oda musterID bilgisi alinir
    * @param  check             oda check bilgisi alinir
    * @param  stat              oda stat bilgisi alinir
    */
    public void fileWrite(int odaNumarasi, String musterID, String check, String stat){
        try(FileWriter fileWriter = new FileWriter(this.filePath, true)){
            String fileContent = Integer.toString(odaNumarasi) + " " + musterID + " " + check + " " + stat +"\n";
            fileWriter.write(fileContent);
            fileWriter.close();
        } catch(IOException e){
            // pass
        }
    }

    /**
    * Kayit dosyasinin okundugu metottur
    *
    * @return lines    Her satir bir karakter dizisi olarak dondurur
    */
    public ArrayList<String> fileRead(){
        ArrayList<String> lines = new ArrayList<String>();
        BufferedReader reader;
        try {
            reader = new BufferedReader(new FileReader(this.filePath));
            String line = reader.readLine();
            while(line != null){
                lines.add(line);
                line = reader.readLine();
            }
            reader.close();
        } catch(IOException e){
            // pass
        }
        return lines;
    }
    
    /**
    * Kayit dosyasinda arama yapan metottur
    *
    * @return text eğer bulunduysa bulunan satiri bulunmadiysa '0' dondurur
    */
    public String fileSearch(int no, String key){
        ArrayList<String> lines = this.fileRead();
        
        String[] arrOfStr;
        for (String text : lines){
            arrOfStr = text.split(" ", 0);
            if (arrOfStr[no].equals(key))
                return text;
        }
        return "0";
    }
    
    /**
    * Kayit dosyasini ve oda durumlarini sifirdan olusturan metottur
    *
    */
    public void fileCreateTable() {
        try(FileWriter fileWriter = new FileWriter(this.filePath)){ 
            for (int i=0;i<this.odaSayisi;i++)
                this.fileWrite((i+1), "0", "out", "bos");
            fileWriter.close(); 
        }
        catch(IOException e){   }
    }
    
    /**
    * Kayit dosyasini sifirlayan, tum icerigini silen metottur
    *
    */
    private void fileReset() {
        try(FileWriter fileWriter = new FileWriter(this.filePath)){ fileWriter.close(); }
        catch(IOException e){   }
    }
    
    /**
    * Kayit dosyasina ait tek satiri degistiren metottur
    *
    * @param  odaNumarasi       odaNumarasi bilgisi alinir
    * @param  musterID          oda musterID bilgisi alinir
    * @param  check             oda check bilgisi alinir
    * @param  stat              oda stat bilgisi alinir
    */
    public void fileChangeLine(int odaNumarasi, String musterID, String check, String stat){
        // Read all lines
        ArrayList<String> lines = this.fileRead();
        
        // Reset file
        this.fileReset();
        
        // Write new lines
        String[] arrOfStr;
        for (int i=0;i<this.odaSayisi;i++)
            if ((i+1) == odaNumarasi)
                this.fileWrite((i+1), musterID, check, stat);
            else{
                arrOfStr = lines.get(i).split(" ", 0);
                this.fileWrite((i+1), arrOfStr[1], arrOfStr[2], arrOfStr[3]);
            }
    }
        
    /**
     * @return the filePath
     */
    public String getFilePath(){
        return this.filePath;
    }
    
    /**
     * @param filePath the filePath to set
     */
    public void setFilePath(String filePath){
        this.filePath = filePath;
    }
    
    /**
     * @return the odaSayisi
     */
    public int getOdaSayisi(){
        return this.odaSayisi;
    }
    
    /**
     * @param odaSayisi the odaSayisi to set
     */
    public void setOdaSayisi(int odaSayisi){
        this.odaSayisi = odaSayisi;
    }
    
}
