#include "TCG/SymbolManager.h"
#include "Common/Scope.h"

SymbolManager::SymbolManager(std::shared_ptr<Scope> global_scope, std::string name)
	: name_(name), global_scope_(global_scope), local_scope_(nullptr),
	  ret_now_(0), para_now_(0), stack_esp_(0), len_(0) {
	func_ = global_scope_->resolve(name);
	ret_num_ = func_->fun_ret_type_list->size();
	para_num_ = func_->fun_para_type_list->size();
}


// reset local scope
void SymbolManager::set_scope(std::shared_ptr<Scope> local_scope) {
	local_scope_ = local_scope;
}


// 编码变量 = scode名 + 变量名 (如果是数字则返回空串)
std::string SymbolManager::encode_var(std::string var) const {
	std::string en_var;
	if (!var.empty() && !isdigit(var[0])) {
		std::shared_ptr<Symbol> symbol = local_scope_->resolve(var);
		en_var = std::to_string(uint64_t(symbol->scope.get())) + ":" + var;
	}
	return en_var;
}


// 计算一个代码块中, 变量的待用信息和活跃信息
void SymbolManager::cal_use_info(std::shared_ptr<TACBlock> block) {
	LOG(INFO) << "cal_use_info";
	// 初始化符号表中所有变量的待用信息
	use_info_.clear();
	// 根据基本块出口来设置变量的活跃信息
	for (auto it = block->rbegin(); it != block->rend(); it++) {
		set_scope(it->scope);
		TACLine &line = *it;
		if (line.op == TACOP::FUN_RET) {
			set_use_info(line.src1.value, {0, true});
		} else {
			break;
		}
	}
	// 从后往前遍历基本块，将 变量的待用信息和活跃信息 绑定在 TACline 上
	for (auto it = block->rbegin(); it != block->rend(); it++) {
		set_scope(it->scope);
		TACLine &line = *it;
		if (line.op == TACOP::CALL) continue;
		if (line.dst.OperType == TACOPERANDTYPE::VAR) {
			// 把符号表中 dst 的待用信息和活跃信息 附加到 中间代码上
			line.dst.use_info = use_info(line.dst.value);
			// 重置符号表 dst 的待用信息和活跃信息
			set_use_info(line.dst.value, {0, false});
		}
		if (line.src1.OperType == TACOPERANDTYPE::VAR) {
			// 把符号表中 src1 的待用信息和活跃信息 附加到 中间代码上
			line.src1.use_info = use_info(line.src1.value);
			// 置位符号表 src1 的待用信息和活跃信息
			set_use_info(line.src1.value, {line.line, true});
		}
		if (line.src2.OperType == TACOPERANDTYPE::VAR) {
			// 把符号表中 src2 的待用信息和活跃信息 附加到 中间代码上
			line.src2.use_info = use_info(line.src2.value);
			// 置位符号表 src2 的待用信息和活跃信息
			set_use_info(line.src2.value, {line.line, true});
		}
	}
}


// 查看 $1 的 use_info
UseInfo SymbolManager::use_info(const std::string& vairable, bool encoded) const {
	std::string var_en = encoded ? vairable : encode_var(vairable);
	try {
		return use_info_.at(var_en);
	} catch (std::out_of_range& e) {
		return {0, false};
	}
}


// set $1 的 use_info 为 $2, 在翻译每个 TACline 时调用，用于替换选择
void SymbolManager::set_use_info(const std::string& vairable, UseInfo use_info) {
	std::string var_en = encode_var(vairable);
	if (!var_en.empty()) {
		use_info_[var_en] = use_info;
	}
}


// 获得一个空闲寄存器 (如果没有返回 REG::None)
REG SymbolManager::get_free_reg() {
	for (int i(0); i < static_cast<int>(REG::None); i++) {
		if (rvalue_[i] == "") {
			return static_cast<REG>(i);
		}
	}
	return REG::None;
}


