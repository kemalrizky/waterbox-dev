#pragma once

#include "Global.h"
#include "SPIFFS.h"
#include "FS.h"

class SpiffsHandler {
    public:
     SpiffsHandler();
     void init();
     String readFile();
     void appendFile(const char *);
     void emptyFile();
     
    private:
     String path;

};