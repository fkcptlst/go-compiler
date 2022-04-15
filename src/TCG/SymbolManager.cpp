#include "TCG/SymbolManager.h"


REG SymbolManager::get_reg(const std::string& vairable) {
	/* crTODO */
	return REG::None;
}


void SymbolManager::cal_use_info(TACBlock& block, SymbolTabel& symbol_tabel) {
	/* crTODO: 基本块之后???? */

	/* 使用vector逆向迭代器 从后向前扫描三地址码 */
	for (auto it = block.rbegin(); it != block.rend(); ++it) {
		TACLine &line = *it;
		/* 把符号表中 dst 的待用信息和活跃信息 附加到 中间代码上 */
		line.dst.use_info = symbol_tabel[line.dst.value].use_info;
		/* 重置符号表 dst 的待用信息和活跃信息 */
		symbol_tabel[line.dst.value].use_info = {0, false};
		/* 把符号表中 src 的待用信息和活跃信息 附加到 中间代码上 */
		line.src1.use_info = symbol_tabel[line.src1.value].use_info;
		line.src2.use_info = symbol_tabel[line.src2.value].use_info;
		/* 置位符号表 src 的待用信息和活跃信息 */
		symbol_tabel[line.src1.value].use_info = {line.line, true};
		symbol_tabel[line.src2.value].use_info = {line.line, true};
	}
}
