# nasm -f elf32 -o out.o ../output/out.asm
# nasm -f elf32 -o print.o ../demo/print.asm
ld -m elf_i386 -o run.bin out.o print.o
./run.bin

# go tool compile -N -l -S hello.go
