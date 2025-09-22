global gdt_flush

gdt_flush:
	mov eax, [esp + 4]    ; Get the pointer to our new GDT
	lgdt [eax]           ; Load the new GDT
	mov eax, 0x10        ; Data segment selector
	mov ds, ax          ; Update the data segment
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:.flush      ; Jump to code segment selector
.flush:
	ret