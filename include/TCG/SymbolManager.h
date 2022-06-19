#ifndef INCLUDE_TCG_SYMBOLMANAGER_H_
#define INCLUDE_TCG_SYMBOLMANAGER_H_

#include "Common/Common.h"
#include "Common/Scope.h"
#include "Common/TAC.h"
#include "TCG/ASM.h"

enum class POSTYPE {
	GLOBAL, REG, MEM, NONE
};


class SymbolManager {
public:
	SymbolManager(std::shared_ptr<Scope> global_scope, std::string name);

	/* 当接受一个函数的三地址代码块时，重新初始化 */
	// todo 重新计算待用信息 重置堆栈和寄存器
	// todo 根据block_name找到对应的三地址代码
	void set_scope(std::shared_ptr<Scope> local_scope);

	std::string encode_var(std::string var);

	std::string rvalue(REG reg);
	REG 		avalue_reg(const std::string& vairable) const;
	int 		avalue_mem(const std::string& vairable) const;
	UseInfo		use_info(const std::string& vairable) const;
	void		set_avalue_reg(const std::string& vairable, REG reg);
	void		set_avalue_mem(const std::string& variable, int mem);
	void		set_use_info(const std::string& vairable, UseInfo use_info);

	REG 		get_free_reg();
	REG			get_reg();
	REG 		get_reg(std::string dst, std::string src1 = "");
	void 		cal_use_info();

	void 		push_reg(REG reg, int overwrite = 1);
	void 		pop_reg(REG reg);
	int 		get_esp();
	void 		set_esp_bias(int bias);
	POSTYPE 	position(std::string variable);
	int 		para();
	int 		ret();
	std::shared_ptr<Symbol> resolve_fun(std::string name);
	std::string get_name();
	void 		set_zero_len();
	int			get_stack_len();



private:
	// 符号表
	std::string									name_;
	std::shared_ptr<Scope> 						global_scope_;
	std::shared_ptr<Scope> 						local_scope_;
	std::shared_ptr<Symbol>						func_;

	// 变量存储信息
	std::string                                 rvalue_[static_cast<int>(REG::None)];
	std::vector<std::string>					svalue_;
	std::unordered_map<std::string, REG> 		avalue_reg_;
	std::unordered_map<std::string, int> 		avalue_mem_;  // 存与ebp的偏移
	std::unordered_map<std::string, UseInfo> 	use_info_;

	/* 函数堆栈模拟 */
	int len_;		// 当前栈的长度 (4 字节)
	int stack_esp_;	// 模拟堆栈的栈顶 (与当前函数栈底的相对值)
	int para_num_;
	int para_now_;
	int ret_num_;
	int ret_now_;
};

#endif // INCLUDE_TCG_SYMBOLMANAGER_H_
