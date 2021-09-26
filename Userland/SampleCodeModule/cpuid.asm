GLOBAL cpuFun
EXTERN cpuidNotValid
EXTERN processorVersionInfo
EXTERN processorExtendedFeatures
EXTERN pushState
EXTERN popState
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
cpuFun:
    push rbp
    mov rbp, rsp
    pushState

    pushfq
    pop rax
    or eax, 00200000h
    jne cpuidIsValid
    call cpuidNotValid
    jmp fin
cpuidIsValid:
    mov rax, 1      ; Version info
    cpuid

    mov edi, ebx
    mov esi, ecx
    ;mov edx, edx
    call processorVersionInfo
    mov eax, 07h
    mov ecx, 0h     ; Extended Features with ECX=0.
    cpuid
    mov edi, ebx
    mov esi, ecx
    ;mov edx, edx
    call processorExtendedFeatures

    fin:
    popState

    mov rsp, rbp
    pop rbp
    ret