GLOBAL sysWrite
GLOBAL sysRead
GLOBAL sysTime
GLOBAL sysFree
GLOBAL sysYield
GLOBAL sysBlock
GLOBAL sysUnblock
GLOBAL sysNice
GLOBAL sysAlloc
GLOBAL sysGetpid
GLOBAL sysPs
GLOBAL sysMemInfo
GLOBAL sysNewP
GLOBAL sysExit
GLOBAL sysSemCreate
GLOBAL sysSemWait
GLOBAL sysSemPost
GLOBAL sysSemClose
GLOBAL sysSemInfo
GLOBAL sysSleep
GLOBAL sysKill
GLOBAL sysPipe
GLOBAL sysDupPipe
GLOBAL sysClosePipe
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

%macro pushStateNoRAX 0
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

%macro popStateNoRAX 0
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

sysPipe:
    pushStateNoRax
    mov rax, 20
    int 80h
    popStateNoRax
    ret

    sysDupPipe:
        pushState
        mov rax, 19
        int 80h
        popState
        ret

sysClosePipe:
    pushStateNoRax
    mov rax, 23
    int 80h
    popStateNoRax
    ret

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
	pushStateNoRAX
	mov rax, 6
	int 80h
	popStateNoRAX
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
    sysSemCreate:
        pushStateNoRax
        mov rax, 14
        int 80h
        popStateNoRax
        ret

    sysSemWait:
        pushStateNoRax
        mov rax, 15
        int 80h
        popStateNoRax
        ret

    sysSemPost:
        pushStateNoRax
        mov rax, 16
        int 80h
        popStateNoRax
        ret

    sysSemClose:
        pushStateNoRax
        mov rax, 17
        int 80h
        popStateNoRax
        ret

        sysSemInfo:
            pushStateNoRax
            mov rax, 18
            int 80h
            popStateNoRax
            ret

sysYield:
	pushState
    mov rax, 9
    int 80h
    popState
    ret

sysSleep:
	pushState
    mov rax, 22
    int 80h
    popState
    ret

sysBlock:
	pushStateNoRAX
    mov rax, 8
    int 80h
    popStateNoRAX
    ret

sysKill:
	pushStateNoRAX
    mov rax, 10
    int 80h
    popStateNoRAX
    ret

sysPs:
	pushState
    mov rax, 11
    int 80h
    popState
    ret

sysGetpid:
	pushStateNoRAX
    mov rax, 12
    int 80h
    popStateNoRAX
    ret

sysNice:
	pushStateNoRAX
    mov rax, 13
    int 80h
    popStateNoRAX
    ret

sysUnblock:
	pushStateNoRAX
    mov rax, 21
    int 80h
    popStateNoRAX
    ret