// 解决a = b op c 以及 a = 3 stat存放状态，如果没有，返回 REG::None
REG SymbolManager::get_reg(std::string dst, std::string src1) {
	/* crTODO: 在 translator 中, 也存在修改 两个数组的情况!!! */
	/* crTODO: 目前默认认为如果变量移除寄存器, 可以保证该变量以后不会使用 或 该变量内存中有位置 */
	if (avalue_reg(dst) != REG::None) {
		return avalue_reg(dst);
	}

	if (avalue_mem_.end() != avalue_mem_.find(dst)) {
		return REG::EDI;
	}

	if (src1 != "" && avalue_reg(src1) != REG::None) {
		return avalue_reg(src1);
	}

	return get_free_reg();
}


// 当寄存器满的时候，找到一个将要替换的reg，要有替换策略
REG SymbolManager::get_reg() {
	// todo 先这么写着
	// 找到非活跃的寄存器
	return REG::ESI;
}


// 寻找一个将要替换的寄存器
RelacedEeg SymbolManager::get_replaced_reg() {
	// 寻找 不再使用的变量
	for (int i(0); i < static_cast<int>(REG::None); i++) {
		std::string& replaced_val = rvalue_[i];
		if (use_info_[replaced_val].no_use()) {
			return {static_cast<REG>(i), replaced_val};
		}
	}
	// 寻找 内存中有位置的变量
	for (int i(0); i < static_cast<int>(REG::None); i++) {
		std::string& replaced_val = rvalue_[i];
		int val_mem = avalue_mem(replaced_val);
		if (val_mem != -1) {
			return {static_cast<REG>(i), rvalue_[i], false, val_mem};
		}
	}
	// 返回需要 push 备份的寄存器
	int max_next_use = 0;
	REG max_next_use_reg;
	for (int i = 0; i < static_cast<int>(REG::None); i++) {
		std::string en_var = rvalue(static_cast<REG>(i));
		int next_use_i = use_info(en_var, true).next_use;
		if (max_next_use < next_use_i) {
			max_next_use = next_use_i;
			max_next_use_reg = static_cast<REG>(i);
		}
	}
	return {max_next_use_reg, rvalue(max_next_use_reg), false, -1};
}


void SymbolManager::set_zero_len() {
	len_ = 0;
}

int SymbolManager::get_stack_len() {
	return len_;
}

std::string SymbolManager::get_name() {
	return name_;
}


std::string SymbolManager::rvalue(REG reg) {
	return rvalue_[static_cast<int>(reg)];
}


REG SymbolManager::avalue_reg(const std::string& vairable) const {
	try {
		return avalue_reg_.at(vairable);
	} catch (std::out_of_range& e) {
		return REG::None;
	}
}


/* 返回 变量 的内存地址, 如果没有返回 uint64(-1) */
int SymbolManager::avalue_mem(const std::string& vairable) const {
	try {
		return avalue_mem_.at(vairable);
	} catch (std::out_of_range& e) {
		return int(-1);
	}
}


int SymbolManager::para() {
	int bias = -4 * (para_num_ - para_now_ + 1); // 栈底之前还有一个ret地址
	para_now_++;
	return bias;
}

int SymbolManager::ret() {
	int bias = -4 * (ret_num_ + para_num_ - ret_now_ + 1);
	ret_now_++;
	return bias;
}

std::shared_ptr<Symbol> SymbolManager::resolve_fun(std::string name) {
	std::shared_ptr<Symbol> p = local_scope_->resolve(name);
	if (p->isFun()) {
		return p;
	} else {
		return nullptr;
	}
}


// 让一个变量存在 reg 里面
void SymbolManager::set_avalue_reg(const std::string& vairable, REG reg) {
	std::string old = rvalue_[static_cast<int>(reg)];
	if (avalue_reg_.end() != avalue_reg_.find(old) && avalue_reg_[old] == reg) {
		avalue_reg_.erase(old);
	}
	avalue_reg_[vairable] = reg;
	rvalue_[static_cast<int>(reg)] = vairable;
}


