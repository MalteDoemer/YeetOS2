#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"

void* memcpy(void* dest, const void* src, size_t num);

#ifdef __cplusplus
}
#endif