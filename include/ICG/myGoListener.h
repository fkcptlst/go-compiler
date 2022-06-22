#ifndef INCLUDE_ICG_MYGOLIS_H_
#define INCLUDE_ICG_MYGOLIS_H_

#include "Common/Common.h"
#include "Common/Scope.h"
#include "Common/TAC.h"
#include "GoParser.h"
#include "GoLexer.h"
#include "GoParserListener.h"
#include "GoParserBaseListener.h"
#include "ICG/StmtICG/ForStmt.h"

namespace std
{

class myGoListener : public GoParserListener
{

public:
    int LineIndex = 0;
    int LocalIndex = 0;
    int IFIndex = 0;
    int ForIndex = 0;
    // TACBlock *test = new TACBlock;
    // 全局的key是global，其他的为函数名字
    TACFile TACBlocks;
    
    antlr4::tree::ParseTreeProperty<string> *values = new antlr4::tree::ParseTreeProperty<string>;
    antlr4::tree::ParseTreeProperty<string> *ifvalues = new antlr4::tree::ParseTreeProperty<string>;
    antlr4::tree::ParseTreeProperty<ForStmt> *forvalues = new antlr4::tree::ParseTreeProperty<ForStmt>;
    
    // 记录哪个临时变量是指针
    std::unordered_map<string,bool> ptrs;

    std::shared_ptr<Scope> currentScope;
    std::shared_ptr<Scope> globalScope;
    vector<std::shared_ptr<Scope>> allScopes;

    // todo:给函数的开始，更新curFUn
    string curFun="global";
    // vector<Symbol*> deleteSymbolList;

    Symbol::Type defineTmpType() {return Symbol::Type::INT;}

    string CreateLocalVar(); // check the local_varname is illgal
    
    string CreateElseLabel();

    string CreateForLabel();

    bool is_digit(string s);
    
    void Go23file(string filename);

    string ToString(TACOP num);

    string ToString(TACOPERANDTYPE num);

    TACOPERANDTYPE OperandTypereslove(string name);

    void myPrint(std::shared_ptr<Scope> currentScope);
    void addScope();
    void popScope();

    void push_line(TACOP op, Operand src1, Operand src2, Operand dst);
    void exitPackageClause(GoParser::PackageClauseContext *ctx);
    void enterPackageClause(GoParser::PackageClauseContext *ctx);

    void enterSourceFile(GoParser::SourceFileContext *ctx);
    void exitSourceFile(GoParser::SourceFileContext *ctx);

    

    void enterImportDecl(GoParser::ImportDeclContext *ctx);
    void exitImportDecl(GoParser::ImportDeclContext *ctx);

    void enterImportSpec(GoParser::ImportSpecContext *ctx);
    void exitImportSpec(GoParser::ImportSpecContext *ctx);

    void enterImportPath(GoParser::ImportPathContext *ctx);
    void exitImportPath(GoParser::ImportPathContext *ctx);

    void enterDeclaration(GoParser::DeclarationContext *ctx);
    void exitDeclaration(GoParser::DeclarationContext *ctx);

    void enterConstDecl(GoParser::ConstDeclContext *ctx);
    void exitConstDecl(GoParser::ConstDeclContext *ctx);

    void enterConstSpec(GoParser::ConstSpecContext *ctx);
    void exitConstSpec(GoParser::ConstSpecContext *ctx);

    void enterIdentifierList(GoParser::IdentifierListContext *ctx);
    void exitIdentifierList(GoParser::IdentifierListContext *ctx);

    void enterExpressionList(GoParser::ExpressionListContext *ctx);
    void exitExpressionList(GoParser::ExpressionListContext *ctx);

    void enterTypeDecl(GoParser::TypeDeclContext *ctx);
    void exitTypeDecl(GoParser::TypeDeclContext *ctx);

    void enterTypeSpec(GoParser::TypeSpecContext *ctx);
    void exitTypeSpec(GoParser::TypeSpecContext *ctx);

    void enterFunctionDecl(GoParser::FunctionDeclContext *ctx);
    void exitFunctionDecl(GoParser::FunctionDeclContext *ctx);

    void enterMethodDecl(GoParser::MethodDeclContext *ctx);
    void exitMethodDecl(GoParser::MethodDeclContext *ctx);

    void enterReceiver(GoParser::ReceiverContext *ctx);
    void exitReceiver(GoParser::ReceiverContext *ctx);

    void enterVarDecl(GoParser::VarDeclContext *ctx);
    void exitVarDecl(GoParser::VarDeclContext *ctx);

