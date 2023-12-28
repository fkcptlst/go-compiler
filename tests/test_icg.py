from pathlib import Path
from utils import diff_3code_files, compile_to_3code


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
    out_dir = Path("tmp/test_icg_out")
    out_dir.mkdir(exist_ok=True, parents=True)

    src_tgt_pairs_dict_ = {}

    # 2. load source files
    src_files = sorted(list(Path("tests/testdata/go_source").glob("*.go")), key=lambda x: x.name)
    assert len(src_files) > 0, "No testdata found"

    # 3. load compare files
    compare_files = sorted(list(Path("tests/testdata/go_3code").glob("*.3code")), key=lambda x: x.name)
    assert len(compare_files) == len(
        src_files
    ), "Number of 3code files does not match number of source files"

    for src_file, compare_file in zip(src_files, compare_files):
        src_tgt_pairs_dict_[src_file.name] = {
            "src": src_file,
            "tgt": compare_file
        }

    return src_tgt_pairs_dict_


src_tgt_pairs_dict = get_src_tgt_pairs()


###############################################################################################################
# TEST CASES                                                                                                  #
###############################################################################################################

def test_01assign():
    src_file = src_tgt_pairs_dict["01assign.go"]["src"]
    tgt_file = src_tgt_pairs_dict["01assign.go"]["tgt"]
    out_file = Path("tmp/test_icg_out/01assign.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_02calculate():
    src_file = src_tgt_pairs_dict["02calculate.go"]["src"]
    tgt_file = src_tgt_pairs_dict["02calculate.go"]["tgt"]
    out_file = Path("tmp/test_icg_out/02calculate.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_03if():
    src_file = src_tgt_pairs_dict["03if.go"]["src"]
    tgt_file = src_tgt_pairs_dict["03if.go"]["tgt"]
    out_file = Path("tmp/test_icg_out/03if.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_04for():
    src_file = src_tgt_pairs_dict["04for.go"]["src"]
    tgt_file = src_tgt_pairs_dict["04for.go"]["tgt"]
    out_file = Path("tmp/test_icg_out/04for.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_05func():
    src_file = src_tgt_pairs_dict["05func.go"]["src"]
    tgt_file = src_tgt_pairs_dict["05func.go"]["tgt"]
    out_file = Path("tmp/test_icg_out/05func.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"


def test_06array():
    src_file = src_tgt_pairs_dict["06array.go"]["src"]
    tgt_file = src_tgt_pairs_dict["06array.go"]["tgt"]
    out_file = Path("tmp/test_icg_out/06array.go").with_suffix(".3code")
    compile_to_3code(str(src_file), str(out_file))
    assert diff_3code_files(str(out_file), str(tgt_file)), f"3code files {out_file} and {tgt_file} differ"
