#pragma once

#include "stddef.h"
#include "stdarg.h"

#ifdef __cplusplus
extern "C" {
#endif

int sprintf(char* buffer, const char* fmt, ...);
int snprintf(char* buffer, size_t count, const char* fmt, ...);
int vsprintf(char* buffer, const char* fmt, va_list va);
int vsnprintf(char* buffer, size_t count, const char* fmt, va_list va);

#ifdef __cplusplus
}
#endif