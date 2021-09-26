

GLOBAL _keyHandler

SECTION .text

_keyHandler:
    xor rax,rax
	in al,60h
	ret
