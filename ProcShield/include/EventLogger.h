#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <string>

class EventLogger
{
public:
    void log(const std::string& message);
    void logEvent(const std::string& event);
};

#endif