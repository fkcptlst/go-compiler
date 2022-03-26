#include <iostream>
#include <antlr4-runtime.h>
#include <vector>
#include "CSVParser.h"
#include "CSVLexer.h"
#include "CSVBaseListener.h"
#include "myCSVListener.h"

using namespace std;


void myCSVListener::exitString(CSVParser::StringContext* ctx){

    currentRowValues.push_back(ctx->STRING()->getText()); 
    
}

void myCSVListener::exitText(CSVParser::TextContext* ctx){
    
    currentRowValues.push_back(ctx->TEXT()->getText()); 
    
}

void myCSVListener::exitEmpty(CSVParser::EmptyContext* ctx){
    
    currentRowValues.push_back(""); 
    
}

void myCSVListener::exitHdr(CSVParser::HdrContext *ctx){
    for (auto rowvalue : currentRowValues)
    {
        header.push_back(rowvalue);
    }
    currentRowValues.clear();
}

void myCSVListener::exitRow(CSVParser::RowContext* ctx)
{
    if(hdr)
    {   hdr = false;
        return;
    }
    unordered_map<string, string> map;
    int i = 0;
    for (auto v : currentRowValues) {
        map.insert({header[i], v});
        i++;
    }
    currentRowValues.clear();
    rows.push_back(map);
}

vector<unordered_map<string, string>> myCSVListener::getRows()
{
    return rows;
}

vector<string> myCSVListener::getHeader()
{
    return header;
}

vector<string> myCSVListener::getCurrentRowValues()
{
    return currentRowValues;
}
