#include "SpiffsHandler.h"

SpiffsHandler::SpiffsHandler(): path("data/test.txt") {}

void SpiffsHandler::init() {
    if(!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
    }
}

String SpiffsHandler::readFile() {
    File file = SPIFFS.open(path, FILE_READ);
    String _content;

    if(file) {
        while(file.available()) {
            _content += (char)file.read();
        }  
    }
    else {
        Serial.println("Failed to open file for reading");
    }
    
    file.close();
    return _content;
}

void SpiffsHandler::appendFile(float _data, unsigned long _timestamp) {
    char _dataChar[7];                     // max 999.99
    dtostrf(_data, 3, 2, _dataChar);
    String _payload = String(_dataChar) + "," + String(_timestamp) + ";";
    
    File file = SPIFFS.open(path, FILE_APPEND);

    if(file) {
        if(file.print(_payload.c_str())) {
            Serial.println("File appended succesfully");
        }
        else {
            Serial.println("Append failed");
        }
    }
    else {
        Serial.println("Failed to open file for appending");
        return;
    }
}

void SpiffsHandler::emptyFile() {
    File file = SPIFFS.open(path, FILE_WRITE);

    file.close();
    Serial.println("File emptied");
}