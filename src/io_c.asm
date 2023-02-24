
output_c:
    enter 0,0
    mov esi, [ebp+8] ; pega o endereco do buffer
    ; escrever o char no stdout
    mov eax, 4; sys_write
    mov ebx, 1; stdout
    mov ecx, esi ; pointer to buffer
    mov edx, 1 ; length of buffer
    int 0x80; call kernel
    ; calcular o tamanho da string
    push ecx ; push pointer to string
    push esi ; store esi
    call len ; call len -> ecx = length of string
    pop esi ; discard pointer to string
    pop esi ; restore esi

    ; mostrar a quantidade de bytes lidos/escritos
    push eax ; store eax
    push ebx ; store ebx
    push ecx ; store ecx
    push edx ; store edx
    push edi ; store edi
    push esi ; store esi
    push ecx ; push length of buffer
    call show_output_msg ; call show_output_msg
    pop ecx ; restore
    pop esi ; restore
    pop edi ; restore
    pop edx ; restore
    pop ecx ; restore
    pop ebx ; restore
    pop eax ; restore

    ; retornar a quantidade de bytes lidos/escritos em eax
    mov eax, ecx ; return length of string
    leave
    ret

;  Input:
;  ESI = pointer to the buffer to store the char in (must have room for at least 1 byte)
;  Output:
;  EAX = quantidade de bytes lidos/escritos (1)
input_c:
    enter 0,0
    mov esi, [ebp+8] ; pega o endereco do buffer
    ; ler um byte do stdin
    mov eax, 3; sys_read
    mov ebx, 0; stdin
    mov ecx, esi ; pointer to buffer
    mov edx, 1 ; length of buffer
    int 0x80; call kernel
    ; calcular o tamanho da string
    push ecx ; push pointer to string
    push esi ; store esi
    call len ; call len -> ecx = length of string
    pop esi ; discard pointer to string
    pop esi ; restore esi

    ; mostrar a quantidade de bytes lidos/escritos
    push eax ; store eax
    push ebx ; store ebx
    push ecx ; store ecx
    push edx ; store edx
    push edi ; store edi
    push esi ; store esi
    push ecx ; push length of buffer
    call show_output_msg ; call show_output_msg 
    pop ecx ; restore
    pop esi ; restore
    pop edi ; restore
    pop edx ; restore
    pop ecx ; restore
    pop ebx ; restore
    pop eax ; restore

    ; retornar a quantidade de bytes lidos/escritos em eax
    mov eax, ecx ; return length of string
    leave
    ret


; Input:
; ESI = pointer to the string to get the length of
; Output:
; ECX = length of the string
len:
    enter 0,0 ; create stack frame
    mov esi, [ebp+8] ; get pointer to string
    mov ecx,0 ; initialize counter
count:
    inc ecx ; increment counter
    inc esi ; increment esi to point to next char
    cmp byte [esi],0 ; check if char is null
    jnz count ; if not null, jump to count

    ;dec ecx ; decrement counter to get length
    leave ; remove stack frame
    ret ; return to caller

; Input:
; first value in stack +12 = integer value to convert
; second value in stack +8 = pointer to buffer to store the string in (must have room for at least 10 bytes)
; Output:
; eax = pointer to the first character of the generated string
; ecx = length of the generated string 
int_to_string:
    enter 0,0 ; create stack frame
    mov eax, [ebp+12] ; push eax
    mov esi, [ebp+8] ; push esi
    add esi,9        ; Point to the end of the buffer
    mov byte [esi],0    ; String terminator

    mov ecx,0 ; initialize counter
    mov ebx,10 ; set base to 10
.next_digit:
    inc ecx ; increment counter
    xor edx,edx         ; Clear edx prior to dividing edx:eax by ebx
    div ebx             ; eax /= 10
    add dl,'0'          ; Convert the remainder to ASCII 
    dec esi             ; store characters in reverse order
    mov [esi],dl
    test eax,eax            
    jnz .next_digit     ; Repeat until eax==0

    ; return a pointer to the first digit (not necessarily the start of the provided buffer)
    mov eax,esi
    leave ; remove stack frame
    ret

show_output_msg:
    enter 0,0 ; create stack frame
    mov edi, [ebp+8] ; get pointer to integer value to print
    mov esi, buffer_number ; pointer to buffer to store the string in
    ; show output message
    mov eax, 4; sys_write
    mov ebx, 1; stdout
    mov ecx, output_msg ; pointer to string
    mov edx, len_output_msg ; length of string
    int 0x80; call kernel
    ; converter o inteiro para string
    push edi ; push integer value
    push esi ; push pointer to buffer
    call int_to_string ; call int_to_string -> eax = pointer to string, ecx = length of string
    pop esi ; restore pointer to buffer
    pop edi ; restore integer value
    ; escrever a string no stdout
    push eax ; push pointer to string
    push ecx ; push length of string
    mov eax, 4; sys_write
    mov ebx, 1; stdout
    pop edx ; pop length of string
    pop ecx ; pop pointer to string
    int 0x80; call kernel
    ; show new line
    mov eax, 4; sys_write
    mov ebx, 1; stdout
    mov ecx, new_line ; pointer to string
    mov edx, 1 ; length of string
    int 0x80; call kernel
    leave ; remove stack frame
    ret ; return to caller