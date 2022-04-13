add_library(src
	SentenceTranslator/BaseTranslator.cpp
	BlockTranslator.cpp
	SymbolManager.cpp
	SymbolTabel.cpp
)
target_include_directories(src PUBLIC ${CMAKE_SOURCE_DIR}/include)