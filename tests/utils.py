import os
import subprocess

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
