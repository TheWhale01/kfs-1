section .text
    global reboot

section .extern
    extern disable_cursor

reboot:
    call disable_cursor
    cli
    jmp 0x0
