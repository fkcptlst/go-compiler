#ifndef INCLUDE_TCG_SYMBOLMANAGER_H_
#define INCLUDE_TCG_SYMBOLMANAGER_H_

#include "Common.h"
#include "TAC.h"
#include "TCG/ASM.h"


/* crTODO */
#define reg_nums 10


class SymbolManager {
public:
	/* 返回寄存器 reg 的变量名, 无意义则返回 空串 */
	std::string rvalue(REG reg);

	/* 返回该变量的地址 内存地址, 寄存器地址(0 ~ regnums) */
	uint64_t avalue(const std::string& vairable);

	/* 给变量 vairable 分配一个寄存器 */
	REG get_reg(const std::string& vairable);

	/* 计算一个代码块中, 变量的待用信息和活跃信息 */
	void cal_use_info(TACBlock block);

private:
	/* 寄存器描述数组 */
	std::string rvalue_[reg_nums];

	/* 变量地址描述数组 */
	std::unordered_map<std::string, uint64_t> avalue_;
};


#endif // INCLUDE_TCG_SYMBOLMANAGER_H_