// 让一个变量存在堆栈里，用跟 ebp 的偏移
void SymbolManager::set_avalue_mem(const std::string& variable, int mem) {
	std::string old = svalue_[mem / 4];
	if (avalue_mem_.end() != avalue_mem_.find(old) && avalue_mem_[old] == mem) {
		avalue_mem_.erase(old);
	}
	avalue_mem_[variable] = mem;
	svalue_[mem / 4] = variable;
}


// 打印 当前通用寄存器中所存的变量
void SymbolManager::show_reg(REG reg) {
	if (reg != REG::None) {
		LOG(INFO) << to_string(reg) << ": " << rvalue_[static_cast<int>(reg)];
	} else {
		for (int reg_i = 0; reg_i < static_cast<int>(REG::None); reg_i++) {
			std::string en_var = rvalue_[static_cast<int>(reg_i)];
			int var_next_use;
			try {
				var_next_use = use_info_.at(en_var).next_use;
			} catch (std::out_of_range& e) {
				var_next_use = 0;
			}
			LOG(INFO) << to_string(static_cast<REG>(reg_i)) << ": " << en_var << " " << var_next_use;
		}
	}
}


// 打印 当前栈中所存的变量
void SymbolManager::show_mem(int mem) {
	LOG(INFO) << "len of mem: " << len_;
	for (auto it : avalue_mem_) {
		LOG(INFO) << "mem " << it.second << ": " << it.first;
	}
}



POSTYPE SymbolManager::position(std::string variable) {
	int pos = variable.find(":");
	std::string scope_str = variable.substr(0, pos);
	std::string ture_name = variable.substr(pos + 1, variable.size());
	std::stringstream ss;
	ss << scope_str;
	uint64_t scope_p_t;
	ss >> scope_p_t;
	Scope* scope_p = (Scope*)(scope_p_t);
	if (scope_p == global_scope_.get()) {
		return POSTYPE::GLOBAL;
	} else if (avalue_reg_.end() != avalue_reg_.find(variable)) {
		return POSTYPE::REG;
	} else if (avalue_mem_.end() != avalue_mem_.find(variable)) {
		return POSTYPE::MEM;
	} else {
		return POSTYPE::NONE;
	}
}


// 模拟堆栈 push指令 把reg里面的变量放到mem里 并且让reg空
void SymbolManager::push_reg(REG reg, int overwrite) {
	if (overwrite == 1) {
		std::string var = rvalue_[static_cast<int>(reg)];
		svalue_.push_back(var);
		avalue_mem_[var] = stack_esp_;
	} else {
		svalue_.push_back("Null");
	}
	stack_esp_ += 4;
	len_++;
}


// 模拟堆栈 pop指令 把栈顶的变量pop到reg里
void SymbolManager::pop_reg(REG reg) {
	std::string var = svalue_.back();
	svalue_.pop_back();
	std::string old = rvalue_[static_cast<int>(reg)];
	if (avalue_reg_.end() != avalue_reg_.find(old) && avalue_reg_[old] == reg) {
		avalue_reg_.erase(old);
	}
	if (var != "Null") {
		avalue_reg_[var] = reg;
		rvalue_[static_cast<int>(reg)] = var;
	}
	stack_esp_ -= 4;  //todo 还要考虑栈空的情况
	len_--;
}


// 直接操作 栈顶 esp
void SymbolManager::set_esp_bias(int bias) {
	if (bias > 0) {
		int num = bias / 4;
		for (int i = 0; i < num; i++) {
			svalue_.push_back("Null");
		}
		len_ += num;
	} else {
		int num = (-bias) / 4;
		for (int i = 0; i < num; i++) {
			std::string old = svalue_.back();
			if (avalue_mem_.end() != avalue_mem_.find(old) && avalue_mem_[old] == stack_esp_ - (i + 1) * 4) {
				avalue_mem_.erase(old);
			}
			svalue_.pop_back();
		}
		len_ -= num;
	}
	stack_esp_ += bias;
}

int SymbolManager::get_esp() {
	return stack_esp_;
}
