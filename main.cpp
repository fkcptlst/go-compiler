#include <antlr4-runtime.h>
#include "Common/Common.h"
#include "ICG/myGoListener.h"
#include "GoParser.h"
#include "GoLexer.h"
#include "GoParserListener.h"
#include "GoParserBaseListener.h"
#include "TCG/Translator.h"

using namespace std;


int main(int argc, char * argv[]){
    system("mkdir -p ../log");
    system("mkdir -p ../output");
    google::InitGoogleLogging((const char *)argv[0]);
    google::SetLogDestination(google::GLOG_INFO, "../log/");
    string filename="calculate.go";
    LOG(INFO) << "start 翻译文件: " << filename;

    antlr4::ANTLRFileStream file;
    file.loadFromFile("../grammar/" + filename);
    antlr4::ANTLRInputStream inputStream(file.toString());
    LOG(INFO) << "start 词法分析";
    GoLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokens(&lexer);
    LOG(INFO) << "start 语法分析";
    GoParser parser(&tokens);
    myGoListener listener;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk( &listener, parser.sourceFile());
    listener.Go23file("../output/3code.txt");

    LOG(INFO) << "start 三地址 -> 汇编";
    std::shared_ptr<TACFile> tac_file = std::make_shared<TACFile>(std::move(listener.TACBlocks));
    Translator translator(std::shared_ptr<TACFile>(tac_file), listener.globalScope);
    translator.Translate();
    std::cout << "---" << std::endl;
    translator.OutputFile("../output/3code.asm");
    return 0;
}
