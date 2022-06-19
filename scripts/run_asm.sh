nasm -f elf32 -o output.o ../output/output.asm
nasm -f elf32 -o print.o ../example/print.asm
ld -m elf_i386 -o run.bin output.o print.o
./run.bin

# go tool compile -N -l -S hello.go
