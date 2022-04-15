#include <iostream>
#include <antlr4-runtime.h>
#include <vector>
#include <string>
#include <fstream>
#include "GoParser.h"
#include "GoLexer.h"
#include "GoParserListener.h"
#include "GoParserBaseListener.h"
#include "myGoListener.h"
#include "TAC.h"

using namespace std;

string myGoListener::CreateLocalVar(){
    string Local;
    bool CheckResult = 0;
    do
    {
        Local = "T" + to_string(myGoListener::LocalIndex);
        myGoListener::LocalIndex ++;
        
    }
    while(CheckResult);
    return Local;
}
string myGoListener::ToString(TACOP num){
    switch (num)
    {
        case TACOP::ADD:    return "ADD";
        case TACOP::SUB:    return "SUB";
        case TACOP::DIV:    return "DIV";
        case TACOP::MUL:    return "MUL";
        default:  return "";
    }
}

void myGoListener::Go23file(string filename){
    for(auto it : test)
    {
        ofstream outfile;
        outfile.open(filename, ios::out|ios::app);
        outfile << it.line << " " << ToString(it.op) << " " << it.src1 << " " << it.src2 << " " << it.dst << endl;
        outfile.close();
    }
}

void myGoListener::exitPackageClause(GoParser::PackageClauseContext *ctx)
{
    cout << ctx->IDENTIFIER()->getText() << endl;
}
void myGoListener::enterPackageClause(GoParser::PackageClauseContext *ctx){}


void myGoListener::enterInteger(GoParser::IntegerContext *ctx){}
void myGoListener::exitInteger(GoParser::IntegerContext *ctx){
    values.put(ctx,ctx->DECIMAL_LIT()->getText());
}


void myGoListener::enterBasicLit(GoParser::BasicLitContext *ctx){}
void myGoListener::exitBasicLit(GoParser::BasicLitContext *ctx){
    if (ctx->integer()){
        string BasicLitValue = values.get(ctx->integer());
        values.put(ctx, BasicLitValue);
    }
}


void myGoListener::enterOperand(GoParser::OperandContext *ctx){}
void myGoListener::exitOperand(GoParser::OperandContext *ctx){
    if (ctx->literal()){
        string OperandValue = values.get(ctx->literal());
        values.put(ctx, OperandValue);
    }
    if (ctx->expression()){
        string OperandValue = values.get(ctx->expression());
        values.put(ctx, OperandValue);
    }
}

void myGoListener::enterLiteral(GoParser::LiteralContext *ctx){}
void myGoListener::exitLiteral(GoParser::LiteralContext *ctx){
    if (ctx->basicLit()){
        string LiteralValue = values.get(ctx->basicLit());
        values.put(ctx, LiteralValue);
    }
}

void myGoListener::enterPrimaryExpr(GoParser::PrimaryExprContext *ctx){}
void myGoListener::exitPrimaryExpr(GoParser::PrimaryExprContext *ctx){
    if (ctx->operand()){
        string PrimaryExprValue = values.get(ctx->operand());
        values.put(ctx, PrimaryExprValue);
    }
}

void myGoListener::enterExpression(GoParser::ExpressionContext *ctx){}
void myGoListener::exitExpression(GoParser::ExpressionContext *ctx){
}

void myGoListener::enterUnaryOperation(GoParser::UnaryOperationContext *ctx){}
void myGoListener::exitUnaryOperation(GoParser::UnaryOperationContext *ctx){

}

void myGoListener::enterPrimaryExpression(GoParser::PrimaryExpressionContext *ctx){}
void myGoListener::exitPrimaryExpression(GoParser::PrimaryExpressionContext *ctx){
    string ExpresionValue = values.get(ctx->primaryExpr());
    values.put(ctx, ExpresionValue);
}

void myGoListener::enterPlusMinusOperation(GoParser::PlusMinusOperationContext *ctx){}
void myGoListener::exitPlusMinusOperation(GoParser::PlusMinusOperationContext *ctx){
    if(ctx->PLUS())
    {
        string dst = CreateLocalVar();
        test.push_back(TACLine(myGoListener::LineIndex, TACOP::ADD, values.get(ctx->expression(0)), values.get(ctx->expression(1)), dst));
        values.put(ctx, dst);
        myGoListener::LineIndex++;
        
    }

    else if(ctx->MINUS())
    {
        string dst = CreateLocalVar();
        test.push_back(TACLine(myGoListener::LineIndex, TACOP::SUB, values.get(ctx->expression(0)), values.get(ctx->expression(1)), dst));
        values.put(ctx, dst);
        myGoListener::LineIndex++;
        
    }

}

