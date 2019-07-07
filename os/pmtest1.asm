%include    "pm.inc"
org     07c00h
    jmp     LABEL_BEGIN


[section    .gdt]
LBAEL_GDT:  Descritor   0, 0, 0
LABEL_DESC_CODE32:  Descritor   0, SegCodec32Len-1, DA_C + DA_32
LABEL_DESC_VIDEO:   Descritor   0B8000h,    0ffffh, DA_DRW

GdtLen  equ $- LABEL_GDT
GdtPtr  dw GdtLen-1
        dd 0

SelectorCode32  equ LABEL_DESC_CODE32-LABEL_GDT
SelectorVideo   equ LABEL_DESC_VIDEO-LABEL_GDT

[SECTION .s16]
[BITS 16]
LABEL_BEGIN:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    move    sp, 0100h
    
    xor eax, eax
