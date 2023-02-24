# sb-trabalho-2

Trabalho Prático 2 - Software Básico

## Grupo
Fernando de Alcantara, 190125586

Rodrigo Pereira Couto, 190116510 

## Compilação
### Compilar utilizando `makefile`
Para compilar o programa, basta executar o comando `make build` na raiz do projeto.

Para compilar o programa em debug, basta executar o comando `make debug` na raiz do projeto. 
A compilação em debug irá mostrar mensagens de todos os passos do programa no terminal.

### Compilar utilizando `g++`
Também é possível compilar o montador diretamente no Linux:

g++ tradutor.cpp src/pre.cpp src/utilitarios.cpp src/tipos.cpp -o tradutor

Caso queira compilar o montador em debug, basta adicionar a flag `-DDEBUG` ao comando acima.

## Execução
### Executar utilizando `makefile`
Para executar o programa, basta executar o comando `make run  file=<file>` na raiz do projeto.

E `<file>` é o caminho do arquivo a ser utilizado (não informar a extensão do arquivo).

### Executar utilizando `g++`
Também é possível rodar o montador diretamente (no Windows, deve executar o arquivo montador.exe):

DEVE ESPECIFICAR A EXTENSÃO DO ARQUIVO(.asm)

`./tradutor <file>.asm`

## Exemplo
### Execução
make build

make run file=fatorial

make teste file=fatorial
### Execução em debug
make debug

make run file=fatorial

make teste file=fatorial
### Execução direta
g++ tradutor.cpp src/pre.cpp src/utilitarios.cpp src/tipos.cpp -o tradutor

DEVE ESPECIFICAR A EXTENSÃO DO ARQUIVO(.asm)

Linux: ./tradutor fatorial.asm

Linux: nasm -f elf32 fatorial.s -o fatorial.o && ld -m elf_i386 fatorial.o -o fatorial && ./fatorial

## Informações
Linux:
- Sistema operacional utilizado: Ubuntu 22.04.1 LTS
- Compilador utilizado: g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
