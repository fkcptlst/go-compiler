#ifndef INCLUDE_TCG_SYMBOLMANAGER_H_
#define INCLUDE_TCG_SYMBOLMANAGER_H_

#include "Common/Common.h"
#include "Common/Scope.h"
#include "Common/TAC.h"
#include "TCG/ASM.h"


class SymbolManager {
public:
	SymbolManager() = delete;
	SymbolManager(Scope& scope) : scope(scope) {}

	/* get and set */
	std::string rvalue(REG reg);
	REG 		avalue_reg(const std::string& vairable) const;
	uint64_t 	avalue_mem(const std::string& vairable) const;
	UseInfo		use_info(const std::string& vairable) const;
	void		set_avalue_reg(const std::string& vairable, REG reg=REG::None);
	void		set_avalue_mem(const std::string& variable, uint64_t mem=-1);
	void		set_use_info(const std::string& vairable, UseInfo use_info);

	REG get_reg(const TACLine& line);
	void cal_use_info();

	/* 当接受一个函数的三地址代码块时，重新初始化 */
	void init_block(const std::string block_name);

private:
	REG get_free_reg();

	std::string                                 rvalue_[static_cast<int>(REG::None)];
	std::unordered_map<std::string, REG> 		avalue_reg_;
	std::unordered_map<std::string, uint64_t> 	avalue_mem_;
	std::unordered_map<std::string, UseInfo> 	use_info_;
	Scope &scope;

	/* 函数堆栈模拟 */
	int stack_ebp;
	int stack_esp;
};

inline UseInfo SymbolManager::use_info(const std::string& vairable) const {
	std::cout << "const" << std::endl;
	try {
		return use_info_.at(vairable);
	} catch (std::out_of_range& e) {
		return {0, false};
	}
}


inline void SymbolManager::set_use_info(const std::string& vairable, UseInfo use_info) {
	use_info_[vairable] = use_info;
}


/* 获得一个空闲寄存器 */
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


inline REG SymbolManager::avalue_reg(const std::string& vairable) const {
	try {
		return avalue_reg_.at(vairable);
	} catch (std::out_of_range& e) {
		return REG::None;
	}
}


/* 返回 变量 的内存地址, 如果没有返回 uint64(-1) */
inline uint64_t SymbolManager::avalue_mem(const std::string& vairable) const {
	try {
		return avalue_mem_.at(vairable);
	} catch (std::out_of_range& e) {
		return uint64_t(-1);
	}
}


inline void SymbolManager::set_avalue_reg(const std::string& vairable, REG reg) {
	avalue_reg_[vairable] = reg;
}


inline void SymbolManager::set_avalue_mem(const std::string& variable, uint64_t mem) {
	avalue_mem_[variable] = mem;
}

#endif // INCLUDE_TCG_SYMBOLMANAGER_H_
