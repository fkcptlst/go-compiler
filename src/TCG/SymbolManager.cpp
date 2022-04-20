#include "TCG/SymbolManager.h"
#include "Common/Scope.h"


/* 给 dst 变量分配一个寄存器 */
REG SymbolManager::get_reg(const TACLine& line) {
	/* crTODO: 在 translator 中, 也存在修改 两个数组的情况!!! */
	/* crTODO: 目前默认认为如果变量移除寄存器, 可以保证该变量以后不会使用 或 该变量内存中有位置 */
	if (avalue_reg(line.src1.value) != REG::None) {
		/* 使用 src1 的 寄存器 */
		avalue_reg_[line.dst.value] = avalue_reg(line.src1.value);
		return avalue_reg(line.dst.value);
	}

	/* 用空闲寄存器 */
	REG reg = get_free_reg();
	if (reg != REG::None) {
		avalue_reg_[line.dst.value] = reg;
		return reg;
	}

	/* crTODO: 替换寄存器 */
	return REG::None;
}


/* 计算一个代码块中, 变量的待用信息和活跃信息 */
void SymbolManager::cal_use_info() {
	/* crTODO: 基本块之后???? */

	/* 使用vector逆向迭代器 从后向前扫描三地址码 */
	for (auto it = scope.block.rbegin(); it != scope.block.rend(); it++) {
		TACLine &line = *it;
		/* 把符号表中 dst 的待用信息和活跃信息 附加到 中间代码上 */
		line.dst.use_info = use_info_[line.dst.value];
		/* 重置符号表 dst 的待用信息和活跃信息 */
		use_info_[line.dst.value] = {0, false};
		/* 把符号表中 src 的待用信息和活跃信息 附加到 中间代码上 */
		line.src1.use_info = use_info_[line.src1.value];
		line.src2.use_info = use_info_[line.src2.value];
		/* 置位符号表 src 的待用信息和活跃信息 */
		use_info_[line.src1.value] = {line.line, true};
		use_info_[line.src2.value] = {line.line, true};
	}
}
