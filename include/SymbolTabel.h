#ifndef INCLUDE_SYMBOLTABEL_H_
#define INCLUDE_SYMBOLTABEL_H_

#include "Common/Common.h"


enum class VairableKind {
	/* 布尔 */
	BOOL,
	/* 数字 */
	INT8, INT16, INT32, INT64, INT,
	UINT8, UINT16, UINT32, UINT64, UINT,
	FLOAT32, FLOAT64,
	COMPLEX64, COMPLEX128,
	BYTE,	/* 等价于 uint8 */
	RUNE,	/* 等价于 int32 */
	/* 字符串 */
	STRING,
	/* crTODO: 其他牛逼类型 */
};


struct UseInfo {
	int64_t next_use;		/* 待用信息 */
	bool active;			/* 活跃信息 */


	UseInfo(int64_t next_use=0, bool active=false)
	: next_use(next_use), active(active) {}

	UseInfo& operator=(const UseInfo& rhs) {
		next_use = rhs.next_use;
		active = rhs.active;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const UseInfo& rhs) {
		os << rhs.next_use << "," << ((rhs.active) ? "Y" : "^");
		return os;
	}

	bool no_use() const {
		return (next_use == 0 && active == false);
	}
};


struct VairableInfo {
	VairableKind kind;
	UseInfo use_info;

	VairableInfo()
	: kind(VairableKind::BOOL), use_info() {}

	VairableInfo(VairableKind kind)
	: kind(kind), use_info() {}
};


using SymbolTabel = std::unordered_map<std::string, VairableInfo>;

/* crTODO: 目前想的一个代码块一个表, 还可以改 */
using SymbolTabels = std::vector<SymbolTabel>;

#endif // INCLUDE_SYMBOLTABEL_H_
