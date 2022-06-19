nasm -f elf32 -o ${1}.o ../${1}/${1}.asm
nasm -f elf32 -o print.o ../example/print.asm
ld -m elf_i386 -o run.bin ${1}.o print.o
./run.bin

# go tool compile -N -l -S hello.go
