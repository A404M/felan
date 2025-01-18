#pragma once

#include <stddef.h>

extern void *a404m_malloc(size_t size);
extern void *a404m_realloc(void *restrict pointer, size_t size);
extern size_t a404m_malloc_usable_size(void *pointer);
