#include "DataLogger.h"

void DataLogger::init() {
    fileHandler.init();

    // check if files exists
    if (!fileHandler.fileExists("/flushQueue.txt")) {
        // if not, create files
        fileHandler.writeFile("/flushQueue.txt", "");
        fileHandler.writeFile("/errorLog.txt", "");
    }

    fileHandler.listDir("/", 3);
}

// ------------ flushQueue.txt ------------ 

String DataLogger::flushQueueGetAll() {
    return fileHandler.readFile("/flushQueue.txt");
}

String DataLogger::flushQueueGetLine() {
    return fileHandler.readLine("/flushQueue.txt");
}

void DataLogger::flushQueueAppendLine(const char * message) {
    fileHandler.appendFile("/flushQueue.txt", String(message).c_str());
}

// void DataLogger::publishQueueDeleteLine() {

// }

void DataLogger::flushQueueClearFile() {
    fileHandler.writeFile("/flushQueue.txt", "");
}

// ------------ errorLog.txt ------------ 

String DataLogger::errorLogGetAll() {
    return fileHandler.readFile("/errorLog.txt");
}

String DataLogger::errorLogGetLine() {
    return fileHandler.readLine("/errorLog.txt");
}

void DataLogger::errorLogAppendLine(const char * message) {
    fileHandler.appendFile("/errorLog.txt", String(message).c_str());
}

// void DataLogger::errorLogDeleteLine() {

// }

void DataLogger::errorLogClearFile() {
    fileHandler.writeFile("/errorLog.txt", "");
}