bits 32


; void* memcpy(void* dest, const void* src, size_t count)
global memcpy
memcpy:
    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, [ebp + 8]      ; dest parameter
    mov eax, edi            ; save dest to return it later
    mov esi, [ebp + 12]     ; src parameter
    mov ecx, [ebp + 16]     ; count parameter

    rep movsb               ; perform the copy

    pop esi
    pop edi
    leave
    ret


;void* memmove(void* dest, const void* src, size_t n)
global memmove
memmove:
    push ebp
    mov ebp, esp
    push edi
    push esi
    
    mov ecx, [ebp + 16]     ; count parameter
    test ecx, ecx           ; check if ecx is zero
    jz .done                ; and exit

    mov edi, [ebp + 8]      ; dest parameter
    mov eax, edi            ; save dest to return it later
    mov esi, [ebp + 12]     ; src parameter

    cmp edi, esi            ; check if dest > src
    ja .normal

    lea edi, [edi + ecx - 1]
    lea esi, [esi + ecx - 1]

    std
    rep movsb               ; perform the copy backwards
    cld

    jmp .done

.normal:
    rep movsb               ; perform the copy

.done:
    pop esi 
    pop edi
    leave
    ret

; void* memset(void* dest, int value, size_t count)
global memset
memset:
    push ebp
    mov ebp, esp
    push edi

    mov edi, [ebp + 8]      ; dest parameter
    mov edx, edi            ; save dest for later
    mov eax, [ebp + 12]     ; value parameter
    mov ecx, [ebp + 16]     ; count parameter

    rep stosb
    
    mov eax, edx            ; return dest

    pop edi
    leave
    ret