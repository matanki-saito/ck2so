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
GLOBAL mainTextProc2
GLOBAL mainTextProc1ReturnAddress
GLOBAL mainTextProc2ReturnAddress

[SECTION .data]
mainTextProc1ReturnAddress: DQ 0
mainTextProc2ReturnAddress: DQ 0

[SECTION .text]
mainTextProc1:
    movsxd  rax, ebp
    movsxd  rdx, ebx

    cmp     BYTE [rax+0x2418f00], ESCAPE_SEQ_1
    jz      .JMP_A
    cmp     BYTE [rax+0x2418f00], ESCAPE_SEQ_2
    jz      .JMP_B
    cmp     BYTE [rax+0x2418f00], ESCAPE_SEQ_3
    jz      .JMP_C
    cmp     BYTE [rax+0x2418f00], ESCAPE_SEQ_4
    jz      .JMP_D

    movzx   ecx, BYTE [rax+0x2418f00]
    lea     r14, [rdx+1]
    mov     BYTE [rdx+0x2419f40],cl
    cmp     BYTE [rsp+0x518-0x438], 0
    jmp     .JMP_E

.JMP_A:
    movzx   ecx, WORD [rax+0x2418f00+1]
    mov     BYTE [rdx+0x2419f40],ESCAPE_SEQ_1
    jmp     .JMP_F

.JMP_B:
    movzx   ecx, WORD [rax+0x2418f00+1]
    mov     BYTE [rdx+0x2419f40],ESCAPE_SEQ_2
    sub     ecx, SHIFT_2
    jmp     .JMP_F

.JMP_C:
    movzx   ecx, WORD [rax+0x2418f00+1]
    mov     BYTE [rdx+0x2419f40],ESCAPE_SEQ_3
    add     ecx, SHIFT_3
    jmp     .JMP_F

.JMP_D:
    movzx   ecx, WORD [rax+0x2418f00+1]
    mov     BYTE [rdx+0x2419f40],ESCAPE_SEQ_4
    add     ecx, SHIFT_4

.JMP_F:
    movzx   ecx, cx
    cmp     ecx, NO_FONT
    ja      .JMP_G
    mov     ecx, NOT_DEF
.JMP_G:
    movsxd  rcx, ecx
    mov     WORD [rdx+0x2419f40+1],cx
    add     ebp, 2

    lea     r14, [rdx + 3]

    cmp     r14d,r14d

.JMP_E:
    push    QWORD [rel mainTextProc1ReturnAddress]
    ret

mainTextProc2:
    movsxd  r14, r13d

    cmp     BYTE [r14+0x2419700], ESCAPE_SEQ_1
    jz      .JMP_A
    cmp     BYTE [r14+0x2419700], ESCAPE_SEQ_2
    jz      .JMP_B
    cmp     BYTE [r14+0x2419700], ESCAPE_SEQ_3
    jz      .JMP_C
    cmp     BYTE [r14+0x2419700], ESCAPE_SEQ_4
    jz      .JMP_D
    movzx   eax, BYTE [r14+0x2419700]
    cmp     BYTE [rsp+0x518-0x438], 0
    jmp     .JMP_E

.JMP_A:
    movzx   eax, WORD [r14+0x2419700+1]
    jmp     .JMP_F
.JMP_B:
    movzx   eax, WORD [r14+0x2419700+1]
    sub     eax, SHIFT_2
    jmp     .JMP_F
.JMP_C:
    movzx   eax, WORD [r14+0x2419700+1]
    add     eax, SHIFT_3
    jmp     .JMP_F
.JMP_D:
    movzx   eax, WORD [r14+0x2419700+1]
    add     eax, SHIFT_4
.JMP_F:
    movzx   eax, ax
    cmp     eax, NO_FONT
    ja      .JMP_G
    mov     eax, NOT_DEF
.JMP_G:
    add     r13d,2
    movsxd  rax, eax
    test    rax,rax ; jzをtrueにする
    mov     rbx, QWORD [rsp + 0x518 - 0x4F0]

.JMP_E:
    push    QWORD [rel mainTextProc2ReturnAddress]
    ret

