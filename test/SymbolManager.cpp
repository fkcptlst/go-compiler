#include <iomanip>
#include "Common.h"
#include "TAC.h"
#include "TCG/SymbolManager.h"


void cal_use_info() {
	TACBlock block;
	block.emplace_back(TACLine(1, TACOP::ADD, "A", "B", "T"));
	block.emplace_back(TACLine(2, TACOP::SUB, "A", "C", "U"));
	block.emplace_back(TACLine(3, TACOP::ADD, "T", "U", "V"));
	block.emplace_back(TACLine(4, TACOP::ADD, "V", "U", "W"));

	std::cout << "TAC文件内容: " << std::endl;
	for (auto& line : block) {
		std::cout << line.to_string() << std::endl;
	}

	/* 建立符号表 */
	SymbolTabel symbol_tabel;
	symbol_tabel.emplace("A", VairableInfo(VairableKind::INT));
	symbol_tabel.emplace("B", VairableInfo(VairableKind::INT));
	symbol_tabel.emplace("C", VairableInfo(VairableKind::INT));
	symbol_tabel.emplace("T", VairableInfo(VairableKind::INT));
	symbol_tabel.emplace("U", VairableInfo(VairableKind::INT));
	symbol_tabel.emplace("V", VairableInfo(VairableKind::INT));
	symbol_tabel.emplace("W", VairableInfo(VairableKind::INT));

	/* 计算待用信息和活跃信息 */
	SymbolManager symbol_manager;
	symbol_tabel["W"].use_info.active = true;
	symbol_manager.cal_use_info(block, symbol_tabel);

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

int main() {
	cal_use_info();
	return 0;
}
