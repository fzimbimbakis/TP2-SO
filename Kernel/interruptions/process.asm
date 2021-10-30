
GLOBAL createStackContext
GLOBAL startFirstP
GLOBAL haltP
GLOBAL updateStack
GLOBAL int20

extern alloc
extern getCurrentSP
EXTERN irqDispatcher
extern ncPrintChar
extern ncPrintHex

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

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro


createStackContext:
    push rbp
    mov rbp, rsp

    mov rsp, rdi
    push 0 ; Align
    push 0; SS
    push rdi ; RSP
    push 0x202 ; RFLAGS
    push 0x8 ; CS
    push rsi; RIP
    mov rdi, rdx
    mov rsi, rcx
    mov rdx, r8
    pushState
    mov rax, rsp
    mov rsp, rbp
    pop rbp

    ret

startFirstP:
    mov rdi, 0 ; pasaje de parametro
    	call irqDispatcher
        ;mov rdi, 65; A
        ;call ncPrintChar

    	call getCurrentSP;
    	mov rsp , rax ;; cambio de contexto.
    	;mov rdi, rax
    	;call ncPrintHex
        ;mov rdi, 66; B
        ;call ncPrintChar
    	popState
    	;pop rdi
        ;call ncPrintHex
    	;sti ;;set interrupts
    	iretq

haltP:
	;mov rdi, 43; +
    ;call ncPrintChar
	sti
	hlt
	jmp haltP
	ret

int20:
	int 20h
	ret

updateStack:
     call getCurrentSP  ;cambio contexto
     mov rsp, rax
     popState

     iretq
