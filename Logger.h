#ifndef LOGGER_H
#define LOGGER_H

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <iostream>

#define __FILENAME__ strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

#define	LOG_ERR		1
#define	LOG_WARNING	2
#define	LOG_INFO	3
#define	LOG_DEBUG	4
#define LOG_TRACE   5

#define _ERROR_     LOG_ERR
#define _WARNING_      LOG_WARNING
#define _INFO_      LOG_INFO
#define _DEBUG_     LOG_DEBUG
#define _TRACE_     LOG_TRACE

#ifndef LOG_LEVEL
#define LOG_LEVEL   _INFO_
#endif

#include <string.h>

#ifndef LOG_CATEGORY
  #define LOG_CATEGORY "GENERAL"
#endif

#define PRINTFUNCTION(MSG, LEVEL, FILE, LINE, ...) \
    if (_##LEVEL##_ <= LOG_LEVEL) { \
        std::cout << fmt::format("[{}]:{}:{}:{}:" #LEVEL ":" MSG, LOG_CATEGORY, FILE, LINE, __FUNCTION__, ##__VA_ARGS__) << std::endl; \
    }

#define TRACE()             PRINTFUNCTION("",  TRACE, __FILENAME__, __LINE__)
#define DEBUG(MSG, ...)     PRINTFUNCTION(MSG, DEBUG, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define INFO(MSG, ...)      PRINTFUNCTION(MSG, INFO, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define WARNING(MSG, ...)   PRINTFUNCTION(MSG, WARNING, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define ERROR(MSG, ...)     PRINTFUNCTION(MSG, ERROR, __FILENAME__, __LINE__, ##__VA_ARGS__)

#endif // LOGGER_H
