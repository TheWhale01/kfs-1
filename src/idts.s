global load_idt
load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    sti
    ret

%macro ISR_NOERRORCODE 1
    global isr%1
    isr%1:
        cli
        push long 0
        push long %1
        jmp isr_common_stub
%endmacro

%macro ISR_ERRORCODE 1
    global isr%1
    isr%1:
        cli
        push long %1
        jmp isr_common_stub
%endmacro

%include "./src/isrs.s"

extern isr_handler
isr_common_stub:
    pusha           ; edi, esi, ebp, esp, ebx, edx, ecx,  eax
    mov eax, ds
    push eax
    mov eax, cr2
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp    ; pass a pointer to all the registers we pushed onto the stack
    call isr_handler

    add esp, 8
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8
    sti
    iret
