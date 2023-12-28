from pathlib import Path

from utils import run_single_test, compile_go_files, generate_asm_codes, compile_myprint, ld_with_myprint, asm_to_bin

# 1. compile go files to binary using official compiler
compile_go_files()

# 2. compile go files to asm using implemented compiler
generate_asm_codes(Path("tests/testdata/go_source"), Path("tmp/test_tcg_out"))

# 3. print.asm -> bin
compile_myprint()

# 4. generated asm -> bin
asm_to_bin()

# 5. link generated.o with print.o
ld_with_myprint()


###############################################################################################################
# TEST CASES                                                                                                  #
###############################################################################################################

def test_01assign():
    run_single_test("01assign")


def test_02calculate():
    run_single_test("02calculate")


def test_03if():
    run_single_test("03if")


def test_04for():
    run_single_test("04for")


def test_05func():
    run_single_test("05func")


def test_06array():
    run_single_test("06array")
