segment	.bss
align	4
global	x:object
x:
	resb	4
segment	.text
align	4
extern	readi
extern	printi
extern	prints
extern	printd
extern	println
extern	printsp
extern	readln
extern	argc
extern	argv
extern	readb
extern	readd
extern	strlen
extern	atoi
extern	atod
extern	itoa
extern	dtoa
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 16
	push	dword 0
	push	dword [ebp-4]
	pop	eax
	mov	[ebp-8], eax
	pop	eax
	mov	[ebp-12], eax
	pop	eax
	mov	[ebp-16], eax
	push	dword 2
	push	dword [esp]
	push	dword $x
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
	push	dword $x
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
	call	println
	push	dword [ebp-4]
	pop	eax
	leave
	ret
