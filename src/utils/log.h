#pragma once

#define printLog(format,...) _printLogBack(format, __FILE_NAME__, __LINE__, ## __VA_ARGS__)

extern void _printLogBack(const char *format, const char *file, int line, ...);
