.486
.model flat, stdcall
option casemap :none

include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\msvcrt.inc
includelib msvcrt.lib

.data
    caption BYTE "Hello",0
    ask BYTE "Is it bigger than "
	asknum BYTE 0,0,0,0,0,0,0,0,0,0
	chose BYTE "YOU CHOSE "
    chosenum BYTE 0,0,0,0,0,0,0,0,0,0

    a DWORD 0
	b DWORD 100

.code
main PROC

  nextIter:
    mov ebx, b
	dec ebx
	cmp a, ebx
	je found

	mov ebx, a
	add ebx, b
	shr ebx, 1

	invoke crt__itoa, ebx, offset asknum, 10

	push ebx
	invoke MessageBoxA, 0, offset ask, offset caption, MB_YESNO
	pop ebx
	cmp eax, IDYES
	je saidYes
	mov b, ebx
	jmp nextIter
  saidYes:
    mov a, ebx
	jmp nextIter

  found:
    invoke crt__itoa, b, offset chosenum, 10
    invoke MessageBoxA, 0, offset chose, offset caption, MB_OK

    invoke ExitProcess, 0

main ENDP

end main
