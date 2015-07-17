section .data
baab db "A1", 0
caab db "B2", 0
daab db "C3", 0
eaab db "D4", 0
section .text
global main
main:
xor esi, esi
extern printr
extern prints
extern print
sub esp, 16
xor edi,edi
mov edi, 0
mov ebp, esp
mov [ebp + 8], edi
xor edi,edi
mov edi, 0
mov ebp, esp
mov [ebp + 12], edi
faab:
mov ebp, esp
mov edi, [ebp + 8]
mov ebp, esp
mov ebx, [ebp + 12]
add edi, ebx
mov ebp, esp
mov [ebp + 8], edi
xor edi,edi
mov edi, 10
mov eax, [ebp + 12]
cmp eax, edi
jz gaab
mov eax, [ebp + 12]
add eax, 1
mov [ebp + 12], eax
jmp faab
gaab:
mov ebp, esp
mov ebx, [ebp + 8]
push dword ebx
call print
pop ebx
haab:
mov ebp, esp
mov edi, [ebp + 12]
xor ebx,ebx
mov ebx, 5
cmp ebx, edi
pushf
pop eax
shr eax, 7
and eax, 1
mov edi, eax
cmp edi, 0
jz iaab
mov ebp, esp
mov ebp, esp
mov edi, [ebp + 8]

mov [ebp - 24], edi
mov ebp, esp
mov edi, [ebp + 8]

mov [ebp - 28], edi
push esi
push edi
push ebx
push ecx
add esi, 0
sub esp, 12
call add
add esp, 8
mov ebp, esp
mov eax, [ebp]
add esp, 4
pop ecx
pop ebx
pop edi
pop esi
mov edi, eax
mov ebp, esp
mov [ebp + 8], edi
mov ebp, esp
mov edi, [ebp + 12]
xor ebx,ebx
mov ebx, 1
sub edi, ebx
mov ebp, esp
mov [ebp + 12], edi
jmp haab
iaab:;
mov ebp, esp
mov edi, [ebp + 8]
push dword edi
call print
pop edi
jaab:mov ebp, esp
mov edi, [ebp + 8]
xor ebx,ebx
mov ebx, 2
mov eax, edi
xor edx, edx
idiv ebx
mov edi, eax
mov ebp, esp
mov [ebp + 8], edi
mov ebp, esp
mov edi, [ebp + 8]
xor ebx,ebx
mov ebx, 100
cmp edi, ebx
pushf
pop eax
shr eax, 7
and eax, 1
mov edi, eax
cmp edi, 0
jz jaab
mov ebp, esp
mov edi, [ebp + 8]
push dword edi
call print
pop edi
mov ebp, esp
mov edi, [ebp + 8]
xor ebx,ebx
mov ebx, 4
mov eax, edi
xor edx, edx
idiv ebx
mov edi, edx
cmp edi, 0
jz kaab
cmp edi, 1
jz laab
cmp edi, 2
jz maab
cmp edi, 3
jz naab

jmp oaab
kaab:
mov ebp, esp
mov eax, baab
mov [ebp + 0], eax
jmp oaab
laab:
mov ebp, esp
mov eax, caab
mov [ebp + 0], eax
jmp oaab
maab:
mov ebp, esp
mov eax, daab
mov [ebp + 0], eax
jmp oaab
naab:
mov ebp, esp
mov eax, eaab
mov [ebp + 0], eax
jmp oaab
oaab:;
mov ebp, esp
mov edi, [ebp + 8]
xor ebx,ebx
mov ebx, 0
cmp edi, ebx
pushf
pop eax
shr eax, 7
and eax, 1
xor eax, 1
mov edi, eax
cmp edi, 0
jz paab
mov ebp, esp
mov edi, [ebp + 8]
xor ebx,ebx
mov ebx, 1
add edi, ebx
mov ebp, esp
mov [ebp + 8], edi
jmp qaab
paab:
qaab:
mov ebp, esp
mov edi, [ebp + 8]
push dword edi
call print
pop edi
mov ebp, esp
mov edi, [ebp + 0]
push dword edi
call prints
pop edi
add esp, 16
mov eax, 1
mov ebx, 0
int 80h
add:
sub esp, 0
mov ebp, esp
mov edi, [ebp + 8]
mov ebp, esp
mov ebx, [ebp + 4]
add edi, ebx
mov ebp, esp
mov [ebp + 12], edi
add esp, 0
ret
