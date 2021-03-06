#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"

void* memccpy(void* s1, const void* s2, int c, size_t n);

const void* memchr(const void* ptr, int value, size_t n);

int memcmp(const void* ptr1, const void* ptr2, size_t n);

void* memcpy(void* dest, const void* src, size_t n);

void* memmove(void* dest, const void* src, size_t n);

void* memset(void* dest, int value, size_t n);

char* strcat(char* dest, const char* src);

char* strchr(const char* str, int value);

int strcmp(const char* str1, const char* str2);

char* strcpy(char* dest, const char* src);

size_t strcspn(const char* str1, const char* str2);

char* strdup(const char* str);

size_t strlen(const char* str);

char* strncat(char* dest, const char* src, size_t n);

int strncmp(const char* str1, const char* str2, size_t n);

char* strncpy(char* dest, const char* src, size_t n);

char* strpbrk(const char* str1, const char* str2);

char* strrchr(const char* str, int value);

size_t strspn(const char* str1, const char* str2);

char* strstr(const char* str1, const char* str2);

char* strtok(char* str, const char* delims);

#ifdef __cplusplus
}
#endif