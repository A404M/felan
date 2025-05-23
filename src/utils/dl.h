#pragma once

#include <stddef.h>

void dynamicLibraryInit();
void dynamicLibraryDestroy();

void* dynamicLibraryOpen(char *file);
