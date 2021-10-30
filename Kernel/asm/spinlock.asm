GLOBAL acquire
GLOBAL release

SECTION .text
acquire:
    push rax
    check:
        mov rax, 1
        xchg rax, qword[rdi]
        cmp rax, 0
        jne check
    pop rax
    ret
release:
    mov qword[rdi], 0
    ret