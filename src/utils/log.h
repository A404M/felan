#pragma once

#define printLog(format,...) _printLogBack(format, __FILE_NAME__, __LINE__, ## __VA_ARGS__)
#define printError(format,begin,end,...) _printErrorBack(format, __FILE_NAME__, __LINE__, begin, end, ## __VA_ARGS__)

#define UNREACHABLE printLog("Unreachable");exit(1)

extern void _printLogBack(const char *format, const char *file, int line, ...);

extern void _printErrorBack(const char *format, const char *file, int line, char *begin, char *end, ...);
