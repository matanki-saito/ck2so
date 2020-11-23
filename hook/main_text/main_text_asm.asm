%define ESCAPE_SEQ_1    0x10
%define ESCAPE_SEQ_2    0x11
%define ESCAPE_SEQ_3    0x12
%define ESCAPE_SEQ_4    0x13
%define LOW_SHIFT       0x0F
%define HIGH_SHIFT      0x09
%define SHIFT_2		LOW_SHIFT
%define SHIFT_3		0x900
%define SHIFT_4         0x8F1
%define NO_FONT         0x98F
%define NOT_DEF         0x2026

GLOBAL mainTextProc1
GLOBAL mainTextProc1ReturnAddress
GLOBAL mainTextProc1JmpAddress

[SECTION .data]
mainTextProc1ReturnAddress: DQ 0
mainTextProc1JmpAddress: DQ 0

[SECTION .text]
mainTextProc1:
    movsxd  rax, ebp
    movzx   ecx, BYTE [rax+0x2418f00]
    movsxd  rdx, ebx
    lea     r14, [rdx+1]

    push QWORD [rel mainTextProc1ReturnAddress]
    ret

mainTextProc2:
    cmp     BYTE [r12 + rcx], ESCAPE_SEQ_1
    jz      .JMP_A
    cmp     BYTE [r12 + rcx], ESCAPE_SEQ_2
    jz      .JMP_B
    cmp     BYTE [r12 + rcx], ESCAPE_SEQ_3
    jz      .JMP_C
    cmp     BYTE [r12 + rcx], ESCAPE_SEQ_4
    jz      .JMP_D
    movzx   edx, BYTE [r12 + rcx]
    jmp     .JMP_E

.JMP_A:
    movzx   edx, WORD [r12 + rcx + 1]
    jmp     .JMP_F

.JMP_B:
    movzx   edx, WORD [r12 + rcx + 1]
    sub     edx, SHIFT_2
    jmp     .JMP_F

.JMP_C:
    movzx   edx, WORD [r12 + rcx + 1]
    add     edx, SHIFT_3
    jmp     .JMP_F

.JMP_D:
    movzx   edx, WORD [r12 + rcx + 1]
    add     edx, SHIFT_4

.JMP_F:
    movzx   edx, dx
    add     ebx, 2
    cmp     edx, NO_FONT

    ja      .JMP_E
    mov     edx, NOT_DEF

.JMP_E:
    mov     rdx, [r12 + rcx * 8 + 0x0D0]

    test    rdx, rdx

    jz      mainTextProc1JmpAddress wrt ..plt
    mov     ecx, DWORD [rdx + 0x0C]

    mov     rsi, mainTextProc1ReturnAddress
    mov     rsi, [rsi]
    jmp     rsi
