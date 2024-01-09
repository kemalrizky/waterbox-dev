/**
 * assumption: can add to flushQueue per line, but currently can only delete line by clearing whole file
 * 
 * todo:
 *    - deleteLine: need to be able to delete line per line. the scenario is, when doing flushing,
 *                  the data will be read line by line, for every successful publish, the successful
 *                  lines will be deleted. another alternative would be to implement the flushing
 *                  logic within DataLogger, to maintain full control of where the current pointer
 *                  to file is at, therefore deleting all read data until the point of the pointer
 */

#include "FileHandler.h"

class DataLogger {
    public:
        void init();

        String flushQueueGetAll();
        String flushQueueGetLine();
        void flushQueueAppendLine(const char * message);
        // void flushQueueDeleteLine();
        void flushQueueClearFile();
        
        String errorLogGetAll();
        String errorLogGetLine();
        void errorLogAppendLine(const char * message);
        // void errorLogDeleteLine();
        void errorLogClearFile();

    private:
        FileHandler fileHandler;
};