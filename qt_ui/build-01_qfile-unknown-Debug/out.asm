.data:


.text:
global _start
extern myprint

_start:
	push 	 ebp
	mov 	 ebp 	 , 	 esp
	mov 	 eax 	 , 	 1
	push 	 eax
	call 	 param
	add 	 esp 	 , 	 4
	push 	 eax
	call 	 myprint
	add 	 esp 	 , 	 4
	sub 	 esp 	 , 	 4
	sub 	 esp 	 , 	 4
	call 	 ret_1
	add 	 esp 	 , 	 0
	mov 	 ebx 	 , 	 [ ebp - 4 ]
	mov 	 ecx 	 , 	 [ ebp - 8 ]
	push 	 ebx
	call 	 myprint
	add 	 esp 	 , 	 4
	push 	 ecx
	call 	 myprint
	add 	 esp 	 , 	 4
	add 	 esp 	 , 	 8
	pop 	 ebp
	mov 	 eax 	 , 	 1
	mov 	 ebx 	 , 	 0
	int 	 80h

param:
	push 	 ebp
	mov 	 ebp 	 , 	 esp
	push 	 eax
	push 	 ebx
	push 	 ecx
	push 	 edx
	push 	 esi
	mov 	 eax 	 , 	 [ ebp + 8 ]
	mov 	 eax 	 , 	 5
	pop 	 esi
	pop 	 edx
	pop 	 ecx
	pop 	 ebx
	pop 	 eax
	pop 	 ebp
	ret

ret_1:
	push 	 ebp
	mov 	 ebp 	 , 	 esp
	push 	 eax
	push 	 ebx
	push 	 ecx
	push 	 edx
	push 	 esi
	mov 	 eax 	 , 	 1
	mov 	 ebx 	 , 	 5
	mov 	 [ ebp + 12 ] 	 , 	 eax
	mov 	 [ ebp + 8 ] 	 , 	 ebx
	pop 	 esi
	pop 	 edx
	pop 	 ecx
	pop 	 ebx
	pop 	 eax
	pop 	 ebp
	ret


