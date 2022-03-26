#include "JsonLexer.h"
#include "JsonParser.h"
#include "translator.h"
#include <iostream>


using namespace std;
using namespace antlr4;


int main (int argc, char* argv[]) {
    string filename = (argc == 1) ? "1.json" : argv[1];
    cout << "start to translate " << filename << endl;

    ANTLRFileStream file;
    file.loadFromFile("../../assets/json/" + filename);

    ANTLRInputStream input_stream(file.toString());

    JsonLexer lexer(&input_stream);
    antlr4::CommonTokenStream tokens(&lexer);

    JsonParser parser(&tokens);

    Translator listener;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, parser.json());

    std::cout << listener.ret_xml_ << std::endl;
    return 0;
}