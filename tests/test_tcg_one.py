from pathlib import Path

from loguru import logger

from utils import compile_to_asm


def test_main():
    working_dir = Path.cwd()

    tests_dir = working_dir / "tests"
    assert tests_dir.exists(), (
        f"tests directory {tests_dir} does not exist, "
        f"make sure you are running pytest from project root"
    )

    # 1. setup output directory
    out_dir = tests_dir / Path("test_icg_out")
    out_dir.mkdir(exist_ok=True)

    # 2. load source files
    src_files = list(Path("tests/testdata/go_source").glob("*.go"))
    src_files = sorted(src_files, key=lambda x: x.name)
    assert len(src_files) > 0, "No testdata found"

    success = []
    failed = []

    src_file = src_files[5]
    out_file = (out_dir / src_file.name).with_suffix(".asm")
    logger.info(f"Compiling {src_file} -> {out_file}")

    compile_to_asm(str(src_file), str(out_file))

    logger.info(f"Failed: {failed}")
    logger.info(f"Success: {success}")


if __name__ == "__main__":
    test_main()
