GLOBAL sysWrite
GLOBAL sysRead
GLOBAL sysTime
GLOBAL sysFree
GLOBAL sysAlloc
GLOBAL sysMemInfo
GLOBAL sysNewP
GLOBAL sysExit
GLOBAL sysWait
GLOBAL sysYield
GLOBAL sysBlock
GLOBAL sysSemCreate
GLOBAL sysSemWait
GLOBAL sysSemPost
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

%macro pushStateNoRax 0
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

%macro popStateNoRax 0
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

sysExit:
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
sysWait:
	pushState
    mov rax, 8
    int 80h
    popState
    ret

sysYield:
    pushState
    mov rax, 10
    int 80h
    popState
    ret

sysBlock:
    pushState
    mov rax, 9
    int 80h
    popState
    ret

sysSemCreate:
    pushStateNoRax
    mov rax, 11
    int 80h
    popStateNoRax
    ret

sysSemWait:
    pushStateNoRax
    mov rax, 12
    int 80h
    popStateNoRax
    ret

sysSemPost:
    pushStateNoRax
    mov rax, 13
    int 80h
    popStateNoRax
    ret