package com.mycompany.dnn;

/**
 *
 * @author mehmetkaanerol
 */
import java.util.ArrayList;
import java.util.List;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

/**
 *
 * @author mehmetkaanerol
 */
public class FileManagement {
    FileManagement() {}

    /**
     *
     * @param path okunacak dosyanın yolu
     * @return okunan graf veri yapısı
     */
    public List<Graph> read(String path){
        int src=0, dst=0, value=0;
        List<Edge> edgesList = new ArrayList<Edge>();
        List<Graph> graphList = new ArrayList<Graph>();

        try {
            File fileObj = new File(path);
            Scanner fileReader = new Scanner(fileObj);
            while (fileReader.hasNextLine()) {
                String data = fileReader.nextLine();
                for (String datum : data.split(";")){
                    src = Integer.parseInt(datum.split(",")[0]);
                    dst = Integer.parseInt(datum.split(",")[1]);
                    value = Integer.parseInt(datum.split(",")[2]);
                    edgesList.add(new Edge(src,dst,value));
                }
                graphList.add(new Graph(edgesList));
                edgesList = new ArrayList<Edge>();
            }
            fileReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }

        return graphList;
    }
    
}
