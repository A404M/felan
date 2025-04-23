#pragma once

#include <stddef.h>

extern size_t fileCodes_capacity;
extern char **fileCodes;
extern char **fileCodes_names;
extern size_t fileCodes_length;

void fileInit();
void fileDelete();

void filePush(const char *filePath, char *code);

char *readWholeFile(const char *filePath);
size_t getFileIndex(const char *filePath);

char *joinToPathOf(const char *original, const char *file);
