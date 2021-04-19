#/bin/bash

llvm-objdump --disassemble --source --arch-name=x86 --x86-asm-syntax=intel --symbolize-operands --print-imm-hex --demangle YeetOS