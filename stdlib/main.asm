format ELF64 executable 3

SYS_exit = 60
SYS_write = 1
STDOUT = 1

macro mov64 dest, src {
  mov rax, src
  mov dest, rax
}

macro def64 name, value {
name:
  dq value
}

segment readable executable
entry _start

; rdi = the number
print_u64:
mov rcx, rsp
mov rax, rdi
mov rbx, 10

.L:
xor rdx, rdx
div rbx
add dl, '0'
dec rcx
mov [rcx],dl
cmp rax, 0
jnz .L

mov rax, SYS_write
mov rdi, STDOUT
mov rsi, rcx

mov rdx, rsp
sub rdx, rcx

syscall
ret

_start:
call main
mov rax, SYS_exit
xor rdi,rdi
syscall

