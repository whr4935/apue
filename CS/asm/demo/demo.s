.text
str:
.string "hello"

.globl main
main:
    xor     %rax, %rax
    addq     $234, %rax
    movl    $str, %edi
    call    puts
    movq    $0xaa55, %rax
    neg     %rax
    call    comp
    call    jumptest
    ret

comp:
    movq    $1, %rdi
    movq    $2, %rsi
    cmpq    %rsi, %rdi
    setl    %al
    movzbl  %al, %eax
    ret

jumptest:
    movq     $0, %rax
    jmp     .L1
    movq   (%rax), %rdx
.L1:
    ret

