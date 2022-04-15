add_library(src
	TCG/SentenceTranslator/BaseTranslator.cpp
	TCG/BlockTranslator.cpp
	TCG/SymbolManager.cpp
	SymbolTabel.cpp
)
target_include_directories(src PUBLIC ${CMAKE_SOURCE_DIR}/include)