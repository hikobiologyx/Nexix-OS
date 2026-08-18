; Nexix OS Bootloader - Stage 1
; Simple bootloader that displays welcome message

[bits 16]
[org 0x7c00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    ; Set video mode to 80x25 text mode
    mov ax, 0x0003
    int 0x10

    ; Clear screen
    mov ax, 0x0600
    mov bx, 0x0007
    mov cx, 0x0000
    mov dx, 0x184f
    int 0x10

    ; Print welcome message
    mov si, welcome_msg
    call print_string

    ; Print loading message
    mov si, loading_msg
    call print_string

    ; Load kernel from disk
    mov ah, 0x02      ; BIOS read sectors function
    mov al, 0x10      ; Read 16 sectors (8KB for kernel)
    mov ch, 0x00      ; Cylinder 0
    mov cl, 0x02      ; Start from sector 2 (sector 1 is boot sector)
    mov dh, 0x00      ; Head 0
    mov dl, 0x00      ; Drive 0 (boot drive)
    mov bx, 0x1000    ; Load kernel to 0x1000:0x0000 (physical address 0x10000)
    int 0x13

    jc disk_error     ; Jump if carry flag set (error occurred)

    ; Verify load successful
    mov si, success_msg
    call print_string

    ; Jump to kernel
    jmp 0x0000:0x1000

disk_error:
    mov si, error_msg
    call print_string
    jmp $             ; Infinite loop on error

print_string:
    pusha
.print_loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    mov bh, 0x00
    mov bl, 0x0f      ; White color
    int 0x10
    jmp .print_loop
.done:
    popa
    ret

welcome_msg db '====================================', 13, 10
            db '   NEXIX OPERATING SYSTEM v1.0', 13, 10
            db '====================================', 13, 10, 0

loading_msg db 13, 10, 'Loading kernel...', 13, 10, 0
success_msg db 'Kernel loaded successfully!', 13, 10, 13, 10, 0
error_msg   db 'DISK READ ERROR!', 13, 10, 'System halted.', 13, 10, 0

times 510-($-$$) db 0
dw 0xaa55
