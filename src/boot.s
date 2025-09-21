; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
MBFLAGS  equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + MBFLAGS) ; checksum of above, to prove we are multiboot
                                ; CHECKSUM + MAGIC + MBFLAGS should be Zero (0)

section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

section .bss
    align 16
stack_bottom:
    resb 16384
stack_top:

section .text
    global _start

_start:
    cli                         ; désactiver interruptions
    lgdt [gdt_descriptor]       ; charger la GDT (descripteur = 6 octets)
    mov eax, cr0
    or eax, 1
    mov cr0, eax                ; activer PE = 1 dans CR0

    ; Far jump pour recharger CS et synchroniser le pipeline
    jmp 0x08:protected_mode     ; 0x08 = sélecteur code dans la GDT

; https://wiki.osdev.org/GDT_Tutorial
align 8
gdt_start:
    dq 0x0000000000000000       ; null descriptor

    dw 0xFFFF                   ; limit low
    dw 0x0000                   ; base low
    db 0x00                     ; base middle
    db 10011010b                ; access: present=1, ring0, code, executable, readable
    db 11001111b                ; granularity: 4K, 32-bit, limit high (magic number)
    db 0x00                     ; base high

    ; data descriptor
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b                ; access: data, writable
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; limit (2 bytes)
    dd gdt_start                ; base (4 bytes)

protected_mode:
    ; set up stack: utiliser l'étiquette stack_top (adresse linéaire)
    mov esp, stack_top

    ; charger segments de données avec sélecteur data (0x10)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; maintenant on est en protected mode 32-bit — on peut appeler kernel_main
    extern kernel_main
    call kernel_main

.hang:
    hlt
    jmp .hang
