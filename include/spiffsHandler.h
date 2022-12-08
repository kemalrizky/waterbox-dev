#include "SPIFFS.h"

void readFile() {
    Serial.println("\nReading file");

    File file = SPIFFS.open("/test.txt");
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    Serial.println("File Content:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(){
    Serial.println("\nWriting file");
    
    File file = SPIFFS.open("/test.txt", FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.println("Temperature data")){
        Serial.println("File written");
    } 
    else {
        Serial.println("Write failed");
    }
  file.close();
}

void appendFile(const char * message){
    Serial.println("\nAppending to file");
    File file = SPIFFS.open("/test.txt", FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.println(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}
