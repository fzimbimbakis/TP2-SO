
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL _softwareHandler
GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

EXTERN printEOE
EXTERN waiting
EXTERN getStackBase
EXTERN ncClear

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN int_80
EXTERN printRegName
EXTERN ncPrintHex
EXTERN ncNewline
SECTION .text

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

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	; Imprimo info del error y ubicación
	mov rdi, %1 ; pasaje de parametro
	mov rsi, [rsp+120]		; Le paso la direc de retorno (Instrucción de la exc.).
	call exceptionDispatcher

	;	Imprimo los registros, uso el stack.
		call printRegs
	
	popState

	;	Espero una tecla del usuario y luego reinicio el kernel.
	call printEOE
	mov rdi, 0xFE
	call picMasterMask
	sti			
	mov rdi, 10
	call waiting
	mov rdi, 0xFD
	call picMasterMask
	pop rax
	call ncClear
	push 0x400000		; Vuelvo a la direccion de memoria de reinicio de shell
	iretq
%endmacro


printRegs:
	mov rbx, 0
	mov rcx, rsp
	add rcx, 8
	nextReg:
	push rcx
	mov rdi, rbx
	call printRegName
	pop rcx
	push rcx
	mov rdi, [rcx]
	call ncPrintHex
	pop rcx
	add rcx, 8
	inc rbx
	cmp rbx, 15
	jne nextReg
	ret


_hlt:
	sti
	hlt
	cli
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


_softwareHandler:
	; No pusheo ni popeo el rax. Por eso no usa pushState y popState
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
	mov rcx, rax
	call int_80
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
	iretq
	
;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1