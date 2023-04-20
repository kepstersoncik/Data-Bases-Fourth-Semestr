package src;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;

public class Settings {
    private Properties properties;
    private String fileName;

    public Settings(String fileName){
        properties = new Properties();
        this.fileName = fileName;
    }

    public void load() throws IOException{
        FileInputStream inputStream = new FileInputStream(fileName);
        properties.load(inputStream);
        inputStream.close();
    }

    public void save() throws IOException{
        FileOutputStream outputStream = new FileOutputStream(fileName);
        properties.store(outputStream, "Application Settings");
        outputStream.close();
    }

    public String getProperty(String key){
        return properties.getProperty(key);
    }

    public void setProperty(String key, String value){
        properties.setProperty(key, value);
    }
}