void myGoListener::enterRelationOperation(GoParser::RelationOperationContext *ctx){}
void myGoListener::exitRelationOperation(GoParser::RelationOperationContext *ctx){

}

void myGoListener::enterMulDivOperation(GoParser::MulDivOperationContext *ctx){}
void myGoListener::exitMulDivOperation(GoParser::MulDivOperationContext *ctx){
    if(ctx->STAR())
    {
        string dst = CreateLocalVar();
        test.push_back(TACLine(myGoListener::LineIndex, TACOP::MUL, values.get(ctx->expression(0)), values.get(ctx->expression(1)), dst));
        values.put(ctx, dst);
        myGoListener::LineIndex++;
        
    }

    else if(ctx->DIV())
    {
        string dst = CreateLocalVar();
        test.push_back(TACLine(myGoListener::LineIndex, TACOP::DIV, values.get(ctx->expression(0)), values.get(ctx->expression(1)), dst));
        values.put(ctx, dst);
        myGoListener::LineIndex++;
        
    }
}

void myGoListener::enterLogicalOrOperation(GoParser::LogicalOrOperationContext *ctx){}
void myGoListener::exitLogicalOrOperation(GoParser::LogicalOrOperationContext *ctx){

}

void myGoListener::enterLogicalAndOperation(GoParser::LogicalAndOperationContext *ctx){}
void myGoListener::exitLogicalAndOperation(GoParser::LogicalAndOperationContext *ctx){
    
}



void myGoListener::enterSourceFile(GoParser::SourceFileContext *ctx){
    Scope* globalScope=new Scope();
    currentScope = globalScope;
    // scopes.put(ctx,currentScope);
    deleteLine.push_back(globalScope);
}

void myGoListener::exitSourceFile(GoParser::SourceFileContext *ctx){}

void myGoListener::enterImportDecl(GoParser::ImportDeclContext *ctx){}
void myGoListener::exitImportDecl(GoParser::ImportDeclContext *ctx){}

void myGoListener::enterImportSpec(GoParser::ImportSpecContext *ctx){}
void myGoListener::exitImportSpec(GoParser::ImportSpecContext *ctx){}

void myGoListener::enterImportPath(GoParser::ImportPathContext *ctx){}
void myGoListener::exitImportPath(GoParser::ImportPathContext *ctx){}

void myGoListener::enterDeclaration(GoParser::DeclarationContext *ctx){}
void myGoListener::exitDeclaration(GoParser::DeclarationContext *ctx){}

void myGoListener::enterConstDecl(GoParser::ConstDeclContext *ctx){}
void myGoListener::exitConstDecl(GoParser::ConstDeclContext *ctx){}

void myGoListener::enterConstSpec(GoParser::ConstSpecContext *ctx){}
void myGoListener::exitConstSpec(GoParser::ConstSpecContext *ctx){}

void myGoListener::enterIdentifierList(GoParser::IdentifierListContext *ctx){}
void myGoListener::exitIdentifierList(GoParser::IdentifierListContext *ctx){}

void myGoListener::enterExpressionList(GoParser::ExpressionListContext *ctx){}
void myGoListener::exitExpressionList(GoParser::ExpressionListContext *ctx){}

void myGoListener::enterTypeDecl(GoParser::TypeDeclContext *ctx){}
void myGoListener::exitTypeDecl(GoParser::TypeDeclContext *ctx){}

void myGoListener::enterTypeSpec(GoParser::TypeSpecContext *ctx){}
void myGoListener::exitTypeSpec(GoParser::TypeSpecContext *ctx){}

void myGoListener::enterFunctionDecl(GoParser::FunctionDeclContext *ctx){
    if(ctx->block()){ // 有block时
        addScope();
    }
}
void myGoListener::exitFunctionDecl(GoParser::FunctionDeclContext *ctx){
    if(ctx->block()){ // 有block时
        popScope();
    }
}

