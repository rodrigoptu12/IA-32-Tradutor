SECTION TEXT
INPUT qtd_termos_fibonacci
copy ONE,arr
copy ONE,arr+1
load qtd_termos_fibonacci
jmpn parar
jmpz parar
fibonacci:
load arr
add arr+1
store arr+2
load arr+1
store arr
load arr+2
store arr+1
OUTPUT arr+2
load qtd_termos_fibonacci
sub ONE
store qtd_termos_fibonacci
jmpp fibonacci
parar:
STOP
SECTION DATA
arr: SPACE 3
ONE: CONST 0x1
qtd_termos_fibonacci: SPACE

