#include <iostream>
#include <antlr4-runtime.h>
#include <vector>
#include "CSVParser.h"
#include "CSVLexer.h"
#include "CSVBaseListener.h"
#include "myCSVListener.h"
using namespace std;


int main(){

    antlr4::ANTLRInputStream inputStream(cin);

    CSVLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokens(&lexer);
    CSVParser parser(&tokens);
    myCSVListener listener;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, parser.file());
    vector<unordered_map<string, string>> rows = listener.getRows();
    for (auto row : rows){
        for (auto &it : row)
        {
            cout<<it.first<<" = "<<it.second<<" ";
        }
        cout<<endl;
    }
    return 0;
    //std::cout << ret.as<LabeledExprVisitor*>()-> << std::endl;
}