void myGoListener::enterMethodDecl(GoParser::MethodDeclContext *ctx){}
void myGoListener::exitMethodDecl(GoParser::MethodDeclContext *ctx){}

void myGoListener::enterReceiver(GoParser::ReceiverContext *ctx){}
void myGoListener::exitReceiver(GoParser::ReceiverContext *ctx){}

void myGoListener::enterVarDecl(GoParser::VarDeclContext *ctx){}
void myGoListener::exitVarDecl(GoParser::VarDeclContext *ctx){}

void myGoListener::enterVarSpec(GoParser::VarSpecContext *ctx){}
void myGoListener::exitVarSpec(GoParser::VarSpecContext *ctx){
    
    int n=ctx->identifierList()->IDENTIFIER().size();
    for(int i=0;i<n;++i){
        string integer=ctx->identifierList()->IDENTIFIER(i)->getText();
        // Symbol::Type type=ctx->type_()->typeName()->getText();
        string stype=ctx->type_()->typeName()->getText();
        Symbol::Type type=Symbol::toType(stype);
        Symbol symbol(integer,currentScope,Symbol::SymbolType::VAR,type);
        myGoListener::currentScope->define(symbol);
    }
}

void myGoListener::enterBlock(GoParser::BlockContext *ctx){}
void myGoListener::exitBlock(GoParser::BlockContext *ctx){}

void myGoListener::enterStatementList(GoParser::StatementListContext *ctx){}
void myGoListener::exitStatementList(GoParser::StatementListContext *ctx){}

void myGoListener::enterStatement(GoParser::StatementContext *ctx){
    if(ctx->block()) cout<<"----------------"<<endl;
}
void myGoListener::exitStatement(GoParser::StatementContext *ctx){}

void myGoListener::enterSimpleStmt(GoParser::SimpleStmtContext *ctx){}
void myGoListener::exitSimpleStmt(GoParser::SimpleStmtContext *ctx){}

void myGoListener::enterExpressionStmt(GoParser::ExpressionStmtContext *ctx){}
void myGoListener::exitExpressionStmt(GoParser::ExpressionStmtContext *ctx){}

void myGoListener::enterSendStmt(GoParser::SendStmtContext *ctx){}
void myGoListener::exitSendStmt(GoParser::SendStmtContext *ctx){}

void myGoListener::enterIncDecStmt(GoParser::IncDecStmtContext *ctx){}
void myGoListener::exitIncDecStmt(GoParser::IncDecStmtContext *ctx){}

void myGoListener::enterAssignment(GoParser::AssignmentContext *ctx){}
void myGoListener::exitAssignment(GoParser::AssignmentContext *ctx){}

void myGoListener::enterAssign_op(GoParser::Assign_opContext *ctx){}
void myGoListener::exitAssign_op(GoParser::Assign_opContext *ctx){}

void myGoListener::enterShortVarDecl(GoParser::ShortVarDeclContext *ctx){}
void myGoListener::exitShortVarDecl(GoParser::ShortVarDeclContext *ctx){}

void myGoListener::enterEmptyStmt(GoParser::EmptyStmtContext *ctx){}
void myGoListener::exitEmptyStmt(GoParser::EmptyStmtContext *ctx){}

void myGoListener::enterLabeledStmt(GoParser::LabeledStmtContext *ctx){}
void myGoListener::exitLabeledStmt(GoParser::LabeledStmtContext *ctx){}

void myGoListener::enterReturnStmt(GoParser::ReturnStmtContext *ctx){}
void myGoListener::exitReturnStmt(GoParser::ReturnStmtContext *ctx){}

void myGoListener::enterBreakStmt(GoParser::BreakStmtContext *ctx){}
void myGoListener::exitBreakStmt(GoParser::BreakStmtContext *ctx){}

void myGoListener::enterContinueStmt(GoParser::ContinueStmtContext *ctx){}
void myGoListener::exitContinueStmt(GoParser::ContinueStmtContext *ctx){}

void myGoListener::enterGotoStmt(GoParser::GotoStmtContext *ctx){}
void myGoListener::exitGotoStmt(GoParser::GotoStmtContext *ctx){}

void myGoListener::enterFallthroughStmt(GoParser::FallthroughStmtContext *ctx){}
void myGoListener::exitFallthroughStmt(GoParser::FallthroughStmtContext *ctx){}

