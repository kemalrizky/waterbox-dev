#include "FileHandler.h"

bool FileHandler::init() {
    if(!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
        Serial.println("LITTLEFS Mount Failed");
        return false;
    }
    return true;
}

void FileHandler::listDir(const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = LITTLEFS.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void FileHandler::createDir(const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(LITTLEFS.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void FileHandler::removeDir(const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(LITTLEFS.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

bool FileHandler::fileExists(const char * path) {
    return LITTLEFS.exists(path);
}

String FileHandler::readLine(const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = LITTLEFS.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return "";
    }

    Serial.println("- read line from file:");
    String buffer = "";
    while(file.available()){
        buffer = file.readString();
    }
    file.close();

    return buffer;
}

String FileHandler::readFile(const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = LITTLEFS.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return "";
    }

    Serial.println("- read from file:");
    String buffer = "";
    while(file.available()){
        buffer += file.read();
    }
    file.close();

    return buffer;
}

void FileHandler::writeFile(const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = LITTLEFS.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void FileHandler::appendFile(const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = LITTLEFS.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void FileHandler::renameFile(const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (LITTLEFS.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void FileHandler::deleteFile(const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(LITTLEFS.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

// SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

void FileHandler::writeFile2(const char * path, const char * message){
    if(!LITTLEFS.exists(path)){
    if (strchr(path, '/')) {
            Serial.printf("Create missing folders of: %s\r\n", path);
      char *pathStr = strdup(path);
      if (pathStr) {
        char *ptr = strchr(pathStr, '/');
        while (ptr) {
          *ptr = 0;
          LITTLEFS.mkdir(pathStr);
          *ptr = '/';
          ptr = strchr(ptr+1, '/');
        }
      }
      free(pathStr);
    }
    }

    Serial.printf("Writing file to: %s\r\n", path);
    File file = LITTLEFS.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void FileHandler::deleteFile2(const char * path){
    Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

    if(LITTLEFS.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }

    char *pathStr = strdup(path);
    if (pathStr) {
        char *ptr = strrchr(pathStr, '/');
        if (ptr) {
            Serial.printf("Removing all empty folders on path: %s\r\n", path);
        }
        while (ptr) {
            *ptr = 0;
            LITTLEFS.rmdir(pathStr);
            ptr = strrchr(pathStr, '/');
        }
        free(pathStr);
    }
}

void FileHandler::testFileIO(const char * path){
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = LITTLEFS.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = LITTLEFS.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
}

void FileHandler::test(){
    Serial.begin(115200);

#ifdef TWOPART
    if(!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED, "/lLITTLEFS2", 5, "part2")){
    Serial.println("part2 Mount Failed");
    return;
    }
    appendFile("/hello0.txt", "World0!\r\n");
    readFile("/hello0.txt");
    LITTLEFS.end();

    Serial.println( "Done with part2, work with the first lLITTLEFS partition..." );
#endif

    if(!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
        Serial.println("LITTLEFS Mount Failed");
        return;
    }
    Serial.println( "SPIFFS-like write file to new path and delete it w/folders" );
    writeFile2("/new1/new2/new3/hello3.txt", "Hello3");
    listDir("/", 3);
    deleteFile2("/new1/new2/new3/hello3.txt");
    
    listDir("/", 3);
    createDir("/mydir");
    writeFile("/mydir/hello2.txt", "Hello2");
    listDir("/", 1);
    deleteFile("/mydir/hello2.txt");
    removeDir("/mydir");
    listDir("/", 1);
    writeFile("/hello.txt", "Hello ");
    appendFile("/hello.txt", "World!\r\n");
    readFile("/hello.txt");
    renameFile("/hello.txt", "/foo.txt");
    readFile("/foo.txt");
    deleteFile("/foo.txt");
    testFileIO("/test.txt");
    deleteFile("/test.txt");
  
    Serial.println( "Test complete" ); 
}