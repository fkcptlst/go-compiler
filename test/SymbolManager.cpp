#include <iomanip>
#include "Common/Common.h"
#include "Common/TAC.h"
#include "TCG/SymbolManager.h"


void test_init(TACBlock& block, SymbolTabel& symbol_table, SymbolManager& symbol_manager) {
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
	symbol_table.emplace("A", VairableInfo(VairableKind::INT));
	symbol_table.emplace("B", VairableInfo(VairableKind::INT));
	symbol_table.emplace("C", VairableInfo(VairableKind::INT));
	symbol_table.emplace("T", VairableInfo(VairableKind::INT));
	symbol_table.emplace("U", VairableInfo(VairableKind::INT));
	symbol_table.emplace("V", VairableInfo(VairableKind::INT));
	symbol_table.emplace("W", VairableInfo(VairableKind::INT));
}


void cal_use_info(TACBlock& block, SymbolTabel& symbol_table, SymbolManager& symbol_manager) {
	/* 计算待用信息和活跃信息 */
	symbol_table["W"].use_info.active = true;
	symbol_manager.cal_use_info(block, symbol_table);

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


void get_reg(TACBlock& block, SymbolTabel& symbol_table, SymbolManager& symbol_manager) {
	symbol_manager.set_variable("A", REG::EAX, 0);
	symbol_manager.set_variable("B", REG::EBX, 0);
	symbol_manager.set_variable("C", REG::ECX, 0);
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
	TACBlock block;
	SymbolTabel symbol_table;
	SymbolManager symbol_manager;
	test_init(block, symbol_table, symbol_manager);
	cal_use_info(block, symbol_table, symbol_manager);
	get_reg(block, symbol_table, symbol_manager);
	return 0;
}
