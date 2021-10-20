
GLOBAL _createStack
GLOBAL startFirstP

extern alloc
section .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro



_createStack:
    push rbp
    mov rbp, rsp

    mov rsp, rdi
    push 0 ; Align
    push 0; SS
    push rdi ; RSP
    push 0x202 ; RFLAGS
    push 0x8 ; CS
    push rsi; RIP
     
    pushState
    mov rax, rsp
    mov rsp, rbp
    pop rbp

    ret

startFirstP:
    mov rsp, rdi
    popState

    ;iretq
