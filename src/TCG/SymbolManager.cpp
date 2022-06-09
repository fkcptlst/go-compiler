#include "TCG/SymbolManager.h"
#include "Common/Scope.h"

SymbolManager::SymbolManager(std::shared_ptr<Scope> Global_Scope, std::string name) {
	this->Global_Scope = Global_Scope;
	this->Local_Scope = nullptr;
	std::shared_ptr<Symbol> fun = Global_Scope->resolve(name);
	this->func = fun;
	this->ret_num = fun->fun_ret_type_list->size();
	this->para_num = fun->fun_para_type_list->size();
	this->stack_esp = 0;
}

/* 给 dst 变量分配一个寄存器 */
REG SymbolManager::get_reg(std::string dst, std::string src1, std::string src2) {
	/* crTODO: 在 translator 中, 也存在修改 两个数组的情况!!! */
	/* crTODO: 目前默认认为如果变量移除寄存器, 可以保证该变量以后不会使用 或 该变量内存中有位置 */
	if (avalue_reg(src1) != REG::None) {
		/* 使用 src1 的 寄存器 */
		avalue_reg_[dst] = avalue_reg(src1);
		return avalue_reg(dst);
	}

	/* 用空闲寄存器 */
	REG reg = get_free_reg();
	if (reg != REG::None) {
		avalue_reg_[dst] = reg;
		return reg;
	}

	return REG::None;
}

REG SymbolManager::get_reg() {
	// todo 先这么写着
	return REG::ESI;
}


/* 计算一个代码块中, 变量的待用信息和活跃信息 */
void SymbolManager::cal_use_info() {
	/* crTODO: 基本块之后???? */

	/* 使用vector逆向迭代器 从后向前扫描三地址码 */
	// 等scope写好再写
	// for (auto it = scope.block.rbegin(); it != scope.block.rend(); it++) {
	// 	TACLine &line = *it;
	// 	/* 把符号表中 dst 的待用信息和活跃信息 附加到 中间代码上 */
	// 	line.dst.use_info = use_info_[line.dst.value];
	// 	/* 重置符号表 dst 的待用信息和活跃信息 */
	// 	use_info_[line.dst.value] = {0, false};
	// 	/* 把符号表中 src 的待用信息和活跃信息 附加到 中间代码上 */
	// 	line.src1.use_info = use_info_[line.src1.value];
	// 	line.src2.use_info = use_info_[line.src2.value];
	// 	/* 置位符号表 src 的待用信息和活跃信息 */
	// 	use_info_[line.src1.value] = {line.line, true};
	// 	use_info_[line.src2.value] = {line.line, true};
	// }
}

std::string SymbolManager::encode_var(std::string name) {
	std::shared_ptr<Symbol> symbol = Local_Scope->resolve(name);
	return std::to_string(uint64_t(symbol->scope.get())) + ":" + name;
}

