#ifndef INCLUDE_TCG_SYMBOLMANAGER_H_
#define INCLUDE_TCG_SYMBOLMANAGER_H_

#include "Common/Common.h"
#include "Common/Scope.h"
#include "Common/TAC.h"
#include "TCG/ASM.h"


class SymbolManager {
public:
	SymbolManager(std::shared_ptr<Scope> Global_Scope, std::string name);

	/* get and set */
	std::string rvalue(REG reg);
	REG 		avalue_reg(const std::string& vairable) const;
	int 		avalue_mem(const std::string& vairable) const;
	UseInfo		use_info(const std::string& vairable) const;
	void		set_avalue_reg(const std::string& vairable, REG reg);  // 让一个变量存在reg里面
	void		set_avalue_mem(const std::string& variable, int mem);  // 让一个变量存在堆栈里，用跟ebp的偏移
	void		set_use_info(const std::string& vairable, UseInfo use_info);

	REG get_reg(std::string dst, std::string src1, std::string src2);  // 解决a = b op c 以及 a = 3 stat存放状态，如果没有，返回none
	REG get_reg();  // 当寄存器满的时候，找到一个将要替换的reg，要有替换策略
	void cal_use_info();
	std::string encode_var(std::string);

	void push_reg(REG reg);  // 模拟堆栈 push指令 把reg里面的变量放到mem里 并且让reg空
	void pop_reg(REG reg);  // 模拟堆栈 pop指令 把栈顶的变量pop到reg里
	void set_esp_bias(int bias);  // 直接操作esp
	int get_esp();

	/* 当接受一个函数的三地址代码块时，重新初始化 */
	// todo 重新计算待用信息 重置堆栈和寄存器
	// todo 根据block_name找到对应的三地址代码
	void set_scope(std::shared_ptr<Scope> local_scope);

private:
	REG get_free_reg();

	std::string                                 rvalue_[static_cast<int>(REG::None)];
	std::vector<std::string>					svalue_;
	std::unordered_map<std::string, REG> 		avalue_reg_;
	std::unordered_map<std::string, int> 		avalue_mem_;  // 存与ebp的偏移
	std::unordered_map<std::string, UseInfo> 	use_info_;
	// Scope &scope;
	// TACBlock block;
	std::shared_ptr<Scope> 						Local_Scope;
	std::shared_ptr<Scope> 						Global_Scope;

	/* 函数堆栈模拟 */
	int stack_esp;
	int para_num;
	int ret_num;
	std::string name;
};

inline void SymbolManager::set_scope(std::shared_ptr<Scope> local_scope) {
	Local_Scope = local_scope;
}

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

inline void SymbolManager::push_reg(REG reg) {
	std::string var = rvalue_[static_cast<int>(reg)];
	svalue_.push_back(var);
	set_avalue_mem(var, stack_esp);
	stack_esp++;
}

inline void SymbolManager::pop_reg(REG reg) {
	std::string var = svalue_.back();
	svalue_.pop_back();
	set_avalue_reg(var, reg);
	rvalue_[static_cast<int>(reg)] = var;
	stack_esp--;  //todo 还要考虑栈空的情况
}

inline void SymbolManager::set_esp_bias(int bias) {
	stack_esp += bias;
	int num = bias / 4;
	for (int i = 0; i < num; i++) {
		svalue_.push_back("null");
	}
}

inline int SymbolManager::get_esp() {
	return stack_esp;
}


#endif // INCLUDE_TCG_SYMBOLMANAGER_H_