    void enterVarSpec(GoParser::VarSpecContext *ctx);
    void exitVarSpec(GoParser::VarSpecContext *ctx);

    void enterBlock(GoParser::BlockContext *ctx);
    void exitBlock(GoParser::BlockContext *ctx);

    void enterStatementList(GoParser::StatementListContext *ctx);
    void exitStatementList(GoParser::StatementListContext *ctx);

    void enterStatement(GoParser::StatementContext *ctx);
    void exitStatement(GoParser::StatementContext *ctx);

    void enterSimpleStmt(GoParser::SimpleStmtContext *ctx);
    void exitSimpleStmt(GoParser::SimpleStmtContext *ctx);

    void enterExpressionStmt(GoParser::ExpressionStmtContext *ctx);
    void exitExpressionStmt(GoParser::ExpressionStmtContext *ctx);

    void enterSendStmt(GoParser::SendStmtContext *ctx);
    void exitSendStmt(GoParser::SendStmtContext *ctx);

    void enterIncDecStmt(GoParser::IncDecStmtContext *ctx);
    void exitIncDecStmt(GoParser::IncDecStmtContext *ctx);

    void enterAssignment(GoParser::AssignmentContext *ctx);
    void exitAssignment(GoParser::AssignmentContext *ctx);

    void enterAssign_op(GoParser::Assign_opContext *ctx);
    void exitAssign_op(GoParser::Assign_opContext *ctx);

    void enterShortVarDecl(GoParser::ShortVarDeclContext *ctx);
    void exitShortVarDecl(GoParser::ShortVarDeclContext *ctx);

    void enterEmptyStmt(GoParser::EmptyStmtContext *ctx);
    void exitEmptyStmt(GoParser::EmptyStmtContext *ctx);

    void enterLabeledStmt(GoParser::LabeledStmtContext *ctx);
    void exitLabeledStmt(GoParser::LabeledStmtContext *ctx);

    void enterReturnStmt(GoParser::ReturnStmtContext *ctx);
    void exitReturnStmt(GoParser::ReturnStmtContext *ctx);

    void enterBreakStmt(GoParser::BreakStmtContext *ctx);
    void exitBreakStmt(GoParser::BreakStmtContext *ctx);

    void enterContinueStmt(GoParser::ContinueStmtContext *ctx);
    void exitContinueStmt(GoParser::ContinueStmtContext *ctx);

    void enterGotoStmt(GoParser::GotoStmtContext *ctx);
    void exitGotoStmt(GoParser::GotoStmtContext *ctx);

    void enterFallthroughStmt(GoParser::FallthroughStmtContext *ctx);
    void exitFallthroughStmt(GoParser::FallthroughStmtContext *ctx);

    void enterDeferStmt(GoParser::DeferStmtContext *ctx);
    void exitDeferStmt(GoParser::DeferStmtContext *ctx);

    void enterIfStmt(GoParser::IfStmtContext *ctx);
    void exitIfStmt(GoParser::IfStmtContext *ctx);

    void enterSwitchStmt(GoParser::SwitchStmtContext *ctx);
    void exitSwitchStmt(GoParser::SwitchStmtContext *ctx);

    void enterExprSwitchStmt(GoParser::ExprSwitchStmtContext *ctx);
    void exitExprSwitchStmt(GoParser::ExprSwitchStmtContext *ctx);

    void enterExprCaseClause(GoParser::ExprCaseClauseContext *ctx);
    void exitExprCaseClause(GoParser::ExprCaseClauseContext *ctx);

    void enterExprSwitchCase(GoParser::ExprSwitchCaseContext *ctx);
    void exitExprSwitchCase(GoParser::ExprSwitchCaseContext *ctx);

    void enterTypeSwitchStmt(GoParser::TypeSwitchStmtContext *ctx);
    void exitTypeSwitchStmt(GoParser::TypeSwitchStmtContext *ctx);

    void enterTypeSwitchGuard(GoParser::TypeSwitchGuardContext *ctx);
    void exitTypeSwitchGuard(GoParser::TypeSwitchGuardContext *ctx);

    void enterTypeCaseClause(GoParser::TypeCaseClauseContext *ctx);
    void exitTypeCaseClause(GoParser::TypeCaseClauseContext *ctx);

    void enterTypeSwitchCase(GoParser::TypeSwitchCaseContext *ctx);
    void exitTypeSwitchCase(GoParser::TypeSwitchCaseContext *ctx);

    void enterTypeList(GoParser::TypeListContext *ctx);
    void exitTypeList(GoParser::TypeListContext *ctx);

