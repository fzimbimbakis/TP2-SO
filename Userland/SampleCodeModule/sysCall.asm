GLOBAL sysWrite
GLOBAL sysRead
GLOBAL sysTime
GLOBAL sysFree
GLOBAL sysAlloc
GLOBAL sysMemInfo
GLOBAL sysNewP
GLOBAL exit
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

sysWrite:
    pushState
    mov rax, 0
    int 80h
    popState
    ret

sysRead:
	pushState
    mov rax, 1
	int 80h
	popState
    ret

sysNewP:
	pushState
	mov rax, 6
	int 80h
	popState
	ret

exit:
    pushState
    mov rax, 7
    int 80h
    popState ;no deberia llegar a ejecutarse esta linea
    ret

sysTime:
	; No pusheo y popeo el rax. Lo uso para retornar.
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

	mov rax, 2
	int 80h

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
	ret

sysAlloc:
	; No pusheo y popeo el rax. Lo uso para retornar.
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

	mov rax, 3
	int 80h

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
	ret

	sysFree:
	pushState
	mov rax, 4
	int 80h
	popState
	ret

	sysMemInfo:
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
    	mov rax, 5
    	int 80h
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
    	ret