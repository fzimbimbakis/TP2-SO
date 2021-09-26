GLOBAL accessClock
SECTION .text
accessClock:
    xor rax,rax
    mov al, dil
    out 70h, al
    in al, 71h
    ret