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
	int 	avalue_mem(const std::string& vairable) const;
	UseInfo		use_info(const std::string& vairable) const;
	void		set_avalue_reg(const std::string& vairable, REG reg);  // 让一个变量存在reg里面
	void		set_avalue_mem(const std::string& variable, int mem);  // 让一个变量存在堆栈里，用跟ebp的偏移
	void		set_use_info(const std::string& vairable, UseInfo use_info);

	REG get_reg(const TACLine& line);  // 解决a = b op c 以及 a = 3 stat存放状态，如果没有，返回none
	REG get_reg();  // 当寄存器满的时候，找到一个将要替换的reg，要有替换策略
	void cal_use_info();

	void push_reg(REG reg);  // 模拟堆栈 push指令 把reg里面的变量放到mem里 并且让reg空
	void pop_reg(REG reg);  // 模拟堆栈 pop指令 把栈顶的变量pop到reg里
	void set_esp_bias(int bias);  // 直接操作esp

	/* 当接受一个函数的三地址代码块时，重新初始化 */
	// todo 重新计算待用信息 重置堆栈和寄存器
	// todo 根据block_name找到对应的三地址代码
	void init_block(std::shared_ptr<TACBlock>);

private:
	REG get_free_reg();

	std::string                                 rvalue_[static_cast<int>(REG::None)];
	std::unordered_map<std::string, REG> 		avalue_reg_;
	std::unordered_map<std::string, int> 	avalue_mem_;  // 存与ebp的偏移
	std::unordered_map<std::string, UseInfo> 	use_info_;
	Scope &scope;

	/* 函数堆栈模拟 */
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
inline int SymbolManager::avalue_mem(const std::string& vairable) const {
	try {
		return avalue_mem_.at(vairable);
	} catch (std::out_of_range& e) {
		return int(-1);
	}
}


inline void SymbolManager::set_avalue_reg(const std::string& vairable, REG reg) {
	avalue_reg_[vairable] = reg;
}


inline void SymbolManager::set_avalue_mem(const std::string& variable, int mem) {
	avalue_mem_[variable] = mem;
}

#endif // INCLUDE_TCG_SYMBOLMANAGER_H_
