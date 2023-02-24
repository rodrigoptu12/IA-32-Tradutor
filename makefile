file=fatorial

all: build run

run:
	./tradutor $(file).asm

build:
	g++ tradutor.cpp src/pre.cpp src/utilitarios.cpp src/tipos.cpp -o tradutor

debug:
	g++ tradutor.cpp src/pre.cpp src/utilitarios.cpp src/tipos.cpp -o tradutor -DDEBUG
	
teste:
	nasm -f elf32 $(file).s -o $(file).o && ld -m elf_i386 $(file).o -o $(file) && ./$(file)