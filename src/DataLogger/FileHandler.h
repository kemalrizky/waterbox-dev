#pragma once

#include <Arduino.h>
#include "FS.h"
#include <LITTLEFS.h>

/* You only need to format LITTLEFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin
   
   If you test two partitions, you need to use a custom
   partition.csv file, see in the sketch folder */

//#define TWOPART

#define FORMAT_LITTLEFS_IF_FAILED true

class FileHandler {
    public:
        bool init();
        void listDir(const char * dirname, uint8_t levels);
        void createDir(const char * path);
        void removeDir(const char * path);
        bool fileExists(const char * path);
        String readLine(const char * path);
        String readFile(const char * path);
        void writeFile(const char * path, const char * message);
        void appendFile(const char * path, const char * message);
        void renameFile(const char * path1, const char * path2);
        void deleteFile(const char * path);

        // SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

        void writeFile2(const char * path, const char * message);
        void deleteFile2(const char * path);
        void testFileIO(const char * path);
        void test();
};