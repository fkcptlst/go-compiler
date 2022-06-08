#include <antlr4-runtime.h>
#include <Common/Common.h>
#include "Common/TAC.h"
#include "GoParser.h"
#include "GoLexer.h"
#include "GoParserListener.h"
#include "GoParserBaseListener.h"
#include "ICG/myGoListener.h"
#include "ICG/Utils.h"

using namespace std;

string myGoListener::CreateLocalVar(){
	string Local;
	bool CheckResult = 0;
	do
	{
		Local = "T" + to_string(myGoListener::LocalIndex);
		myGoListener::LocalIndex ++;
		shared_ptr<Symbol> tmp;
		if(currentScope->resolve(Local,tmp)==SUCCESS){
			CheckResult=1; // 临时变量与已经定义的变量重名
		}
		else{
			CheckResult=0;
		}
	}
	while(CheckResult);
	/* TODO: 判断tmp类型 */
	std::shared_ptr<Symbol> symbol= make_shared<Symbol>(Local,currentScope,Symbol::SymbolType::VAR,defineTmpType());
	myGoListener::currentScope->para_define(symbol); 
	return Local;
}



string myGoListener::ToString(TACOP num){
	switch (num)
	{
		case TACOP::ADD:    return "ADD";
		case TACOP::SUB:    return "SUB";
		case TACOP::DIV:    return "DIV";
		case TACOP::MUL:    return "MUL";
		case TACOP::ASSIGN: return "ASSIGN";
		case TACOP::CALL:    return "CALL";
		case TACOP::PARA:    return "PARA";
		case TACOP::RET:    return "RET";
		case TACOP::ENDCALL:    return "ENDCALL";
		case TACOP::FUN_RET:    return "FUN_RET";
		default:  return "";
	}
}


void myGoListener::Go23file(string filename){
	ofstream outfile;
	outfile.open(filename, ios::out);
	outfile.clear();
	
	for(auto p : TACBlocks)
	{
		auto block=p.second;
		for(auto it: *block){

			outfile << setw(4) <<it.line << " Operater: " << setw(10) <<ToString(it.op) << " src1: " << setw(10) <<it.src1.value << "      src2:" << setw(10) << it.src2.value << "      dst: " <<setw(10) << it.dst.value << endl;
		}
		outfile<<"----------------------\n";
		
	}
	outfile.close();
}

// 第一个参数，改成函数名字
void myGoListener::push_line(TACOP op, Operand src1, Operand src2, Operand dst){
	TACBlocks[curFun]->push_back(TACLine(myGoListener::LineIndex, op, src1, src2, dst, currentScope));
	myGoListener::LineIndex ++;
}

void myGoListener::myPrint(std::shared_ptr<Scope> currentScope){
    for(auto it:currentScope->para_symbols){
		cout<<"para_symbol:";
        cout<<it.first<<endl;
    }
    for(auto it:currentScope->fun_symbols){
		cout<<"fun_symbol:";
        cout<<it.first<<endl;
    }
}

void myGoListener::addScope(){
	std::shared_ptr<Scope> scope(new Scope(currentScope));
	// scopes.put(ctx,scope);
	allScopes.push_back(scope);
	currentScope=scope;
}

void myGoListener::popScope(){
    myPrint(currentScope);
    currentScope=currentScope->enclosing_scope;
}

void myGoListener::exitPackageClause(GoParser::PackageClauseContext *ctx)
{
	
}
void myGoListener::enterPackageClause(GoParser::PackageClauseContext *ctx){}


void myGoListener::enterInteger(GoParser::IntegerContext *ctx){}
void myGoListener::exitInteger(GoParser::IntegerContext *ctx){
	values->put(ctx,ctx->DECIMAL_LIT()->getText());
}


void myGoListener::enterBasicLit(GoParser::BasicLitContext *ctx){}
void myGoListener::exitBasicLit(GoParser::BasicLitContext *ctx){
	if (ctx->integer()){
	
		string BasicLitValue = values->get(ctx->integer());
		values->put(ctx, BasicLitValue);
	}
}