    void enterSelectStmt(GoParser::SelectStmtContext *ctx);
    void exitSelectStmt(GoParser::SelectStmtContext *ctx);

    void enterCommClause(GoParser::CommClauseContext *ctx);
    void exitCommClause(GoParser::CommClauseContext *ctx);

    void enterCommCase(GoParser::CommCaseContext *ctx);
    void exitCommCase(GoParser::CommCaseContext *ctx);

    void enterRecvStmt(GoParser::RecvStmtContext *ctx);
    void exitRecvStmt(GoParser::RecvStmtContext *ctx);

    void enterForStmt(GoParser::ForStmtContext *ctx);
    void exitForStmt(GoParser::ForStmtContext *ctx);

    void enterForClause(GoParser::ForClauseContext *ctx);
    void exitForClause(GoParser::ForClauseContext *ctx);

    void enterRangeClause(GoParser::RangeClauseContext *ctx);
    void exitRangeClause(GoParser::RangeClauseContext *ctx);

    void enterGoStmt(GoParser::GoStmtContext *ctx);
    void exitGoStmt(GoParser::GoStmtContext *ctx);

    void enterType_(GoParser::Type_Context *ctx);
    void exitType_(GoParser::Type_Context *ctx);

    void enterTypeName(GoParser::TypeNameContext *ctx);
    void exitTypeName(GoParser::TypeNameContext *ctx);

    void enterTypeLit(GoParser::TypeLitContext *ctx);
    void exitTypeLit(GoParser::TypeLitContext *ctx);

    void enterArrayType(GoParser::ArrayTypeContext *ctx);
    void exitArrayType(GoParser::ArrayTypeContext *ctx);

    void enterArrayLength(GoParser::ArrayLengthContext *ctx);
    void exitArrayLength(GoParser::ArrayLengthContext *ctx);

    void enterElementType(GoParser::ElementTypeContext *ctx);
    void exitElementType(GoParser::ElementTypeContext *ctx);

    void enterPointerType(GoParser::PointerTypeContext *ctx);
    void exitPointerType(GoParser::PointerTypeContext *ctx);

    void enterInterfaceType(GoParser::InterfaceTypeContext *ctx);
    void exitInterfaceType(GoParser::InterfaceTypeContext *ctx);

    void enterSliceType(GoParser::SliceTypeContext *ctx);
    void exitSliceType(GoParser::SliceTypeContext *ctx);

    void enterMapType(GoParser::MapTypeContext *ctx);
    void exitMapType(GoParser::MapTypeContext *ctx);

    void enterChannelType(GoParser::ChannelTypeContext *ctx);
    void exitChannelType(GoParser::ChannelTypeContext *ctx);

    void enterMethodSpec(GoParser::MethodSpecContext *ctx);
    void exitMethodSpec(GoParser::MethodSpecContext *ctx);

    void enterFunctionType(GoParser::FunctionTypeContext *ctx);
    void exitFunctionType(GoParser::FunctionTypeContext *ctx);

    void enterSignature(GoParser::SignatureContext *ctx);
    void exitSignature(GoParser::SignatureContext *ctx);

    void enterResult(GoParser::ResultContext *ctx);
    void exitResult(GoParser::ResultContext *ctx);

    void enterParameters(GoParser::ParametersContext *ctx);
    void exitParameters(GoParser::ParametersContext *ctx);

    void enterParameterDecl(GoParser::ParameterDeclContext *ctx);
    void exitParameterDecl(GoParser::ParameterDeclContext *ctx);

    void enterExpression(GoParser::ExpressionContext *ctx);
    void exitExpression(GoParser::ExpressionContext *ctx);

    void enterUnaryOperation(GoParser::UnaryOperationContext *ctx);
    void exitUnaryOperation(GoParser::UnaryOperationContext *ctx);

    void enterPrimaryExpression(GoParser::PrimaryExpressionContext *ctx);
    void exitPrimaryExpression(GoParser::PrimaryExpressionContext *ctx);

    void enterPlusMinusOperation(GoParser::PlusMinusOperationContext *ctx);
    void exitPlusMinusOperation(GoParser::PlusMinusOperationContext *ctx);

    void enterRelationOperation(GoParser::RelationOperationContext *ctx);
    void exitRelationOperation(GoParser::RelationOperationContext *ctx);

    void enterMulDivOperation(GoParser::MulDivOperationContext *ctx);
    void exitMulDivOperation(GoParser::MulDivOperationContext *ctx);

