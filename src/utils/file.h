#pragma once

#include <stddef.h>

extern size_t fileCodes_capacity;
extern char **fileCodes;
extern char **fileCodes_names;
extern size_t fileCodes_length;

void fileInit();
void fileDelete();

char *readWholeFile(const char *filePath);
