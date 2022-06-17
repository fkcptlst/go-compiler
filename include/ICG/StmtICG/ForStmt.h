#ifndef INCLUDE_ICG_STMTICG_FORSTMT_H_
#define INCLUDE_ICG_STMTICG_FORSTMT_H_
#include "Common/Common.h"
#include "Common/TAC.h"

struct ForStmt
{
    std::string CurIndex; //index to direct to which FOR
    TACLine LoopCon;      //condition to continue the loop
    TACLine UpdateCon;    //update line after loop done
    std::vector<std::string> newParas;
    
    ForStmt(std::string CurIndex_)
    {
        CurIndex = CurIndex_; 
    }


    ForStmt()
    {
    }

};



#endif //INCLUDE_ICG_STMTICG_FORSTMT_H_
