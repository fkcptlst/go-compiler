; print.asm
global print 				; 设置print为全局可见
print:
    push ebp
    mov ebp, esp
    mov eax, 4              ; 发起系统调用
    mov ebx, 1              ; ebx表示stdout
    mov ecx, [ebp + 8]      ; 表示字符串的地址
    mov edx, [ebp + 12]     ; 表示字符的个数
    int 0x80                ; int 0x80表示系统调用
    pop ebp
    ret