void myGoListener::enterDeferStmt(GoParser::DeferStmtContext *ctx){}
void myGoListener::exitDeferStmt(GoParser::DeferStmtContext *ctx){}

void myGoListener::enterIfStmt(GoParser::IfStmtContext *ctx){}
void myGoListener::exitIfStmt(GoParser::IfStmtContext *ctx){}

void myGoListener::enterSwitchStmt(GoParser::SwitchStmtContext *ctx){}
void myGoListener::exitSwitchStmt(GoParser::SwitchStmtContext *ctx){}

void myGoListener::enterExprSwitchStmt(GoParser::ExprSwitchStmtContext *ctx){}
void myGoListener::exitExprSwitchStmt(GoParser::ExprSwitchStmtContext *ctx){}

void myGoListener::enterExprCaseClause(GoParser::ExprCaseClauseContext *ctx){}
void myGoListener::exitExprCaseClause(GoParser::ExprCaseClauseContext *ctx){}

void myGoListener::enterExprSwitchCase(GoParser::ExprSwitchCaseContext *ctx){}
void myGoListener::exitExprSwitchCase(GoParser::ExprSwitchCaseContext *ctx){}

void myGoListener::enterTypeSwitchStmt(GoParser::TypeSwitchStmtContext *ctx){}
void myGoListener::exitTypeSwitchStmt(GoParser::TypeSwitchStmtContext *ctx){}

void myGoListener::enterTypeSwitchGuard(GoParser::TypeSwitchGuardContext *ctx){}
void myGoListener::exitTypeSwitchGuard(GoParser::TypeSwitchGuardContext *ctx){}

void myGoListener::enterTypeCaseClause(GoParser::TypeCaseClauseContext *ctx){}
void myGoListener::exitTypeCaseClause(GoParser::TypeCaseClauseContext *ctx){}

void myGoListener::enterTypeSwitchCase(GoParser::TypeSwitchCaseContext *ctx){}
void myGoListener::exitTypeSwitchCase(GoParser::TypeSwitchCaseContext *ctx){}

void myGoListener::enterTypeList(GoParser::TypeListContext *ctx){}
void myGoListener::exitTypeList(GoParser::TypeListContext *ctx){}

void myGoListener::enterSelectStmt(GoParser::SelectStmtContext *ctx){}
void myGoListener::exitSelectStmt(GoParser::SelectStmtContext *ctx){}

void myGoListener::enterCommClause(GoParser::CommClauseContext *ctx){}
void myGoListener::exitCommClause(GoParser::CommClauseContext *ctx){}

void myGoListener::enterCommCase(GoParser::CommCaseContext *ctx){}
void myGoListener::exitCommCase(GoParser::CommCaseContext *ctx){}

void myGoListener::enterRecvStmt(GoParser::RecvStmtContext *ctx){}
void myGoListener::exitRecvStmt(GoParser::RecvStmtContext *ctx){}

void myGoListener::enterForStmt(GoParser::ForStmtContext *ctx){}
void myGoListener::exitForStmt(GoParser::ForStmtContext *ctx){}

void myGoListener::enterForClause(GoParser::ForClauseContext *ctx){}
void myGoListener::exitForClause(GoParser::ForClauseContext *ctx){}

void myGoListener::enterRangeClause(GoParser::RangeClauseContext *ctx){}
void myGoListener::exitRangeClause(GoParser::RangeClauseContext *ctx){}

void myGoListener::enterGoStmt(GoParser::GoStmtContext *ctx){}
void myGoListener::exitGoStmt(GoParser::GoStmtContext *ctx){}

void myGoListener::enterType_(GoParser::Type_Context *ctx){}
void myGoListener::exitType_(GoParser::Type_Context *ctx){}

void myGoListener::enterTypeName(GoParser::TypeNameContext *ctx){}
void myGoListener::exitTypeName(GoParser::TypeNameContext *ctx){}

void myGoListener::enterTypeLit(GoParser::TypeLitContext *ctx){}
void myGoListener::exitTypeLit(GoParser::TypeLitContext *ctx){}

void myGoListener::enterArrayType(GoParser::ArrayTypeContext *ctx){}
void myGoListener::exitArrayType(GoParser::ArrayTypeContext *ctx){}

