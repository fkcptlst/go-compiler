#include <iomanip>
#include "Common/Common.h"
#include "Common/Scope.h"
#include "Common/TAC.h"
#include "TCG/SymbolManager.h"


void test_init(Scope& scope, SymbolManager& symbol_manager) {
	TACBlock &block = scope.block;
	/* TAC 文件 */
	block.emplace_back(TACLine(1, TACOP::ADD, Operand("A"), Operand("B"), Operand("T")));
	block.emplace_back(TACLine(2, TACOP::SUB, Operand("A"), Operand("C"), Operand("U")));
	block.emplace_back(TACLine(3, TACOP::ADD, Operand("T"), Operand("U"), Operand("V")));
	block.emplace_back(TACLine(4, TACOP::ADD, Operand("V"), Operand("U"), Operand("W")));

	std::cout << "TAC文件内容: " << std::endl;
	for (auto& line : block) {
		std::cout << line.to_string() << std::endl;
	}

	/* 建立符号表 */
	scope.symbols.emplace("A", Symbol("A", &scope, Symbol::SymbolType::VAR, Symbol::Type::INT));
	scope.symbols.emplace("B", Symbol("B", &scope, Symbol::SymbolType::VAR, Symbol::Type::INT));
	scope.symbols.emplace("C", Symbol("C", &scope, Symbol::SymbolType::VAR, Symbol::Type::INT));
	scope.symbols.emplace("T", Symbol("T", &scope, Symbol::SymbolType::VAR, Symbol::Type::INT));
	scope.symbols.emplace("U", Symbol("U", &scope, Symbol::SymbolType::VAR, Symbol::Type::INT));
	scope.symbols.emplace("V", Symbol("V", &scope, Symbol::SymbolType::VAR, Symbol::Type::INT));
	scope.symbols.emplace("W", Symbol("W", &scope, Symbol::SymbolType::VAR, Symbol::Type::INT));
}


void cal_use_info(Scope& scope, SymbolManager& symbol_manager) {
	TACBlock &block = scope.block;
	/* 计算待用信息和活跃信息 */
	symbol_manager.set_use_info("W", {0, true});
	symbol_manager.cal_use_info();

	/* 打印结果 */
	std::cout << "待用信息和活跃信息: " << std::endl;
	for (TACLine &line : block) {
		std::cout << line.to_string()
				  << std::setw(8) << "(" << line.dst.use_info << ")"
				  << std::setw(8) << "(" << line.src1.use_info << ")"
				  << std::setw(8) << "(" << line.src2.use_info << ")"
				  << std::endl;
	}
}


void get_reg(Scope& scope, SymbolManager& symbol_manager) {
	TACBlock &block = scope.block;
	symbol_manager.set_avalue_reg("A", REG::EAX);
	symbol_manager.set_avalue_reg("B", REG::EBX);
	symbol_manager.set_avalue_reg("C", REG::ECX);
	std::cout << "获取寄存器: " << std::endl;
	for (TACLine &line : block) {
		symbol_manager.get_reg(line);
		std::cout << line.to_string()
				  << std::setw(8) << to_string(symbol_manager.avalue_reg(line.src1.value))
				  << std::setw(8) << to_string(symbol_manager.avalue_reg(line.src2.value))
				  << std::setw(8) << to_string(symbol_manager.avalue_reg(line.dst.value))
				  << std::endl;
	}
}


int main() {
	Scope scope;
	SymbolManager symbol_manager(scope);
	test_init(scope, symbol_manager);
	cal_use_info(scope, symbol_manager);
	get_reg(scope, symbol_manager);
	return 0;
}
