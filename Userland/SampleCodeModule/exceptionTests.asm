GLOBAL divTest
GLOBAL opCodeTest

divTest:
    mov rax, 0
    div ax
    ret

opCodeTest:
    UD2
    ret