#include <antlr4-runtime.h>
#include "Common/Common.h"
#include "ICG/myGoListener.h"
#include "GoParser.h"
#include "GoLexer.h"
#include "GoParserListener.h"
#include "GoParserBaseListener.h"
#include "TCG/Translator.h"

using namespace std;


void init_log(const std::string& log_file_name, const std::string& log_path) {
    system(("mkdir -p " + log_path).c_str());
    google::InitGoogleLogging(log_file_name.c_str());
    google::SetLogDestination(google::GLOG_INFO, (log_path + "/").c_str());
    google::SetStderrLogging(google::INFO);
    google::InstallFailureSignalHandler();                  // 捕捉信号
    FLAGS_log_prefix = true;                                // 日志前缀
    FLAGS_colorlogtostderr = true; // 颜色
    FLAGS_alsologtostderr = true;                           // 日志输出到屏幕
    FLAGS_logbufsecs = 0;                                   // 日志缓冲时间
}


struct CmdParam {
    std::vector<std::string>    input;
    std::string                 output = ".";
    std::string                 output_3code = "3code.txt";
    std::string                 output_asm = "out.asm";
    std::string                 output_bin = "out.bin";
    std::string                 log_path = "./log";
    std::string                 log_file_name;
};


CmdParam read_cmd_param(int argc, char* argv[]) {
    CmdParam cmd_param;
    cmd_param.log_file_name = argv[0];
    int i = 1;
    while (i < argc) {
        std::string argv_i = argv[i];
        if (argv_i[0] == '-') {
            // option
            if ((argv_i == "-h") || (argv_i == "--help")) {
                std::cout << "可选参数如下:" << std::endl;
                std::cout << "-h (--help):      帮助" << std::endl;
                std::cout << "-o (--output):    [default: .]            输出文件路径, 末尾不可含/" << std::endl;
                std::cout << "-3 (--3code):     [default: 3code.txt]    输出 3code 文件名/" << std::endl;
                std::cout << "-a (--asm):       [default: out]          输出 asm 文件名" << std::endl;
                std::cout << "-b (--bin):       [default: bin]          输出 bin 文件名" << std::endl;
                std::cout << "-l (--log):       [default: log]          输出 log 目录" << std::endl;
                exit(0);
            }
            if (argv_i.length() == 1) {
                std::cout << "选项错误" << std::endl;
                exit(0);
            } else if (i + 1 == argc) {
                std::cout << "选项 " << argv_i << " 没有给定值" << std::endl;
                exit(0);
            } else {
                // 解析参数
                if ((argv_i == "-o") || (argv_i == "--output")) {
                    cmd_param.output = argv[++i];
                } else if ((argv_i == "-3") || (argv_i == "--3code")) {
                    cmd_param.output_3code = argv[++i];
                } else if ((argv_i == "-a") || (argv_i == "--asm")) {
                    cmd_param.output_asm = argv[++i];
                } else if ((argv_i == "-b") || (argv_i == "--bin")) {
                    cmd_param.output_bin == argv[++i];
                } else if ((argv_i == "-l") || (argv_i == "--log")) {
                    cmd_param.log_path = argv[++i];
                } else {
                    std::cout << "未知参数 " << argv_i << std::endl;
                    exit(0);
                }
            }
        } else {
            // input
            cmd_param.input.emplace_back(argv[i]);
        }
        i++;
    }
    if (cmd_param.input.size() == 0) {
        std::cout << "没有输入的 .go 文件" << std::endl;
        exit(0);
    }
    system(("mkdir -p " + cmd_param.output).c_str());
    return cmd_param;
}


int main(int argc, char * argv[]){

    CmdParam cmd_param = read_cmd_param(argc, argv);
    init_log(cmd_param.log_file_name, cmd_param.log_path);

    // string filename = "calculate.go";
    string filename = cmd_param.input[0];
    LOG(WARNING) << "start 翻译文件: " << filename;

    antlr4::ANTLRFileStream file;
    file.loadFromFile(filename);
    antlr4::ANTLRInputStream inputStream(file.toString());
    LOG(WARNING) << "start 词法分析";
    GoLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokens(&lexer);
    LOG(WARNING) << "start 语法分析";
    GoParser parser(&tokens);
    myGoListener listener;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, parser.sourceFile());
    std::string path_3code = cmd_param.output + "/" + cmd_param.output_3code;
    listener.Go23file(path_3code);
    sleep(1);

    LOG(WARNING) << "start 三地址 -> 汇编";
    std::shared_ptr<TACFile> tac_file = std::make_shared<TACFile>(std::move(listener.TACBlocks));
    Translator translator(std::shared_ptr<TACFile>(tac_file), listener.globalScope);
    translator.Translate();
    std::string path_asm = cmd_param.output + "/" + cmd_param.output_asm;
    translator.OutputFile(path_asm);

    std::string path_bin = cmd_param.output + "/" + cmd_param.output_bin;
    std::string cmd;
    cmd = "nasm -f elf32 -o " + cmd_param.output + "/out.o " + path_asm;
    std::cout << cmd << std::endl; system(cmd.c_str());
    cmd = "nasm -f elf32 -o " + cmd_param.output + "/print.o " + "/usr/local/share/doc/print.asm";
    std::cout << cmd << std::endl; system(cmd.c_str());
    cmd = "ld -m elf_i386 -o " + path_bin + " " + cmd_param.output + "/out.o " + cmd_param.output + "/print.o";
    std::cout << cmd << std::endl; system(cmd.c_str());
    return 0;
}
