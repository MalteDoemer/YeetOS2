#pragma once

#include "stdint.h"

#define KERNEL_BASE 0xC0000000

#define ALIGN(x, n) ((x + (n) - 1) & ~((n) - 1))

#define PACKED __attribute__((packed))