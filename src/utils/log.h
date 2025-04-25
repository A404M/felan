#pragma once

#include <stdlib.h>
#include "utils/type.h"

#ifndef __FILE_NAME__
#define __FILE_NAME__ __FILE__
#endif

#ifdef __GNUC__
#define UNREACHABLE_BACK (__builtin_unreachable())
#else
#define UNREACHABLE_BACK (exit(1))
#endif

#define printLog(format,...) _printLogBack(format, __FILE_NAME__, __LINE__, ## __VA_ARGS__)
#define printError(begin,end,format,...) _printErrorWarningBack(__FILE_NAME__, __LINE__, begin, end, true, format, ## __VA_ARGS__)
#define printWarning(begin,end,format,...) _printErrorWarningBack(__FILE_NAME__, __LINE__, begin, end, false, format, ## __VA_ARGS__)

#define UNREACHABLE printLog("Unreachable"); UNREACHABLE_BACK
#define NOT_IMPLEMENTED printLog("Not implemented"); exit(1)

extern void _printLogBack(const char *format, const char *file, int line, ...);

extern void _printErrorWarningBack(const char *file, int line, char *begin, char *end,bool isError,const char *format,  ...);
