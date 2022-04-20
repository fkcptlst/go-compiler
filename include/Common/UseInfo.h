#ifndef INCLUDE_COMMON_USEINFO_H_
#define INCLUDE_COMMON_USEINFO_H_

#include "Common/Common.h"


struct UseInfo {
	int64_t next_use;		/* 待用信息 */
	bool active;			/* 活跃信息 */

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
	os << rhs.next_use << "," << ((rhs.active) ? "Y" : "^");
	return os;
}


inline bool UseInfo::no_use() const {
	return (next_use == 0 && active == false);
}

#endif