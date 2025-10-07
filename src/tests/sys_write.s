section     .rodata
    hello_str   db "allo JULO !", 0x0A, 0

global test_sys_write:
test_sys_write:
    mov eax, 0 ; write syscall
    mov ebx, 1 ; fd
    mov ecx, hello_str ; buf
    mov edx, 12 ; size
    int 0x80
    ret