void myGoListener::enterArrayLength(GoParser::ArrayLengthContext *ctx){}
void myGoListener::exitArrayLength(GoParser::ArrayLengthContext *ctx){}

void myGoListener::enterElementType(GoParser::ElementTypeContext *ctx){}
void myGoListener::exitElementType(GoParser::ElementTypeContext *ctx){}

void myGoListener::enterPointerType(GoParser::PointerTypeContext *ctx){}
void myGoListener::exitPointerType(GoParser::PointerTypeContext *ctx){}

void myGoListener::enterInterfaceType(GoParser::InterfaceTypeContext *ctx){}
void myGoListener::exitInterfaceType(GoParser::InterfaceTypeContext *ctx){}

void myGoListener::enterSliceType(GoParser::SliceTypeContext *ctx){}
void myGoListener::exitSliceType(GoParser::SliceTypeContext *ctx){}

void myGoListener::enterMapType(GoParser::MapTypeContext *ctx){}
void myGoListener::exitMapType(GoParser::MapTypeContext *ctx){}

void myGoListener::enterChannelType(GoParser::ChannelTypeContext *ctx){}
void myGoListener::exitChannelType(GoParser::ChannelTypeContext *ctx){}

void myGoListener::enterMethodSpec(GoParser::MethodSpecContext *ctx){}
void myGoListener::exitMethodSpec(GoParser::MethodSpecContext *ctx){}

void myGoListener::enterFunctionType(GoParser::FunctionTypeContext *ctx){}
void myGoListener::exitFunctionType(GoParser::FunctionTypeContext *ctx){}

void myGoListener::enterSignature(GoParser::SignatureContext *ctx){}
void myGoListener::exitSignature(GoParser::SignatureContext *ctx){}

void myGoListener::enterResult(GoParser::ResultContext *ctx){}
void myGoListener::exitResult(GoParser::ResultContext *ctx){}

void myGoListener::enterParameters(GoParser::ParametersContext *ctx){}
void myGoListener::exitParameters(GoParser::ParametersContext *ctx){}

void myGoListener::enterParameterDecl(GoParser::ParameterDeclContext *ctx){
    int n=ctx->identifierList()->IDENTIFIER().size();
    for(int i=0;i<n;++i){
        string integer=ctx->identifierList()->IDENTIFIER(i)->getText();
        // Symbol::Type type=ctx->type_()->typeName()->getText();
        string stype=ctx->type_()->typeName()->getText();
        Symbol::Type type=Symbol::toType(stype);
        Symbol symbol(integer,currentScope,Symbol::SymbolType::VAR,type);
        myGoListener::currentScope->define(symbol);
    }
}
void myGoListener::exitParameterDecl(GoParser::ParameterDeclContext *ctx){}

void myGoListener::enterConversion(GoParser::ConversionContext *ctx){}
void myGoListener::exitConversion(GoParser::ConversionContext *ctx){}

void myGoListener::enterNonNamedType(GoParser::NonNamedTypeContext *ctx){}
void myGoListener::exitNonNamedType(GoParser::NonNamedTypeContext *ctx){}



void myGoListener::enterOperandName(GoParser::OperandNameContext *ctx){}
void myGoListener::exitOperandName(GoParser::OperandNameContext *ctx){}

void myGoListener::enterQualifiedIdent(GoParser::QualifiedIdentContext *ctx){}
void myGoListener::exitQualifiedIdent(GoParser::QualifiedIdentContext *ctx){}

void myGoListener::enterCompositeLit(GoParser::CompositeLitContext *ctx){}
void myGoListener::exitCompositeLit(GoParser::CompositeLitContext *ctx){}

void myGoListener::enterLiteralType(GoParser::LiteralTypeContext *ctx){}
void myGoListener::exitLiteralType(GoParser::LiteralTypeContext *ctx){}

void myGoListener::enterLiteralValue(GoParser::LiteralValueContext *ctx){}
void myGoListener::exitLiteralValue(GoParser::LiteralValueContext *ctx){}

void myGoListener::enterElementList(GoParser::ElementListContext *ctx){}
void myGoListener::exitElementList(GoParser::ElementListContext *ctx){}

