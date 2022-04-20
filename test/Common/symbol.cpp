#include "Common/Common.h"
#include "Common/Scope.h"

void copy_assign() {
	Symbol symbol1("A", nullptr, Symbol::SymbolType::VAR, Symbol::Type::INT);
	Symbol symbol2("B", nullptr, Symbol::SymbolType::VAR, Symbol::Type::INT);
	Symbol symbol3 = symbol1;
	std::cout << "symbol1.name: " << symbol1.name << std::endl;
	std::cout << "symbol2.name: " << symbol2.name << std::endl;
	std::cout << "symbol3.name: " << symbol3.name << std::endl;
	symbol1.name = "Q";
	std::cout << "symbol1.name: " << symbol1.name << std::endl;
	std::cout << "symbol2.name: " << symbol2.name << std::endl;
	std::cout << "symbol3.name: " << symbol3.name << std::endl;
}


int main() {
	copy_assign();
	return 0;
}