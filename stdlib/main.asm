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

macro push8 value {
  sub rsp, 1
  mov [rsp], value
}

macro push16 value {
  mov ax, value
  push ax
}

macro push32 value {
  mov eax, value
  push eax
}

macro push64 value {
  mov rax, value
  push rax
}

macro funBegin name {
  name#^var_size = 0
}

macro funDefVar name, var_name, size {
  name#^var_size = name#^var_size + size
  name#^#var_name#^pos = name#^var_size
}

macro funEntry name {
name:
  if name#^var_size <> 0
    push rbp
    mov rbp, rsp
    sub rsp, name#^var_size
  end if
}

macro funEnd name {
  if name#^var_size <> 0
    add rsp, name#^var_size
    pop rbp
  end if
  ret
}

macro funCallBegin id {
  n^#id#^param_size = 0
}

macro funCallParam id, value, size {
  push#size value
  n^#id#^param_size = n^#id#^param_size + size
}

macro funCall id, name {
  call name
  if n^#id#^param_size <> 0
    add rsp, n^#id#^param_size
  end if
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

