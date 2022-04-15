#include <iostream>
#include <antlr4-runtime.h>
#include <vector>
#include "GoParser.h"
#include "GoLexer.h"
#include "GoParserListener.h"
#include "GoParserBaseListener.h"
#include "myGoListener.h"
using namespace std;


int main(int argc, char * argv[]){
    string filename="calculate.go";
    cout << "start to translate "+filename << endl;

    antlr4::ANTLRFileStream file;
    file.loadFromFile("../grammar/"+filename);

    antlr4::ANTLRInputStream inputStream(file.toString());

    GoLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokens(&lexer);
    GoParser parser(&tokens);
    myGoListener listener;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk( &listener, parser.sourceFile());

    return 0;
    //std::cout << ret.as<LabeledExprVisitor*>()-> << std::endl;
}