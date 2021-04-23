bits 32

%define KERNEL_CODE_DESC 0x08
%define KERNEL_DATA_DESC 0x10
%define USER_CODE_DESC 0x1B
%define USER_DATA_DESC 0x23
%define TSS_DESC 0x2B

; bool sse_init()
[global init_sse]
init_sse:
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
    mov esp, ebp
    pop ebp
    ret


; void load_gdt(GDTReference* gdtr)
[global load_gdt]
load_gdt:
    mov eax, [esp + 4]
    lgdt [eax]

    mov ax, KERNEL_DATA_DESC
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp KERNEL_CODE_DESC:.flush
.flush:

    ret

; void load_idt(IDTReference* idtr)
[global load_idt]
load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

; void load_tss(u16 descriptor)
[global load_tss]
load_tss:
    mov ax, [esp + 4]
    ltr ax
    ret
