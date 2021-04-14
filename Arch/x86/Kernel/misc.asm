bits 32

[global sse_init]
sse_init:
    push ebp
    mov ebp, esp

    ; save ebx since cpuid changes it
    push ebx

    ; check the cpuid if sse is supported
    mov eax, 1
    cpuid
    test edx, 1 << 25
    jz .no_sse

    mov eax, cr0
    and ax, ~(1 << 2) ; clear CR0.EM bit (coprocessor emulation)
    or ax, 1 << 1     ; set CR0.MP bit (coprocessor monitoring)
    mov cr0, eax

    mov eax, cr4
    or ax, 1 << 9     ; set CR4.OSFXSR (enable fxsave/fxrstore)
    or ax, 1 << 10    ; set CR4.OSXMMEXCPT (floating point exceptions)
    mov cr4, eax

    ; return true
    mov eax, 1
    pop ebx
    leave
    ret

.no_sse:
    ; return false
    xor eax, eax
    pop ebx
    leave
    ret