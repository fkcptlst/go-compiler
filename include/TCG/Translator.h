#ifndef INCLUDE_TCG_TRANSLATOR_H_
#define INCLUDE_TCG_TRANSLATOR_H_

#include "Common/Common.h"
#include "Common/Scope.h"
#include "Common/TAC.h"
#include "TCG/ASM.h"
#include "TCG/BlockTranslator.h"

class Translator {
private:
    // SymbolManager SymbolManager_;        /* crTODO: 将 SymbolManager_ 改为 一个快一个 ? */
    BlockTranslator BlockTranslator_;
    TACFile TACFile_;
    ASMFile ASMFile_;

    // 翻译数据段
    void dataTranslate();

    // 翻译代码段
    void textTranslate();
public:
    // 构造函数，接受输入
    Translator(Scope&, TACFile&);
    
    // 完成三地址到汇编的翻译
    void Translate();

    // 结果格式化输出到文件
    void OutputFile(std::string);
};


#endif // INCLUDE_TCG_TRANSLATOR_H_
