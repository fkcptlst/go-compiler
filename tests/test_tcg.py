from pathlib import Path

from antlr4 import InputStream, CommonTokenStream, ParseTreeWalker

from go_compiler.grammar.generated.GoLexer import GoLexer
from go_compiler.grammar.generated.GoParser import GoParser

from go_compiler.icg.MyGoListener import MyGoListener, reset_my_func_count
from go_compiler.tcg.Translator import Translator

from go_compiler.logger.logger import logger


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


def test_main():
    working_dir = Path.cwd()
    logger.info(f"Working directory: {working_dir}")
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

    success = []
    failed = []
    # 4. compile source files to 3code files
    for src_file in src_files:
        reset_my_func_count()
        out_file = (out_dir / src_file.name).with_suffix(".asm")
        logger.info(f"Compiling {src_file} -> {out_file}")
        reset_my_func_count()
        compile_to_asm(str(src_file), str(out_file))

        # 5. compare the generated 3code file with the expected 3code file
        # assert diff_3code_files(str(out_file), str(compare_file)), f"3code files {out_file} and {compare_file} differ"

        # try:
        #     reset_my_func_count()
        #     compile_to_asm(str(src_file), str(out_file))
        #     # 5. compare the generated 3code file with the expected 3code file
        #     # assert diff_3code_files(str(out_file), str(compare_file)), f"3code files {out_file} and {compare_file} differ"
        #     success.append(out_file)
        #
        # except Exception as e:
        #     logger.info("Error: ", e)
        #     failed.append(out_dir)

    logger.info(f"Failed: {failed}")
    logger.info(f"Success: {success}")


if __name__ == "__main__":
    test_main()
