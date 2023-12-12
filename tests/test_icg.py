from pathlib import Path

from antlr4 import InputStream, CommonTokenStream, ParseTreeWalker

from go_compiler.grammar.generated.GoLexer import GoLexer
from go_compiler.grammar.generated.GoParser import GoParser

from go_compiler.icg.MyGoListener import MyGoListener


def compile_to_3code(src_file_path: str, output_file_path: str):
    with open(src_file_path, 'r') as f:
        program_text = f.read()

    lexer = GoLexer(InputStream(program_text))
    token_stream = CommonTokenStream(lexer)
    parser = GoParser(token_stream)

    listener = MyGoListener()
    walker = ParseTreeWalker()
    walker.walk(listener, parser.sourceFile())

    listener.go_to_3file(output_file_path)


def test_main():
    working_dir = Path.cwd()
    print(f"Working directory: {working_dir}", flush=True)
    assert working_dir.name == 'tests', 'Please run this script from the tests directory'

    out_dir = Path('test_icg_out')
    out_dir.mkdir(exist_ok=True)

    src_files = list(Path('testdata').glob('*.go'))
    assert len(src_files) > 0, 'No testdata found'

    for src_file in src_files:
        out_file = (out_dir / src_file.name).with_suffix('.3code')
        print(f'Compiling {src_file} -> {out_file}')
        compile_to_3code(str(src_file), str(out_file))


if __name__ == '__main__':
    test_main()
