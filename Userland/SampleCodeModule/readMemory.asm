
extern printRegName
extern printHex

global readDirection
global printRegs



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

readDirection:
	xor rax,rax
	mov rax,[rdi] 
	ret


printRegs:
	pushState
	mov rbx, 0
	mov rcx, rsp
	nextReg:
	push rcx
	mov rdi, rbx
	call printRegName
	pop rcx
	push rcx
	mov rdi, [rcx]
	call printHex
	pop rcx
	add rcx, 8
	inc rbx
	cmp rbx, 15
	jne nextReg
	popState
	ret