void myGoListener::enterKeyedElement(GoParser::KeyedElementContext *ctx){}
void myGoListener::exitKeyedElement(GoParser::KeyedElementContext *ctx){}

void myGoListener::enterKey(GoParser::KeyContext *ctx){}
void myGoListener::exitKey(GoParser::KeyContext *ctx){}

void myGoListener::enterElement(GoParser::ElementContext *ctx){}
void myGoListener::exitElement(GoParser::ElementContext *ctx){}

void myGoListener::enterStructType(GoParser::StructTypeContext *ctx){}
void myGoListener::exitStructType(GoParser::StructTypeContext *ctx){}

void myGoListener::enterFieldDecl(GoParser::FieldDeclContext *ctx){}
void myGoListener::exitFieldDecl(GoParser::FieldDeclContext *ctx){}

void myGoListener::enterString_(GoParser::String_Context *ctx){}
void myGoListener::exitString_(GoParser::String_Context *ctx){}

void myGoListener::enterEmbeddedField(GoParser::EmbeddedFieldContext *ctx){}
void myGoListener::exitEmbeddedField(GoParser::EmbeddedFieldContext *ctx){}

void myGoListener::enterFunctionLit(GoParser::FunctionLitContext *ctx){}
void myGoListener::exitFunctionLit(GoParser::FunctionLitContext *ctx){}

void myGoListener::enterIndex(GoParser::IndexContext *ctx){}
void myGoListener::exitIndex(GoParser::IndexContext *ctx){}

void myGoListener::enterSlice_(GoParser::Slice_Context *ctx){}
void myGoListener::exitSlice_(GoParser::Slice_Context *ctx){}

void myGoListener::enterTypeAssertion(GoParser::TypeAssertionContext *ctx){}
void myGoListener::exitTypeAssertion(GoParser::TypeAssertionContext *ctx){}

void myGoListener::enterArguments(GoParser::ArgumentsContext *ctx){}
void myGoListener::exitArguments(GoParser::ArgumentsContext *ctx){}

void myGoListener::enterMethodExpr(GoParser::MethodExprContext *ctx){}
void myGoListener::exitMethodExpr(GoParser::MethodExprContext *ctx){}

void myGoListener::enterReceiverType(GoParser::ReceiverTypeContext *ctx){}
void myGoListener::exitReceiverType(GoParser::ReceiverTypeContext *ctx){}

void myGoListener::enterEoss(GoParser::EossContext *ctx){}
void myGoListener::exitEoss(GoParser::EossContext *ctx){}

void myGoListener::visitTerminal(antlr4::tree::TerminalNode *node){}
void myGoListener::visitErrorNode(antlr4::tree::ErrorNode *node){}
void myGoListener::enterEveryRule(antlr4::ParserRuleContext *ctx){}
void myGoListener::exitEveryRule(antlr4::ParserRuleContext *ctx){}
// void myGoListener::myGoListener::exitString(GoParser::StringContext* ctx){

//     currentRowValues.push_back(ctx->STRING()->getText()){}

// }

// void myGoListener::myGoListener::exitText(GoParser::TextContext* ctx){

//     currentRowValues.push_back(ctx->TEXT()->getText()){}

// }

// void myGoListener::myGoListener::exitEmpty(GoParser::EmptyContext* ctx){

//     currentRowValues.push_back(""){}

// }


// void myGoListener::myGoListener::exitHdr(GoParser::HdrContext *ctx){
//     for (auto rowvalue : currentRowValues)
//     {
//         header.push_back(rowvalue){}
//     }
//     currentRowValues.clear(){}
// }

// void myGoListener::myGoListener::exitRow(GoParser::RowContext* ctx)
// {
//     if(hdr)
//     {   hdr = false{}
//         return{}
//     }
//     unordered_map<string, string> map{}
//     int i{}
//     for (auto v : currentRowValues) {
//         map.insert({header[i], v}){}
//         i++{}
//     }
//     currentRowValues.clear(){}
//     rows.push_back(map){}
// }

// vector<unordered_map<string, string>> myGoListener::getRows()
// {
//     return rows{}
// }

// vector<string> myGoListener::getHeader()
// {
//     return header{}
// }

// vector<string> myGoListener::getCurrentRowValues()
// {
//     return currentRowValues{}
// }
