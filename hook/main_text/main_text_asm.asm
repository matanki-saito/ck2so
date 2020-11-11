GLOBAL mainTextProc1
EXTERN addA

[SECTION .text]
mainTextProc1:
    mov     rdx, QWORD [r12 + rcx*8 + 0xD0]
    test    rdx, rdx
    jz      1
    call    addA
    mov     ecx, DWORD [rdx + 0x0C]
    push    1
    ret