.code 

DirectIO PROC

	mov r10, rcx
	mov eax, 7
	syscall
	ret

DirectIO ENDP

END