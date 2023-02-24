
output_s:
    enter 0,0
    mov esi, [ebp+12] ; pega o endereco do buffer
    mov edi, [ebp+8] ; pega  qtd_bytes
    ; escrever o char no stdout
    mov eax, 4; sys_write
    mov ebx, 1; stdout
    mov ecx, esi ; pointer to buffer
    mov edx, edi ; length of buffer
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
;  ESI = pointer to the buffer to store the string in 
;  Output:
;  EAX = quantidade de bytes lidos/escritos (1)
input_s:
    enter 0,0
    mov esi, [ebp+12] ; pega o endereco do buffer
    mov edi, [ebp+8] ; pega  qtd_bytes
    ; ler um byte do stdin
    mov eax, 3; sys_read
    mov ebx, 0; stdin
    mov ecx, esi ; pointer to buffer
    mov edx, edi ; length of buffer
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