#ifndef INCLUDE_SYMBOLTABEL_H_
#define INCLUDE_SYMBOLTABEL_H_

#include "Common.h"

enum VairableKind {
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


struct VairableInfo {
	VairableKind kind;

	/* 待用信息和活跃信息 */
	int next_use;
	bool active;

	VairableInfo() : kind(INT), next_use(0), active(false) {}
};


struct SymbolTabel {
	std::unordered_map<std::string, VairableInfo> symbol_tabel;
};


/* crTODO: 目前想的一个代码块一个表, 还可以改 */
using SymbolTabels = std::vector<SymbolTabel>;

#endif // INCLUDE_SYMBOLTABEL_H_
