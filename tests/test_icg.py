import os
import subprocess
from pathlib import Path

from antlr4 import InputStream, CommonTokenStream, ParseTreeWalker

from go_compiler.grammar.generated.GoLexer import GoLexer
from go_compiler.grammar.generated.GoParser import GoParser
from go_compiler.icg.MyGoListener import MyGoListener


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


def get_src_tgt_pairs():
    """
    Util function to get source and target files for testing.
    Returns:
        src_tgt_pairs_dict:
            {
               "src_file_name_1":
                     {
                         "src": "source file path",
                         "tgt": "target file path"
                     },
                     ...
            }
    """
    working_dir = Path.cwd()
    tests_dir = working_dir / "tests"
    assert tests_dir.exists(), (f"tests directory {tests_dir} does not exist, "
                                f"make sure you are running pytest from project root")

    # 1. setup output directory
    out_dir = tests_dir / Path("test_icg_out")
    out_dir.mkdir(exist_ok=True)

    src_tgt_pairs_dict = {}

    # 2. load source files
    src_files = sorted(list(Path("tests/testdata/go_source").glob("*.go")), key=lambda x: x.name)
    assert len(src_files) > 0, "No testdata found"

    # 3. load compare files
    compare_files = sorted(list(Path("tests/testdata/go_3code").glob("*.3code")), key=lambda x: x.name)
    assert len(compare_files) == len(
        src_files
    ), "Number of 3code files does not match number of source files"

    for src_file, compare_file in zip(src_files, compare_files):
        src_tgt_pairs_dict[src_file.name] = {
            "src": src_file,
            "tgt": compare_file
        }

    return src_tgt_pairs_dict


src_tgt_pairs_dict = get_src_tgt_pairs()


###############################################################################################################
# TEST CASES                                                                                                  #
###############################################################################################################

def test_01assign():
    src_file = src_tgt_pairs_dict["01assign.go"]["src"]
    tgt_file = src_tgt_pairs_dict["01assign.go"]["tgt"]
    out_file = Path("tests/test_icg_out/01assign.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_02calculate():
    src_file = src_tgt_pairs_dict["02calculate.go"]["src"]
    tgt_file = src_tgt_pairs_dict["02calculate.go"]["tgt"]
    out_file = Path("tests/test_icg_out/02calculate.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_03if():
    src_file = src_tgt_pairs_dict["03if.go"]["src"]
    tgt_file = src_tgt_pairs_dict["03if.go"]["tgt"]
    out_file = Path("tests/test_icg_out/03if.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_04for():
    src_file = src_tgt_pairs_dict["04for.go"]["src"]
    tgt_file = src_tgt_pairs_dict["04for.go"]["tgt"]
    out_file = Path("tests/test_icg_out/04for.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_05func():
    src_file = src_tgt_pairs_dict["05func.go"]["src"]
    tgt_file = src_tgt_pairs_dict["05func.go"]["tgt"]
    out_file = Path("tests/test_icg_out/05func.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_06array():
    src_file = src_tgt_pairs_dict["06array.go"]["src"]
    tgt_file = src_tgt_pairs_dict["06array.go"]["tgt"]
    out_file = Path("tests/test_icg_out/06array.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"
