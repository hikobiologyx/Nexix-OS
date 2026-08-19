; Nexix OS Kernel - Multiboot Compliant
; Simple kernel that displays system info and runs a basic loop

; Multiboot header must be in the first 8KB
ALIGN 4096

MULTIBOOT_HEADER:
    dd 0x1BADB002          ; Magic number
    dd 0x00000000          ; Flags
    dd -(0x1BADB002 + 0x00000000) ; Checksum

[bits 32]

kernel_start:
    cli
    xor eax, eax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x18000
    sti

    ; Set video mode to 80x25 text mode (BIOS call requires real mode)
    ; For now, we'll use VGA memory directly
    
    ; Clear screen using VGA memory at 0xB8000
    mov edi, 0xB8000
    mov ecx, 2000         ; 80x25 = 2000 character cells
    mov ax, 0x0720        ; Space with white on black
.clear_loop:
    mov [edi], ax
    add edi, 2
    dec ecx
    jnz .clear_loop

    ; Print kernel banner at center of screen
    mov si, kernel_banner
    mov edi, 0xB8000 + 1600  ; Row 10 (160 bytes per row * 10 = 1600)
    call print_string_32

    ; Print CPU info
    mov si, cpu_msg
    mov edi, 0xB8000 + 1920  ; Row 12
    call print_string_32

    ; Print memory info
    mov si, memory_msg
    mov edi, 0xB8000 + 2080  ; Row 13
    call print_string_32

    ; Print status
    mov si, status_msg
    mov edi, 0xB8000 + 2400  ; Row 15
    call print_string_32

    ; Main kernel loop
.kernel_loop:
    hlt
    jmp .kernel_loop

print_string_32:
    pusha
.print_loop:
    lodsb
    or al, al
    jz .done
    mov [di], al
    mov byte [di+1], 0x07  ; White on black
    add di, 2
    jmp .print_loop
.done:
    popa
    ret

kernel_banner db '====================================', 0
              db '      NEXIX KERNEL INITIALIZED      ', 0
              db '====================================', 0

cpu_msg         db 'CPU: x86 Protected Mode Active', 0
memory_msg      db 'Memory: 32-bit Addressing Enabled', 0
status_msg      db 'Status: System Running...', 0

times 8192-($-$$) db 0
