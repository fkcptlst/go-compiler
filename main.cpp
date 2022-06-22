#include <antlr4-runtime.h>
#include "Common/Common.h"
#include "ICG/myGoListener.h"
#include "GoParser.h"
#include "GoLexer.h"
#include "GoParserListener.h"
#include "GoParserBaseListener.h"
#include "TCG/Translator.h"

using namespace std;


void init_log(const char* log_file_name) {
    system("mkdir -p ../log");
    google::InitGoogleLogging(log_file_name);
    google::SetLogDestination(google::GLOG_INFO, "../log/");
    google::SetStderrLogging(google::INFO);
    google::InstallFailureSignalHandler();                  // 捕捉信号
    FLAGS_log_prefix = true;                                // 日志前缀
    FLAGS_colorlogtostderr = true; // 颜色
    FLAGS_alsologtostderr = true;                           // 日志输出到屏幕
    FLAGS_logbufsecs = 0;                                   // 日志缓冲时间
}


int main(int argc, char * argv[]){
    system("mkdir -p ../output");

    init_log(argv[0]);

    // string filename = "calculate.go";
    string filename = argv[1];
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
    listener.Go23file("../output/3code.txt");
    sleep(1);
    LOG(WARNING) << "start 三地址 -> 汇编";
    std::shared_ptr<TACFile> tac_file = std::make_shared<TACFile>(std::move(listener.TACBlocks));
    Translator translator(std::shared_ptr<TACFile>(tac_file), listener.globalScope);
    translator.Translate();
    translator.OutputFile("../output/output.asm");
    return 0;
}
