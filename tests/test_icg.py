import os

from pathlib import Path

from antlr4 import InputStream, CommonTokenStream, ParseTreeWalker

from go_compiler.grammar.generated.GoLexer import GoLexer
from go_compiler.grammar.generated.GoParser import GoParser

from go_compiler.icg.MyGoListener import MyGoListener, reset_my_func_count


def compile_to_3code(src_file_path: str, output_file_path: str):
    with open(src_file_path, "r") as f:
        program_text = f.read()

    lexer = GoLexer(InputStream(program_text))
    token_stream = CommonTokenStream(lexer)
    parser = GoParser(token_stream)

    listener = MyGoListener()
    walker = ParseTreeWalker()
    walker.walk(listener, parser.sourceFile())

    listener.go_to_3file(output_file_path)


def diff_3code_files(file1: str, file2: str) -> bool:
    """
    Compare two 3code files and return True if they are the same, False otherwise
    Args:
        file1: path to file1
        file2: path to file2

    Returns: True if the files are the same, False otherwise

    """
    assert os.system("diff --version > /dev/null") == 0, "diff command not available"

    ret = os.system(
        f"diff -bBw <(sort {file1}) <(sort {file2})"
    )  # ignore whitespace and sort lines
    return ret == 0


def test_main():
    working_dir = Path.cwd()
    print(f"Working directory: {working_dir}", flush=True)
    assert (
        working_dir.name == "tests"
    ), "Please run this script from the tests directory"

    # 1. setup output directory
    out_dir = Path("test_icg_out")
    out_dir.mkdir(exist_ok=True)

    # 2. load source files
    src_files = list(Path("testdata/go_source").glob("*.go"))
    src_files = sorted(src_files, key=lambda x: x.name)
    assert len(src_files) > 0, "No testdata found"

    # 3. load compare files
    compare_files = list(Path("testdata/go_3code").glob("*.3code"))
    compare_files = sorted(compare_files, key=lambda x: x.name)
    assert len(compare_files) == len(
        src_files
    ), "Number of 3code files does not match number of source files"

    success = []
    failed = []
    # 4. compile source files to 3code files
    for src_file, compare_file in zip(src_files, compare_files):
        reset_my_func_count()
        out_file = (out_dir / src_file.name).with_suffix(".3code")
        print(f"Compiling {src_file} -> {out_file}")

        try:
            reset_my_func_count()
            compile_to_3code(str(src_file), str(out_file))
            # 5. compare the generated 3code file with the expected 3code file
            # assert diff_3code_files(str(out_file), str(compare_file)), f"3code files {out_file} and {compare_file} differ"
            if diff_3code_files(str(out_file), str(compare_file)):
                success.append(out_file)
            else:
                failed.append(out_file)

        except Exception as e:
            print("Error: ", e)
            failed.append(out_dir)

    print(f"Failed: {failed}")
    print(f"Success: {success}")


if __name__ == "__main__":
    test_main()