    void enterLogicalOrOperation(GoParser::LogicalOrOperationContext *ctx);
    void exitLogicalOrOperation(GoParser::LogicalOrOperationContext *ctx);

    void enterLogicalAndOperation(GoParser::LogicalAndOperationContext *ctx);
    void exitLogicalAndOperation(GoParser::LogicalAndOperationContext *ctx);

    void enterPrimaryExpr(GoParser::PrimaryExprContext *ctx);
    void exitPrimaryExpr(GoParser::PrimaryExprContext *ctx);

    void enterConversion(GoParser::ConversionContext *ctx);
    void exitConversion(GoParser::ConversionContext *ctx);

    void enterNonNamedType(GoParser::NonNamedTypeContext *ctx);
    void exitNonNamedType(GoParser::NonNamedTypeContext *ctx);

    void enterOperand(GoParser::OperandContext *ctx);
    void exitOperand(GoParser::OperandContext *ctx);

    void enterLiteral(GoParser::LiteralContext *ctx);
    void exitLiteral(GoParser::LiteralContext *ctx);

    void enterBasicLit(GoParser::BasicLitContext *ctx);
    void exitBasicLit(GoParser::BasicLitContext *ctx);

    void enterInteger(GoParser::IntegerContext *ctx);
    void exitInteger(GoParser::IntegerContext *ctx);

    void enterOperandName(GoParser::OperandNameContext *ctx);
    void exitOperandName(GoParser::OperandNameContext *ctx);

    void enterQualifiedIdent(GoParser::QualifiedIdentContext *ctx);
    void exitQualifiedIdent(GoParser::QualifiedIdentContext *ctx);

    void enterCompositeLit(GoParser::CompositeLitContext *ctx);
    void exitCompositeLit(GoParser::CompositeLitContext *ctx);

    void enterLiteralType(GoParser::LiteralTypeContext *ctx);
    void exitLiteralType(GoParser::LiteralTypeContext *ctx);

    void enterLiteralValue(GoParser::LiteralValueContext *ctx);
    void exitLiteralValue(GoParser::LiteralValueContext *ctx);

    void enterElementList(GoParser::ElementListContext *ctx);
    void exitElementList(GoParser::ElementListContext *ctx);

    void enterKeyedElement(GoParser::KeyedElementContext *ctx);
    void exitKeyedElement(GoParser::KeyedElementContext *ctx);

    void enterKey(GoParser::KeyContext *ctx);
    void exitKey(GoParser::KeyContext *ctx);

    void enterElement(GoParser::ElementContext *ctx);
    void exitElement(GoParser::ElementContext *ctx);

    void enterStructType(GoParser::StructTypeContext *ctx);
    void exitStructType(GoParser::StructTypeContext *ctx);

    void enterFieldDecl(GoParser::FieldDeclContext *ctx);
    void exitFieldDecl(GoParser::FieldDeclContext *ctx);

    void enterString_(GoParser::String_Context *ctx);
    void exitString_(GoParser::String_Context *ctx);

    void enterEmbeddedField(GoParser::EmbeddedFieldContext *ctx);
    void exitEmbeddedField(GoParser::EmbeddedFieldContext *ctx);

    void enterFunctionLit(GoParser::FunctionLitContext *ctx);
    void exitFunctionLit(GoParser::FunctionLitContext *ctx);

    void enterIndex(GoParser::IndexContext *ctx);
    void exitIndex(GoParser::IndexContext *ctx);

    void enterSlice_(GoParser::Slice_Context *ctx);
    void exitSlice_(GoParser::Slice_Context *ctx);

    void enterTypeAssertion(GoParser::TypeAssertionContext *ctx);
    void exitTypeAssertion(GoParser::TypeAssertionContext *ctx);

    void enterArguments(GoParser::ArgumentsContext *ctx);
    void exitArguments(GoParser::ArgumentsContext *ctx);

    void enterMethodExpr(GoParser::MethodExprContext *ctx);
    void exitMethodExpr(GoParser::MethodExprContext *ctx);

    void enterReceiverType(GoParser::ReceiverTypeContext *ctx);
    void exitReceiverType(GoParser::ReceiverTypeContext *ctx);

    void enterEoss(GoParser::EossContext *ctx);
    void exitEoss(GoParser::EossContext *ctx);

    void visitTerminal(antlr4::tree::TerminalNode *node);
    void visitErrorNode(antlr4::tree::ErrorNode *node);
    void enterEveryRule(antlr4::ParserRuleContext *ctx);
    void exitEveryRule(antlr4::ParserRuleContext *ctx);

private:
};

}// namespace std end
#endif