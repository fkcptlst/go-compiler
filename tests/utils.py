import os
import subprocess
from pathlib import Path

from antlr4 import InputStream, CommonTokenStream, ParseTreeWalker

from go_compiler.grammar.generated.GoLexer import GoLexer
from go_compiler.grammar.generated.GoParser import GoParser
from go_compiler.icg.MyGoListener import MyGoListener
from go_compiler.tcg.Translator import Translator


def compile_to_3code(src_file_path: str, output_file_path: str):
    """
    Compile a Go source file to 3code and write the output to a file.
    Args:
        src_file_path: path to source file
        output_file_path: path to output file

    Returns:

    """
    with open(src_file_path, "r") as f:
        program_text = f.read()

    lexer = GoLexer(InputStream(program_text))
    token_stream = CommonTokenStream(lexer)
    parser = GoParser(token_stream)

    listener = MyGoListener()
    walker = ParseTreeWalker()
    walker.walk(listener, parser.sourceFile())

    listener.go_to_3file(output_file_path)


def compile_to_asm(src_file_path: str, output_file_path: str):
    with open(src_file_path, "r") as f:
        program_text = f.read()

    lexer = GoLexer(InputStream(program_text))
    token_stream = CommonTokenStream(lexer)
    parser = GoParser(token_stream)

    listener = MyGoListener()
    walker = ParseTreeWalker()
    walker.walk(listener, parser.sourceFile())

    # listener.go_to_3file(output_file_path)

    tac_file = listener.TACBlocks
    translator: Translator = Translator(tac_file, listener.globalScope)

    translator.Translate()
    translator.OutputFile(output_file_path)


def diff_3code_files(file1: str, file2: str) -> bool:
    """
    Compare two 3code files and return True if they are the same, False otherwise
    Args:
        file1: path to file1
        file2: path to file2

    Returns: True if the files are the same, False otherwise

    """
    # check if files exist
    assert os.path.exists(file1), f"file {file1} does not exist"
    assert os.path.exists(file2), f"file {file2} does not exist"

    # assert diff command is available
    assert os.system("bash -c 'diff --version > /dev/null'") == 0, "diff command not available"

    ret = subprocess.run(
        f"bash -c 'diff -bBw <(sort {file1}) <(sort {file2})'", shell=True, capture_output=True
    )  # ignore whitespace and sort lines

    if ret.returncode == 0:
        return True
    else:
        print(ret.stdout.decode("utf-8"))
        print(ret.stderr.decode("utf-8"))
        return False


def compile_go_files():
    """
    Compile the go files using official go compiler as ground truth
    """
    assert os.system(
        "scripts/compile_go_testcases.sh tests/testdata/go_source") == 0, "error while executing compile_go_files"


def compile_myprint():
    """
    print.asm -> print.o
    """
    tmp_dir = Path("tmp")
    tmp_dir.mkdir(exist_ok=True, parents=True)
    assert os.system(
        f"nasm -f elf32 -o {tmp_dir}/print.o tests/testdata/print.asm") == 0, "error while executing compile_myprint"


def asm_to_bin():
    asm_files = sorted(list(Path("tmp/test_tcg_out").glob("*.asm")), key=lambda x: x.name)
    assert len(asm_files) > 0, "No testdata found"

    output_dir = Path("tmp/test_tcg_out_bin")
    output_dir.mkdir(exist_ok=True, parents=True)

    for asm_file in asm_files:
        assert os.system(
            f"nasm -f elf32 -o {output_dir}/{asm_file.stem}.o {asm_file}") == 0, f"error while asm_to_bin: {asm_file}"


def ld_single_with_myprint(file_name: str):
    """
    ld -m elf_i386 -o test test.o print.o

    Args:
        file_name: file_name without suffix .o
    """
    assert os.system(
        f'ld -m elf_i386 -o {file_name} tmp/test_tcg_out_bin/{file_name}.o tmp/print.o') == 0, f"error while ld_single_with_myprint({file_name})"


def ld_with_myprint():
    bin_files = sorted(list(Path("tmp/test_tcg_out_bin").glob("*.o")), key=lambda x: x.name)
    assert len(bin_files) > 0, "No testdata found"

    output_dir = Path("tmp/test_tcg_out_bin")
    output_dir.mkdir(exist_ok=True, parents=True)

    for bin_file in bin_files:
        ld_single_with_myprint(bin_file.stem)


def generate_asm_codes(input_dir: Path, output_dir: Path):
    """
    Generate asm codes using implemented compiler
    """
    src_files = sorted(list(input_dir.glob("*.go")), key=lambda x: x.name)
    assert len(src_files) > 0, "No testdata found"

    output_dir.mkdir(exist_ok=True, parents=True)

    for src_file in src_files:
        out_file = (output_dir / src_file.name).with_suffix(".asm")
        compile_to_asm(str(src_file), str(out_file))


def run_single_test(name: str):
    """

    Args:
        name: the name of the go file without .go suffix
    """
    print(f"cwd: {os.getcwd()}")
    print(f"files under 'tmp': {list(os.walk("tmp"))}")

    ret_gt = subprocess.run(
        f"./tmp/gt/{name}"
    )

    ret_test = subprocess.run(
        f"./tmp/test_tcg_out_bin/{name}"
    )

    assert str(ret_gt.stdout) == str(ret_test.stdout), f"Not passing {name}"
