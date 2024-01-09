#include "DataLogger.h"

void DataLogger::init() {
    fileHandler.init();

    // check if files exists
    if (!fileHandler.fileExists("/publishQueue.txt")) {
        // if not, create files
        fileHandler.writeFile("/publishQueue.txt", "");
        fileHandler.writeFile("/errorLog.txt", "");
    }

    fileHandler.listDir("/", 3);
}

// ------------ publishQueue.txt ------------ 

String DataLogger::flushQueueGetAll() {
    return fileHandler.readFile("/publishQueue.txt");
}

String DataLogger::flushQueueGetLine() {
    return fileHandler.readLine("/publishQueue.txt");
}

void DataLogger::flushQueueAppendLine(const char * message) {
    fileHandler.appendFile("/publishQueue.txt", String(message).c_str());
}

// void DataLogger::publishQueueDeleteLine() {

// }

void DataLogger::flushQueueClearFile() {
    fileHandler.writeFile("/publishQueue.txt", "");
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