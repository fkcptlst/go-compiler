import argparse
import os
import sys
from pathlib import Path

from antlr4 import InputStream, CommonTokenStream, ParseTreeWalker
from loguru import logger

from go_compiler.grammar.generated.GoLexer import GoLexer
from go_compiler.grammar.generated.GoParser import GoParser
from go_compiler.icg.MyGoListener import MyGoListener
from go_compiler.tcg.Translator import Translator


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("file", help="The .go file to compile")
    parser.add_argument("-o", "--output", help="The output file")
    parser.add_argument("-v", "--verbose", help="Verbose output", action="store_true")
    parser.add_argument("--tac", help="output the three address code", action="store_true")
    parser.add_argument("--ld", help="link with myprint", action="store_true")

    args = parser.parse_args()

    if args.verbose:
        logger.remove()
        logger.add(sys.stdout, level="DEBUG")
    else:
        logger.remove()
        logger.add(sys.stdout, level="INFO")

    file_path = Path(args.file)

    if args.output:  # if specified, use the output path
        output_bin_path = Path(args.output)
    else:  # otherwise, use the same path as the source file
        output_bin_path = file_path.with_suffix(".o")

    assert file_path.exists(), f"File {file_path} does not exist"
    assert file_path.suffix == ".go", f"File {file_path} is not a .go file"

    with open(file_path, "r") as f:
        program_text = f.read()

    lexer = GoLexer(InputStream(program_text))
    token_stream = CommonTokenStream(lexer)
    parser = GoParser(token_stream)

    listener = MyGoListener()
    walker = ParseTreeWalker()
    walker.walk(listener, parser.sourceFile())

    if args.tac:
        listener.go_to_3file(str(output_bin_path.with_suffix(".3code")))

    tac_file = listener.TACBlocks
    translator: Translator = Translator(tac_file, listener.globalScope)

    translator.Translate()

    translator.OutputFile(str(output_bin_path.with_suffix(".asm")))

    os.system(f"nasm -f elf32 -o {output_bin_path.with_suffix('.o')} {output_bin_path.with_suffix('.asm')}")

    # link binary with myprint
    if args.ld:
        package_root = Path(__file__).parent.parent
        myprint_asm_path = package_root / "tests/testdata/print.asm"
        os.system(f"nasm -f elf32 -o {myprint_asm_path.with_suffix('.o')} {myprint_asm_path}")
        os.system(f"ld -m elf_i386 -o {output_bin_path} {output_bin_path.with_suffix('.o')} {myprint_asm_path.with_suffix('.o')}")
