#ifndef INCLUDE_COMMON_USEINFO_H_
#define INCLUDE_COMMON_USEINFO_H_

#include "Common/Common.h"


struct UseInfo {
	int64_t 		next_use;		// 待用信息 (0 表示不再使用, 其他数表示下一次使用的位置)
	bool 			active;			// 活跃信息 (true 表示还要使用, false 表示不需要了)

	UseInfo(int64_t next_use=0, bool active=false) : next_use(next_use), active(active) {}

	UseInfo& operator=(const UseInfo& rhs);
	friend std::ostream& operator<<(std::ostream& os, const UseInfo& rhs);

	bool no_use() const;
};


inline UseInfo& UseInfo::operator=(const UseInfo& rhs) {
	next_use = rhs.next_use;
	active = rhs.active;
	return *this;
}


inline std::ostream& operator<<(std::ostream& os, const UseInfo& rhs) {
	os << "(" << rhs.next_use << "," << ((rhs.active) ? "Y" : "^") << ")";
	return os;
}


inline bool UseInfo::no_use() const {
	return (next_use == 0 && active == false);
}

#endif
