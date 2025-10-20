#ifndef _INCLUDE_STRING_
#define _INCLUDE_STRING_

#include <ant/types.h>

size_t strlen(const char *);
void strrev(char *);
void *memcpy(void *, const void *, u64);
void *memset(void *, u64, u64);
size_t memcmp(const void *, const void *, size_t);
void *strcpy(char *dest, const char *src);
size_t strncmp(const char *, const char *, size_t);
size_t strcmp(const char *, const char *);

#endif
