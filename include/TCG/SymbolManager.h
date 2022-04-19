#ifndef INCLUDE_TCG_SYMBOLMANAGER_H_
#define INCLUDE_TCG_SYMBOLMANAGER_H_

#include "Common/Common.h"
#include "SymbolTabel.h"
#include "Common/TAC.h"
#include "TCG/ASM.h"


class SymbolManager {
public:
	/* 返回寄存器 reg 的变量名, 无意义则返回 空串 */
	std::string rvalue(REG reg);

	/* 返回该变量的地址 内存地址, 寄存器地址(0 ~ regnums) */
	REG avalue_reg(const std::string& vairable);
	uint64_t avalue_mem(const std::string& vairable);
	void set_variable(const std::string& variable, REG reg=REG::None, uint64_t mem=0);

	/* 给 dst 变量分配一个寄存器 */
	REG get_reg(const TACLine& line);

	/* 计算一个代码块中, 变量的待用信息和活跃信息 */
	void cal_use_info(TACBlock& block, SymbolTabel& symbol_tabel);

private:
	/* 获得一个空间寄存器 */
	REG get_free_reg();

private:
	/* 寄存器描述数组 */
	std::string rvalue_[static_cast<int>(REG::None)];

	/* 变量地址描述数组 */
	std::unordered_map<std::string, REG> avalue_reg_;
	std::unordered_map<std::string, uint64_t> avalue_mem_;
};


inline REG SymbolManager::get_free_reg() {
	for (int i(0); i < static_cast<int>(REG::None); i++) {
		if (rvalue_[i] != "") {
			return static_cast<REG>(i);
		}
	}
	return REG::None;
}


inline std::string SymbolManager::rvalue(REG reg) {
	return rvalue_[static_cast<int>(reg)];
}


inline REG SymbolManager::avalue_reg(const std::string& vairable) {
	if (avalue_reg_.find(vairable) != avalue_reg_.end()) {
		return avalue_reg_[vairable];
	} else {
		return REG::None;
	}
}


inline uint64_t SymbolManager::avalue_mem(const std::string& vairable) {
	if (avalue_mem_.find(vairable) != avalue_mem_.end()) {
		return avalue_mem_[vairable];
	} else {
		return 0;
	}
}


inline void SymbolManager::set_variable(const std::string& variable, REG reg, uint64_t mem) {
	if (mem != 0) {
		avalue_mem_[variable] = mem;
	}
	if (reg != REG::None) {
		avalue_reg_[variable] = reg;
		rvalue_[static_cast<int>(reg)] = variable;
	}
}


#endif // INCLUDE_TCG_SYMBOLMANAGER_H_