void myGoListener::enterOperand(GoParser::OperandContext *ctx){}
void myGoListener::exitOperand(GoParser::OperandContext *ctx){
	if (ctx->literal()){
		string OperandValue = values->get(ctx->literal());
		values->put(ctx, OperandValue);
	}
	if (ctx->expression()){
		string OperandValue = values->get(ctx->expression());
		values->put(ctx, OperandValue);
	}
	if (ctx->operandName()){
		string OperandValue = values->get(ctx->operandName());
		values->put(ctx, OperandValue);
	}
}

void myGoListener::enterLiteral(GoParser::LiteralContext *ctx){
	
}
void myGoListener::exitLiteral(GoParser::LiteralContext *ctx){
	if (ctx->basicLit()){
		string LiteralValue = values->get(ctx->basicLit());

		values->put(ctx, LiteralValue+DELIMITER);
	}
}

void myGoListener::enterPrimaryExpr(GoParser::PrimaryExprContext *ctx){}
void myGoListener::exitPrimaryExpr(GoParser::PrimaryExprContext *ctx){
	
	if(ctx->arguments()){


		vector<string> primaryExpr_value;

		/*看是否定义*/
		shared_ptr<vector<string>> fun_identity;
		fun_identity=ctx_decoder(values->get(ctx->primaryExpr()));
		string identity=(*fun_identity)[0];
		std::shared_ptr<Symbol> fun_symbol;
		if(currentScope->resolve(identity,fun_symbol)==FAIL){
			cout<<"Undefined function: "<<identity<<endl;
			exit(-1);
		}
		/*找到了函数*/
		std::shared_ptr<vector<string>> arguments_values=ctx_decoder(values->get(ctx->arguments()));
		/*判断参数数量合理*/

		if(fun_symbol->fun_para_type_list->size()!=arguments_values->size()){
			cout<<"Wrong parameter number for : "<<identity<<endl;
			exit(-1);
		}

		/*函数调用*/
		string blank="";
		for(auto ret: *fun_symbol->fun_ret_type_list){
			// TODO:CreateLocalVar应当传入不同变量的type
			string tmp=CreateLocalVar();
			push_line (TACOP::RET, tmp, blank, blank);
			primaryExpr_value.push_back(tmp);
		}
		for(auto para: *arguments_values){
			push_line (TACOP::PARA, para, blank, blank);
		}
		push_line (TACOP::CALL, identity, blank, blank);
		values->put(ctx, ctx_encoder(primaryExpr_value));
		
	}
	else if (ctx->operand()){
		
		string PrimaryExprValue = values->get(ctx->operand());
		values->put(ctx, PrimaryExprValue);
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
	string ExpresionValue = values->get(ctx->primaryExpr());
	values->put(ctx, ExpresionValue);
}

void myGoListener::enterPlusMinusOperation(GoParser::PlusMinusOperationContext *ctx){}
void myGoListener::exitPlusMinusOperation(GoParser::PlusMinusOperationContext *ctx){
	std::shared_ptr<vector<string>> left=ctx_decoder(values->get(ctx->expression(0)));
	std::shared_ptr<vector<string>> right=ctx_decoder(values->get(ctx->expression(1)));
	/*判断value是否只有一个*/
	if(left->size()!=1 || right->size()!=1){
		cout<<"wrong literal number1"<<endl;
		exit(-1);
	}
	string dst = CreateLocalVar();
	vector<string> plusMinusOperation_values;
	plusMinusOperation_values.push_back(dst);

	if(ctx->PLUS())
	{
		push_line (TACOP::ADD, (*left)[0],  (*right)[0], dst);
	}
	else if(ctx->MINUS())
	{
		push_line (TACOP::SUB, (*left)[0],  (*right)[0], dst);
	}
	values->put(ctx, ctx_encoder(plusMinusOperation_values));
}

void myGoListener::enterRelationOperation(GoParser::RelationOperationContext *ctx){}
void myGoListener::exitRelationOperation(GoParser::RelationOperationContext *ctx){

}

void myGoListener::enterMulDivOperation(GoParser::MulDivOperationContext *ctx){}
void myGoListener::exitMulDivOperation(GoParser::MulDivOperationContext *ctx){
	std::shared_ptr<vector<string>> left=ctx_decoder(values->get(ctx->expression(0)));
	std::shared_ptr<vector<string>> right=ctx_decoder(values->get(ctx->expression(1)));
	/*判断value是否只有一个*/

	if(left->size()!=1 || right->size()!=1){
		cout<<"wrong literal number2"<<endl;
		exit(-1);
	}
	string dst = CreateLocalVar();
	vector<string> mulDivOperation_values;
	mulDivOperation_values.push_back(dst);

	if(ctx->STAR())
	{
		push_line (TACOP::MUL, (*left)[0],  (*right)[0], dst);
	}
	else if(ctx->DIV())
	{
		push_line (TACOP::DIV, (*left)[0],  (*right)[0], dst);
	}
	values->put(ctx, ctx_encoder(mulDivOperation_values));
	
}

void myGoListener::enterLogicalOrOperation(GoParser::LogicalOrOperationContext *ctx){}
void myGoListener::exitLogicalOrOperation(GoParser::LogicalOrOperationContext *ctx){

}

void myGoListener::enterLogicalAndOperation(GoParser::LogicalAndOperationContext *ctx){}
void myGoListener::exitLogicalAndOperation(GoParser::LogicalAndOperationContext *ctx){
	
}



void myGoListener::enterSourceFile(GoParser::SourceFileContext *ctx){
	globalScope=make_shared<Scope>();
	currentScope = globalScope;
	std::shared_ptr<TACBlock> currentBlock=make_shared<TACBlock>();
	TACBlocks[curFun]=currentBlock;
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
void myGoListener::exitExpressionList(GoParser::ExpressionListContext *ctx){
	vector<string> expression_values;
	for(int i=0;i<ctx->expression().size();++i){
		string s=values->get(ctx->expression(i));
		std::shared_ptr<vector<string>> vs=ctx_decoder(s);
		for(auto each: *vs){
			expression_values.push_back(each);
		}
	}
	values->put(ctx,ctx_encoder(expression_values));
}

void myGoListener::enterTypeDecl(GoParser::TypeDeclContext *ctx){}
void myGoListener::exitTypeDecl(GoParser::TypeDeclContext *ctx){}

void myGoListener::enterTypeSpec(GoParser::TypeSpecContext *ctx){}
void myGoListener::exitTypeSpec(GoParser::TypeSpecContext *ctx){}

void myGoListener::enterFunctionDecl(GoParser::FunctionDeclContext *ctx){
	string identifier=ctx->IDENTIFIER()->getText();
	/*判断是否函数名字重复*/
	if(currentScope->cur_resolve(identifier)==SUCCESS){
		cout<<"Redeclaration of funciton:"<<identifier<<endl;
		exit(-1);
	}

	/* 更改目前的function */
	curFun=identifier;
	std::shared_ptr<TACBlock> currentBlock=make_shared<TACBlock>();
	TACBlocks[curFun]=currentBlock;
	
	/* 构建fun symbol */
	std::shared_ptr<vector<Symbol::Type>> funRetTypeList=make_shared<vector<Symbol::Type>>();
	std::shared_ptr<vector<Symbol::Type>> funParaList=make_shared<vector<Symbol::Type>>();
	/*函数ret参数*/
	if(!ctx->signature()->result() || ctx->signature()->result()->parameters()->parameterDecl().size()==0){ // 无返回值
		// do nothing
	}
	else{  // 有返回值
		int n=ctx->signature()->result()->parameters()->parameterDecl().size();
		for(int i=0;i<n;++i){
			string eachType=ctx->signature()->result()->parameters()->parameterDecl(i)->type_()->typeName()->getText();
			Symbol::Type eachSType=Symbol::toType(eachType);
			funRetTypeList->push_back(eachSType);
		}
	}

	/*函数para参数*/
	//无参数
	if(ctx->signature()->parameters()->parameterDecl().size()==0){
		// do nothing
	}
	else{ //有参数
		int n=ctx->signature()->parameters()->parameterDecl().size();
		for(int i=0;i<n;++i){
			string eachType=ctx->signature()->parameters()->parameterDecl(i)->type_()->typeName()->getText();
			Symbol::Type eachSType=Symbol::toType(eachType);
			for(int j=0;j<ctx->signature()->parameters()->parameterDecl(i)->identifierList()->IDENTIFIER().size();++j){
				funParaList->push_back(eachSType);	
			}
		}	
	}
	
	
	std::shared_ptr<Symbol> symbol= make_shared<Symbol>(identifier,currentScope,funRetTypeList, funParaList);
	std::shared_ptr<Scope> scope=make_shared<Scope>(currentScope);
	myGoListener::currentScope->fun_define(symbol); 
	currentScope=scope;
	
}
void myGoListener::exitFunctionDecl(GoParser::FunctionDeclContext *ctx){
	curFun="global";
	popScope();
}

void myGoListener::enterMethodDecl(GoParser::MethodDeclContext *ctx){}
void myGoListener::exitMethodDecl(GoParser::MethodDeclContext *ctx){}

void myGoListener::enterReceiver(GoParser::ReceiverContext *ctx){}
void myGoListener::exitReceiver(GoParser::ReceiverContext *ctx){}

void myGoListener::enterVarDecl(GoParser::VarDeclContext *ctx){}
void myGoListener::exitVarDecl(GoParser::VarDeclContext *ctx){}

void myGoListener::enterVarSpec(GoParser::VarSpecContext *ctx){
	
}
void myGoListener::exitVarSpec(GoParser::VarSpecContext *ctx){
	int n = ctx->identifierList()->IDENTIFIER().size();

	for(int i=0;i<n;++i){
		string varname = ctx->identifierList()->IDENTIFIER(i)->getText();
		Symbol::Type type;
		if(!ctx->type_()){
			type=defineTmpType();
		}
		else{
			string stype = ctx->type_()->typeName()->getText();
			type = Symbol::toType(stype);
		}
		/*如果已经定义了，报错*/
		if(currentScope->cur_resolve(varname)==SUCCESS){
			cout<<"Redeclaration of parameter:"<<varname<<endl;
			exit(-1);
		}
		/*不重复，新建*/
		std::shared_ptr<Symbol> symbol=make_shared<Symbol>(varname,currentScope,Symbol::SymbolType::VAR,type);
		myGoListener::currentScope->para_define(symbol);
	}
	



	/* TODO:如果是函数调用怎么办？？ */
	if(ctx->expressionList()){
		shared_ptr<vector<string>> right_values;
		right_values=ctx_decoder(values->get(ctx->expressionList()));
		/*数量是否匹配*/
		if(n!=right_values->size()){
			cout<<"wrong number"<<endl;
			exit(-1);
		}
		/*赋值*/
		for(int i=0;i<n;++i){
			push_line(TACOP::ASSIGN, (*right_values)[i],Operand(""),ctx->identifierList()->IDENTIFIER(i)->getText());
		}
	}


}

void myGoListener::enterBlock(GoParser::BlockContext *ctx){
	
	addScope();
	
}
void myGoListener::exitBlock(GoParser::BlockContext *ctx){
	popScope();
	
	
}

void myGoListener::enterStatementList(GoParser::StatementListContext *ctx){}
void myGoListener::exitStatementList(GoParser::StatementListContext *ctx){}

void myGoListener::enterStatement(GoParser::StatementContext *ctx){

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
void myGoListener::exitAssignment(GoParser::AssignmentContext *ctx){
	

	if(ctx->assign_op()->getText() == "=")
	{
		std::shared_ptr<vector<string>> left_values;
		std::shared_ptr<vector<string>> right_values;
		left_values=ctx_decoder(values->get(ctx->expressionList(0)));
		right_values=ctx_decoder(values->get(ctx->expressionList(1)));
		/*左右参数量是否相等*/
		if(left_values->size()!=right_values->size()){
			cout<<"not equal number parameter for assign"<<endl;
			exit(-1);
		}

		/*ok*/
		for(int i = 0; i < left_values->size(); i++)
		{
			string varname = (*left_values)[i];
			string varvalue = (*right_values)[i];
			push_line (TACOP::ASSIGN, Operand(varvalue), Operand(""), Operand(varname));
		}
	}
}

void myGoListener::enterAssign_op(GoParser::Assign_opContext *ctx){}
void myGoListener::exitAssign_op(GoParser::Assign_opContext *ctx){}

void myGoListener::enterShortVarDecl(GoParser::ShortVarDeclContext *ctx){}
void myGoListener::exitShortVarDecl(GoParser::ShortVarDeclContext *ctx){}

void myGoListener::enterEmptyStmt(GoParser::EmptyStmtContext *ctx){}
void myGoListener::exitEmptyStmt(GoParser::EmptyStmtContext *ctx){}

void myGoListener::enterLabeledStmt(GoParser::LabeledStmtContext *ctx){}
void myGoListener::exitLabeledStmt(GoParser::LabeledStmtContext *ctx){}

void myGoListener::enterReturnStmt(GoParser::ReturnStmtContext *ctx){}
void myGoListener::exitReturnStmt(GoParser::ReturnStmtContext *ctx){
	std::shared_ptr<vector<string>> return_values;
	return_values=ctx_decoder(values->get(ctx->expressionList()));
	/*是否声明*/
	for(auto i:(*return_values)){
		shared_ptr<Symbol> tmp;
		if(currentScope->resolve(i,tmp)==FAIL){
			cout<<"Undefined : "<<i<<endl;
			exit(-1);
		}
	}
	/*生成TAC*/
	for(auto i:(*return_values)){
		push_line (TACOP::FUN_RET, i, Operand(""), Operand(""));
	}
}

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
	if(ctx->identifierList()){  /* x int */
		
		int n=ctx->identifierList()->IDENTIFIER().size();
		for(int i=0;i<n;++i){
			
			string integer=ctx->identifierList()->IDENTIFIER(i)->getText();
			// Symbol::Type type=ctx->type_()->typeName()->getText();
			string stype=ctx->type_()->typeName()->getText();
			Symbol::Type type=Symbol::toType(stype);
			std::shared_ptr<Symbol> symbol= make_shared<Symbol>(integer,currentScope,Symbol::SymbolType::VAR,type);
			myGoListener::currentScope->para_define(symbol);
		}
		
	}
	
}
void myGoListener::exitParameterDecl(GoParser::ParameterDeclContext *ctx){}

void myGoListener::enterConversion(GoParser::ConversionContext *ctx){}
void myGoListener::exitConversion(GoParser::ConversionContext *ctx){}

void myGoListener::enterNonNamedType(GoParser::NonNamedTypeContext *ctx){}
void myGoListener::exitNonNamedType(GoParser::NonNamedTypeContext *ctx){}



void myGoListener::enterOperandName(GoParser::OperandNameContext *ctx){}
void myGoListener::exitOperandName(GoParser::OperandNameContext *ctx){
	shared_ptr<Symbol> tmp;
	if(currentScope->resolve(ctx->IDENTIFIER()->getText(),tmp)==FAIL){
		cout<<"Undefined : "<<ctx->IDENTIFIER()->getText()<<endl;
		exit(-1);
	}
	values->put(ctx,ctx->IDENTIFIER()->getText()+DELIMITER);
}

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
void myGoListener::exitArguments(GoParser::ArgumentsContext *ctx){
	if(ctx->expressionList()){
		values->put(ctx, values->get(ctx->expressionList()));
	}
}

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
