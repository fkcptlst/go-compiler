#include <antlr4-runtime.h>
#include <Common/Common.h>
#include "Common/TAC.h"
#include "GoParser.h"
#include "GoLexer.h"
#include "GoParserListener.h"
#include "GoParserBaseListener.h"
#include "ICG/myGoListener.h"
#include "ICG/Utils.h"
#include "ICG/StmtICG/ForStmt.h"

using namespace std;

string myGoListener::CreateLocalVar(){
	string Local;
	bool CheckResult = 0;
	do
	{
		Local = "T" + to_string(myGoListener::LocalIndex);
		myGoListener::LocalIndex++;
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

string myGoListener::CreateElseLabel(){
	string Local;
	bool CheckResult = 0;
	Local = to_string(myGoListener::IFIndex);
	myGoListener::IFIndex++;
	return Local;
}

string myGoListener::CreateForLabel(){
	string Local;
	bool CheckResult = 0;
	Local = to_string(myGoListener::ForIndex);
	myGoListener::ForIndex++;
	return Local;
}

string myGoListener::ToString(TACOP num){
	switch (num)
	{
		case TACOP::ADD:    	return "ADD";
		case TACOP::SUB:    	return "SUB";
		case TACOP::DIV:    	return "DIV";
		case TACOP::MUL:   		return "MUL";
		case TACOP::ASSIGN: 	return "ASSIGN";
		case TACOP::CALL:    	return "CALL";
		case TACOP::PARA:    	return "PARA";
		case TACOP::RET:    	return "RET";
		case TACOP::ENDCALL:	return "ENDCALL";
		case TACOP::FUN_RET:	return "FUN_RET";
		case TACOP::FUN_PARA:	return "FUN_PARA";
		case TACOP::IFEXP:		return "IFEXP";
		case TACOP::ENDIF: 		return "ENDIF";
		case TACOP::IFGE:		return "IFGE";
		case TACOP::IFLE:		return "IFLE";
		case TACOP::IFGT:		return "IFGT";
		case TACOP::IFLT:		return "IFLT";
		case TACOP::IFEQ:		return "IFEQ";
		case TACOP::IFNEQ:		return "IFNEQ";
		case TACOP::GOTO:		return "GOTO";
		case TACOP::ELSE:		return "ELSE";
		case TACOP::LABEL: 		return "LABEL";
		case TACOP::CREATLIST: 		return "CREATLIST";
		default: 				return "";
	}
}

string myGoListener::ToString(TACOPERANDTYPE num){
	switch (num)
	{
		case TACOPERANDTYPE::VAR:      	return "VAR";
		case TACOPERANDTYPE::NULL_:    	return "NULL_";
		case TACOPERANDTYPE::IMM: 	   	return "IMM";
		case TACOPERANDTYPE::PTR: 	   	return "PTR";
		default:  						return "----";
	}
}

TACOPERANDTYPE myGoListener::OperandTypereslove(string name){
	if(ptrs[name]==1){
		return TACOPERANDTYPE::PTR;
	}
	if((name[0] >= '0' && name[0] <= '9') || name[0] == '-')
	{
		return TACOPERANDTYPE::IMM;
	}
	return TACOPERANDTYPE::VAR;
}

bool myGoListener::is_digit(string s){
	if(s[0]=='-') s=s.substr(1,s.size()-1);
	for(auto c:s){
		if((c>= '0' && c<= '9')) continue;
		else return false;
	}
	return true;
}

void myGoListener::Go23file(string filename){
	ofstream outfile;
	outfile.open(filename, ios::out);
	outfile.clear();

	for(auto p : TACBlocks)
	{
		auto block=p.second;
		for(auto it: *block){

			outfile << setw(4) <<it.line << " Operater: " << setw(10) <<ToString(it.op) << " src1: " <<  setw(7) <<ToString(it.src1.OperType) + ":" << setw(7) <<it.src1.value << "      src2:" <<  setw(7) <<ToString(it.src2.OperType) + ":" << setw(7) <<it.src2.value << "      dst: " <<  setw(7) <<ToString(it.dst.OperType) + ":" << setw(7) <<it.dst.value << endl;
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
    for(auto it:currentScope->para_symbols) {
		LOG(INFO) << "para_symbol: " << it.first << "currenScope: " <<currentScope;
    }
    for(auto it:currentScope->fun_symbols) {
		LOG(INFO) << "fun_symbol: " << it.first;
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
		// string dst = CreateLocalVar();
		string BasicLitValue = values->get(ctx->integer());
		// push_line(TACOP::ASSIGN, Operand(BasicLitValue,TACOPERANDTYPE::IMM),Operand("",TACOPERANDTYPE::NULL_), Operand(dst,TACOPERANDTYPE::VAR));
		// values->put(ctx, dst);
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

	/* 是函数调用 */
	if(ctx->arguments()){
		vector<string> primaryExpr_value;

		/*看是否定义*/
		shared_ptr<vector<string>> fun_identity;
		fun_identity=ctx_decoder(values->get(ctx->primaryExpr()));
		string identity=(*fun_identity)[0];
		std::shared_ptr<Symbol> fun_symbol;
		if(currentScope->resolve(identity,fun_symbol)==FAIL){
			LOG(FATAL) << "Undefined function: " << identity;
			cout << "Undefined function: " << identity<<endl;
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
			push_line (TACOP::RET, Operand(tmp, OperandTypereslove(tmp)), Operand(blank, TACOPERANDTYPE::NULL_), Operand(blank, TACOPERANDTYPE::NULL_));
			primaryExpr_value.push_back(tmp);
		}
		for(auto para: *arguments_values){
			push_line (TACOP::PARA, Operand(para, OperandTypereslove(para)), Operand(blank, TACOPERANDTYPE::NULL_), Operand(blank, TACOPERANDTYPE::NULL_));
		}
		push_line (TACOP::CALL, Operand(identity, OperandTypereslove(identity)), Operand(blank, TACOPERANDTYPE::NULL_), Operand(blank, TACOPERANDTYPE::NULL_));
		values->put(ctx, ctx_encoder(primaryExpr_value));

	}
	/*数*/
	else if (ctx->operand()){

		string PrimaryExprValue = values->get(ctx->operand());
		values->put(ctx, PrimaryExprValue);
	}
	/* 数组 */
	else if(ctx->index()){
		shared_ptr<vector<string>> array_name;
		array_name=ctx_decoder(values->get(ctx->primaryExpr()));
		string identity=(*array_name)[0];

		// 名字不是数组而是变量
		std::shared_ptr<Symbol> array_symbol;
		currentScope->resolve(identity,array_symbol);
		cout<<"0000 "<<array_symbol->name<<endl;
		cout<<"1111 "<<array_symbol->array_length<<endl;
		cout<<"2222 "<<array_symbol->is_array<<endl;
		if(!array_symbol->is_array){
			cout << "Only array can be indexed: " << identity<<endl;
			exit(-1);
		}		

		
		shared_ptr<vector<string>> array_index;
		array_index=ctx_decoder(values->get(ctx->index()->expression()));
		string index_s=(*array_index)[0];
		if( 1 !=array_index->size()){
			cout<<"wrong number of array index input"<<endl;
			exit(-1);
		}

		// 若编译时可判断数组越界
		if(is_digit(index_s)){
			int idx = std::stoi(index_s);
			if(idx > array_symbol->array_length-1 || idx<0){
				cout << "Array index out of bound: " << identity<<endl;
				cout<<idx<<' '<<array_symbol->array_length<<endl;
				exit(-1);
			}
		}

		string tmp_ptr_offset = CreateLocalVar();
		string int_size="4";
		push_line (TACOP::MUL, 
				   Operand(int_size, OperandTypereslove(int_size)), 
				   Operand(index_s, OperandTypereslove(index_s)),
				   Operand(tmp_ptr_offset, OperandTypereslove(tmp_ptr_offset))
		);
		string tmp_ptr = CreateLocalVar();
		push_line (TACOP::ADD, 
				   Operand(identity,OperandTypereslove(identity)), 
				   Operand(tmp_ptr_offset, OperandTypereslove(tmp_ptr_offset)), 
				   Operand(tmp_ptr, OperandTypereslove(tmp_ptr))
		);
		//todo map
		ptrs[tmp_ptr]=1;

		values->put(ctx, tmp_ptr);
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
		push_line (TACOP::ADD, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
	}
	else if(ctx->MINUS())
	{
		push_line (TACOP::SUB, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
	}
	values->put(ctx, ctx_encoder(plusMinusOperation_values));
}

void myGoListener::enterRelationOperation(GoParser::RelationOperationContext *ctx){}
void myGoListener::exitRelationOperation(GoParser::RelationOperationContext *ctx){
	std::shared_ptr<vector<string>> left=ctx_decoder(values->get(ctx->expression(0)));
	std::shared_ptr<vector<string>> right=ctx_decoder(values->get(ctx->expression(1)));
	if(left->size()!=1 || right->size()!=1){
		cout<<"wrong literal number1"<<endl;
		exit(-1);
	}
	// vector<string> plusMinusOperation_values;
	// plusMinusOperation_values.push_back(dst);

	// expression 在for 下的情况，需要添加ifexp 并且反过来 将condition_loop信息写入forstatement里面
	if(ctx->parent->parent->children[0]->getText() == "for" && ctx->parent->parent->children[1] == ctx->parent)
	{
		std::string dst,dst_;
		ForStmt tmp = forvalues->get(ctx->parent->parent);
		dst = "ENDFOR" + tmp.CurIndex;
		dst_ = "FORLOOP" + tmp.CurIndex;
		TACLine tmpline;
		if(ctx->EQUALS())
		{
			tmpline = TACLine(myGoListener::LineIndex, TACOP::IFEQ, Operand((*left)[0], OperandTypereslove((*left)[0])), Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst_, OperandTypereslove(dst_)), currentScope);

			push_line (TACOP::IFNEQ, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->NOT_EQUALS())
		{
			tmpline = TACLine(myGoListener::LineIndex, TACOP::IFNEQ, Operand((*left)[0], OperandTypereslove((*left)[0])), Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst_, OperandTypereslove(dst_)), currentScope);

			push_line (TACOP::IFEQ, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->GREATER())
		{
			tmpline = TACLine(myGoListener::LineIndex, TACOP::IFGT, Operand((*left)[0], OperandTypereslove((*left)[0])), Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst_, OperandTypereslove(dst_)), currentScope);

			push_line (TACOP::IFLE, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->GREATER_OR_EQUALS())
		{
			tmpline = TACLine(myGoListener::LineIndex, TACOP::IFGE, Operand((*left)[0], OperandTypereslove((*left)[0])), Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst_, OperandTypereslove(dst_)), currentScope);

			push_line (TACOP::IFLT, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->LESS())
		{
			tmpline = TACLine(myGoListener::LineIndex, TACOP::IFLT, Operand((*left)[0], OperandTypereslove((*left)[0])), Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst_, OperandTypereslove(dst_)), currentScope);

			push_line (TACOP::IFGE, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->LESS_OR_EQUALS())
		{
			tmpline = TACLine(myGoListener::LineIndex, TACOP::IFLE, Operand((*left)[0], OperandTypereslove((*left)[0])), Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst_, OperandTypereslove(dst_)), currentScope);

			push_line (TACOP::IFGT, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		tmp.LoopCon = tmpline;
		forvalues->put(ctx->parent->parent, tmp);
	}

	// expression 在if 下的情况，需要添加ifexp 并且反过来
	if(ctx->parent->getText().find("if") == 0 && ctx == ctx->parent->children[1])
	{
		std::string dst;
		if(ctx->parent->getText().find("else") != -1)//有else，就goto else
		{
			dst = "ELSE" + ifvalues->get(ctx->parent);
		}
		else
		{
			dst = "ENDIF" + ifvalues->get(ctx->parent);
		}

		if(ctx->EQUALS())
		{
			push_line (TACOP::IFNEQ, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->NOT_EQUALS())
		{
			push_line (TACOP::IFEQ, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->GREATER())
		{
			push_line (TACOP::IFLE, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->GREATER_OR_EQUALS())
		{
			push_line (TACOP::IFLT, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->LESS())
		{
			push_line (TACOP::IFGE, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		else if(ctx->LESS_OR_EQUALS())
		{
			push_line (TACOP::IFGT, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
		}
		// values->put(ctx, ctx_encoder(plusMinusOperation_values));

		// push_line (TACOP::IFEXP, Operand(dst, OperandTypereslove(dst)), Operand("",TACOPERANDTYPE::NULL_), Operand("",TACOPERANDTYPE::NULL_));


	}

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
		push_line (TACOP::MUL, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
	}
	else if(ctx->DIV())
	{
		push_line (TACOP::DIV, Operand((*left)[0], OperandTypereslove((*left)[0])),  Operand((*right)[0], OperandTypereslove((*right)[0])), Operand(dst, OperandTypereslove(dst)));
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

void myGoListener::exitSourceFile(GoParser::SourceFileContext *ctx){
	cout<<"exit source file"<<endl;
}

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

	std::shared_ptr<Symbol> symbol= make_shared<Symbol>(identifier,currentScope,Symbol::SymbolType::FUN,funRetTypeList, funParaList);
	std::shared_ptr<Scope> scope=make_shared<Scope>(currentScope);
	myGoListener::currentScope->fun_define(symbol);
	currentScope=scope;

	/*打印 FUN_PARA*/
	for(int i=0;i<ctx->signature()->parameters()->parameterDecl().size();++i){
		// int n=ctx->signature()->parameters()->parameterDecl()->identifierList()->IDENTIFIER().size();
		int para_number=ctx->signature()->parameters()->parameterDecl(i)->identifierList()->IDENTIFIER().size();
		for(int j=0;j<para_number;++j){
			std::string fun_para = ctx->signature()->parameters()->parameterDecl(i)->identifierList()->IDENTIFIER(j)->getText();
			push_line (TACOP::FUN_PARA, Operand(fun_para, OperandTypereslove(fun_para)), Operand("", TACOPERANDTYPE::NULL_), Operand("", TACOPERANDTYPE::NULL_));
		}
	}

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
	/*是否是数组*/
	bool is_array=0;
	int array_length=0;
	/*如果是数组*/
	if(ctx->type_()->typeLit()){
		is_array=1;
		/*如果是数组，长度是多少*/
		shared_ptr<vector<string>> right_values;
		right_values=ctx_decoder(values->get(ctx->type_()->typeLit()->arrayType()->arrayLength()->expression()));
		// 数量是否是1
		if( 1 !=right_values->size()){
			cout<<"wrong number of array length input"<<endl;
			exit(-1);
		}
		string array_length_s=(*right_values)[0];
	
		if(is_digit(array_length_s)){
			array_length=std::stoi(array_length_s);
			// 数组长度不为负数
			if(array_length<1){
				cout<<"array length should >=1"<<endl;
				exit(-1);
			}
		}
		else{
			cout<<"array decleration need static capacity"<<endl;
			exit(-1);
		}
	}

	int n = ctx->identifierList()->IDENTIFIER().size();
	for(int i=0;i<n;++i){
		string varname = ctx->identifierList()->IDENTIFIER(i)->getText();
		Symbol::Type type;
		if(!ctx->type_()){
			type=defineTmpType();
		}
		else{
			/* 变量 */
			if(ctx->type_()->typeName()){
				string stype = ctx->type_()->typeName()->getText();
				type = Symbol::toType(stype);
			}
			/* 数组 */
			else if(ctx->type_()->typeLit() && ctx->type_()->typeLit()->arrayType()){
				string stype = ctx->type_()->typeLit()->arrayType()->elementType()->getText();
				type = Symbol::toType(stype);
			}
			
		}
				
		/*如果已经定义了，报错*/
		if(currentScope->cur_resolve(varname)){
			cout<<"Redeclaration of parameter:"<<varname<<endl;
			exit(-1);
		}
		/*不重复，新建*/
		std::shared_ptr<Symbol> symbol=make_shared<Symbol>(varname,currentScope,Symbol::SymbolType::VAR,type,is_array,array_length);
		if(is_array){
			push_line (TACOP::CREATLIST, Operand(varname, OperandTypereslove(varname)), Operand(std::to_string(array_length), OperandTypereslove(std::to_string(array_length))), Operand("INT", TACOPERANDTYPE::NULL_));
		}
		myGoListener::currentScope->para_define(symbol);
	}

	/*定义时赋值*/
	// 仅仅有变量
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
			push_line(TACOP::ASSIGN, Operand((*right_values)[i], OperandTypereslove((*right_values)[i])),Operand("", TACOPERANDTYPE::NULL_),Operand(ctx->identifierList()->IDENTIFIER(i)->getText(), OperandTypereslove(ctx->identifierList()->IDENTIFIER(i)->getText())));
		}
	}
	// TODO:数组compositeLit


}

void myGoListener::enterBlock(GoParser::BlockContext *ctx){
	//for 情况
	if(ctx->parent->children[0]->getText() == "for")
	{
		push_line(TACOP::LABEL,Operand("FORLOOP" + forvalues->get(ctx->parent).CurIndex, TACOPERANDTYPE::NULL_),Operand("", TACOPERANDTYPE::NULL_),Operand("", TACOPERANDTYPE::NULL_));
	}
	//if else 情况
	if (ctx->parent->children[4] == ctx && ctx->parent->children[3]->getText() == "else"){
		push_line(TACOP::LABEL,Operand("ELSE" + ifvalues->get(ctx->parent), TACOPERANDTYPE::NULL_),Operand("", TACOPERANDTYPE::NULL_),Operand("", TACOPERANDTYPE::NULL_));
	}
	addScope();

}
void myGoListener::exitBlock(GoParser::BlockContext *ctx){
	//for 情况
	if(ctx->parent->children[0]->getText() == "for")
	{
		ForStmt fortmp = forvalues->get(ctx->parent);
		TACLine updateCondition = fortmp.UpdateCon;
		TACLine loopCondition = fortmp.LoopCon;
		push_line(updateCondition.op, updateCondition.src1, updateCondition.src2, updateCondition.dst);
		push_line(loopCondition.op, loopCondition.src1, loopCondition.src2, loopCondition.dst);

	}

	//if 情况
	if (ctx->parent->children[2] == ctx && ctx->parent->children[0]->getText() == "if"){
		push_line(TACOP::GOTO,Operand("ENDIF" + ifvalues->get(ctx->parent), TACOPERANDTYPE::NULL_),Operand("", TACOPERANDTYPE::NULL_),Operand("", TACOPERANDTYPE::NULL_));
	}
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
void myGoListener::exitIncDecStmt(GoParser::IncDecStmtContext *ctx){
	if (ctx->children[1]->getText() == "++")
	{
		std::shared_ptr<vector<string>> left_values;
		left_values=ctx_decoder(values->get(ctx->expression()));
		if(left_values->size()!=1){
			cout<<"too many parameter for incdec \"++\""<<endl;
			exit(-1);
		}

		//for statement 的情况，不写入3code，但是要记录在forstmt里面，作为update的条件
		if(ctx->parent->parent->parent->children[0]->getText() == "for" && ctx->parent->parent == ctx->parent->parent->parent->children[1])
		{
			string varname = (*left_values)[0];
			string varvalue = "1";
			ForStmt fortmp =  forvalues->get(ctx->parent->parent->parent);
			TACLine tmpline = TACLine(myGoListener::LineIndex, TACOP::ADD, Operand(varname, OperandTypereslove(varname)), Operand(varvalue, OperandTypereslove(varvalue)), Operand(varname, OperandTypereslove(varname)), currentScope);
			fortmp.UpdateCon = tmpline;
			forvalues->put(ctx->parent->parent->parent, fortmp);
		}
		//普通情况，block里面，写入3code
		else
		{
			string varname = (*left_values)[0];
			string varvalue = "1";
			push_line (TACOP::ADD, Operand(varname, OperandTypereslove(varname)), Operand(varvalue, OperandTypereslove(varvalue)), Operand(varname, OperandTypereslove(varname)));

		}
	}
}

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

			// // 数组赋值
			// if(varname[varname.size()-1]==']'){
			// 	tmp s="[";
			// 	int identity_end=varname.find(tmp);

			// }
			// // 普通变量赋值
			// else{
			// cout<<"zyx:"<<"OperandTypereslove(varname):"<<ToString(OperandTypereslove(varname))<<endl;
			push_line (TACOP::ASSIGN, Operand(varvalue, OperandTypereslove(varvalue)), Operand("", TACOPERANDTYPE::NULL_), Operand(varname, OperandTypereslove(varname)));
			// }
			
		}
	}
	// 未实现数组的
	if(ctx->assign_op()->getText() == "+=")
	{
		std::shared_ptr<vector<string>> left_values;
		std::shared_ptr<vector<string>> right_values;
		left_values=ctx_decoder(values->get(ctx->expressionList(0)));
		right_values=ctx_decoder(values->get(ctx->expressionList(1)));
		/*左右参数量是否相等*/
		if(left_values->size()!=1 || right_values->size() != 1){
			cout<<"too many parameter for assign \"+=\""<<endl;
			exit(-1);
		}

		//for statement 的情况，不写入3code，但是要记录在forstmt里面，作为update的条件
		if(ctx->parent->parent->parent->children[0]->getText() == "for" && ctx->parent->parent == ctx->parent->parent->parent->children[1])
		{
			string varname = (*left_values)[0];
			string varvalue = (*right_values)[0];
			ForStmt fortmp =  forvalues->get(ctx->parent->parent->parent);
			TACLine tmpline = TACLine(myGoListener::LineIndex, TACOP::ADD, Operand(varname, OperandTypereslove(varname)), Operand(varvalue, OperandTypereslove(varvalue)), Operand(varname, OperandTypereslove(varname)), currentScope);
			fortmp.UpdateCon = tmpline;
			forvalues->put(ctx->parent->parent->parent, fortmp);
		}
		//普通情况，block里面，写入3code
		else
		{
			string varname = (*left_values)[0];
			string varvalue = (*right_values)[0];
			push_line (TACOP::ADD, Operand(varname, OperandTypereslove(varname)), Operand(varvalue, OperandTypereslove(varvalue)), Operand(varname, OperandTypereslove(varname)));

		}
	}
}

void myGoListener::enterAssign_op(GoParser::Assign_opContext *ctx){}
void myGoListener::exitAssign_op(GoParser::Assign_opContext *ctx){}

void myGoListener::enterShortVarDecl(GoParser::ShortVarDeclContext *ctx){}
void myGoListener::exitShortVarDecl(GoParser::ShortVarDeclContext *ctx){
	int n = ctx->identifierList()->IDENTIFIER().size();

	for(int i=0;i<n;++i){
		string varname = ctx->identifierList()->IDENTIFIER(i)->getText();
		Symbol::Type type;
		type=defineTmpType();
		/*如果已经定义了，报错*/
		// 不报错
		
		// if(currentScope->cur_resolve(varname)==SUCCESS){
		// 	cout<<"Redeclaration of parameter:"<<varname<<endl;
		// 	exit(-1);
		// }
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
			push_line(TACOP::ASSIGN, Operand((*right_values)[i], OperandTypereslove((*right_values)[i])),Operand("", TACOPERANDTYPE::NULL_),Operand(ctx->identifierList()->IDENTIFIER(i)->getText(), OperandTypereslove(ctx->identifierList()->IDENTIFIER(i)->getText())));
		}
	}
	if (ctx->parent->parent->parent->children[0]->getText() == "for" && ctx->parent->parent->parent->children[1] == ctx->parent->parent)
	{
		//init 变量添加进forstmt结构体，方便在forstmt结束后销毁变量
		ForStmt tmp = forvalues->get(ctx->parent->parent->parent);
		for(int i=0;i<n;++i){
		string varname = ctx->identifierList()->IDENTIFIER(i)->getText();
		tmp.newParas.push_back(varname);
		}
		forvalues->put(ctx->parent->parent->parent, tmp);
	}
}

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
			LOG(FATAL) << "Undefined: " << i;
		}
	}
	/*生成TAC*/
	for(auto i:(*return_values)){
		push_line (TACOP::FUN_RET, Operand(i, OperandTypereslove(i)), Operand("", TACOPERANDTYPE::NULL_), Operand("", TACOPERANDTYPE::NULL_));
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

void myGoListener::enterIfStmt(GoParser::IfStmtContext *ctx){
	std::string iftmp = CreateElseLabel();
	ifvalues->put(ctx, iftmp);
}
void myGoListener::exitIfStmt(GoParser::IfStmtContext *ctx){
	push_line(TACOP::LABEL,Operand("ENDIF" + ifvalues->get(ctx),TACOPERANDTYPE::NULL_),Operand("",TACOPERANDTYPE::NULL_),Operand("",TACOPERANDTYPE::NULL_));
}

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

void myGoListener::enterForStmt(GoParser::ForStmtContext *ctx){
	std::string tmp = CreateForLabel();
	ForStmt newfor = ForStmt(tmp);
	cout << newfor.CurIndex<<endl;
	forvalues->put(ctx, newfor);
	addScope();
}
void myGoListener::exitForStmt(GoParser::ForStmtContext *ctx){
	ForStmt tmp = forvalues->get(ctx);
	popScope();
	// for (auto para : tmp.newParas)
	// {
	// 	currentScope->para_delete(para);
	// }
	push_line(TACOP::LABEL,Operand("ENDFOR" + forvalues->get(ctx).CurIndex,TACOPERANDTYPE::NULL_),Operand("",TACOPERANDTYPE::NULL_),Operand("",TACOPERANDTYPE::NULL_));
}

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
		LOG(FATAL) << "Undefined: " << ctx->IDENTIFIER()->getText();
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
