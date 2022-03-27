#include <iostream>
#include <antlr4-runtime.h>
#include <vector>
#include "CSVParser.h"
#include "CSVLexer.h"
#include "CSVBaseListener.h"

using namespace std;


class myCSVListener : public CSVBaseListener{
    public:
        void exitString(CSVParser::StringContext* ctx);

        void exitText(CSVParser::TextContext* ctx);

        void exitEmpty(CSVParser::EmptyContext* ctx);

        void exitHdr(CSVParser::HdrContext *ctx);

        void exitRow(CSVParser::RowContext* ctx);

        vector<unordered_map<string, string>> getRows();

        vector<string> getHeader();

        vector<string> getCurrentRowValues();
        
    private:
        vector<string> currentRowValues;
        vector<string> header;
        vector<unordered_map<string, string>> rows;
        bool hdr = true;

};
