from loguru import logger
from antlr4.tree.Tree import ParseTree
from typing import override
from functools import partial
from ..grammar.generated.GoParserListener import GoParserListener
from ..grammar.generated.GoParser import GoParser
from ..common.common import DELIMITER, SUCCESS
from ..common.tac import TACFile, TACOP, TACOPERANDTYPE, Operand, TACLine, TACBlock
from ..common.Scope import Scope, Symbol
from .utils import ctx_decoder, ctx_encoder
from .statement_icg.ForStatement import ForStatement


class MyGoListener(GoParserListener):
    def __init__(self):
        super().__init__()
        self.line_index = 0
        self.local_index = 0
        self.if_index = 0
        self.for_index = 0

        # 全局的key是global，其他的为函数名字
        self.TACBlocks: TACFile = TACFile()

        self.values: dict[ParseTree, str] = {}
        self.if_values: dict[ParseTree, str] = {}
        self.for_values: dict[ParseTree, ForStatement] = {}

        # 记录哪个临时变量是指针
        self.ptrs: dict[str, bool] = {}

        self.currentScope: Scope | None = None
        self.globalScope: Scope | None = None
        self.allScopes: list[Scope] = []

        self.cur_fun: str = "global"

    @property
    def tmp_symbol_type(self) -> Symbol.Type:
        return Symbol.Type.INT

    def create_local_var(self) -> str:
        local: str = ""
        check_result: bool = False
        first_time: bool = True
        while check_result or first_time:
            first_time = False  # do while loop

            local = "T" + str(self.local_index)
            self.local_index += 1
            tmp: Symbol | None = self.currentScope.resolve(local)
            if tmp is not None:
                check_result = True
            else:
                check_result = False

        symbol: Symbol = Symbol(
            local, self.currentScope, Symbol.SymbolType.VAR, type_=self.tmp_symbol_type
        )
        self.currentScope.para_define(symbol)
        return local

    def create_else_label(self) -> str:
        local: str = str(self.if_index)
        self.if_index += 1
        return local

    def create_for_label(self) -> str:
        local: str = str(self.for_index)
        self.for_index += 1
        return local

    def operand_type_resolve(self, name: str) -> TACOPERANDTYPE:
        if self.ptrs.get(name, False):
            return TACOPERANDTYPE.PTR
        if name[0].isdigit() or name[0] == "-":
            return TACOPERANDTYPE.IMM
        return TACOPERANDTYPE.VAR

    @staticmethod
    def is_digit(s: str) -> bool:
        """check if s is integer"""
        try:
            int(s)
            return True
        except ValueError:
            return False

    def go_to_3file(self, filename: str):
        with open(filename, "w") as f:
            for key, block in self.TACBlocks.items():
                for it in block:
                    f.write(
                        f"{it.line:<4}{it.op.name:<15}{it.src1.OperType.name:>7}:{it.src1.value:<10}{it.src2.OperType.name:>7}:{it.src2.value:<10}{it.dst.OperType.name:>7}:{it.dst.value:<10}\n"
                    )
                f.write("\n")

    def push_line(self, op: TACOP, src1: Operand, src2: Operand, dst: Operand):
        self.TACBlocks[self.cur_fun].append(
            TACLine(self.line_index, op, src1, src2, dst, self.currentScope)
        )
        self.line_index += 1

    def add_scope(self):
        scope: Scope = Scope(self.currentScope)
        self.allScopes.append(scope)
        self.currentScope = scope

    def my_print(self, scope: Scope):
        for symbol in scope.para_symbols:
            logger.debug(f"para_symbol: {symbol}")
        for symbol in scope.fun_symbols:
            logger.debug(f"fun_symbol: {symbol}")

    def pop_scope(self):
        self.my_print(self.currentScope)
        self.currentScope = self.currentScope.enclosing_scope

    @override
    def exitInteger(self, ctx: GoParser.IntegerContext):
        self.values[ctx] = ctx.DECIMAL_LIT().getText()

    @override
    def exitBasicLit(self, ctx: GoParser.BasicLitContext):
        if ctx.integer():
            basic_lit_value: str = self.values[ctx.integer()]
            self.values[ctx] = basic_lit_value

    @override
    def exitOperand(self, ctx: GoParser.OperandContext):
        if ctx.literal():
            operand_value: str = self.values[ctx.literal()]
            self.values[ctx] = operand_value
        if ctx.expression():
            operand_value: str = self.values[ctx.expression()]
            self.values[ctx] = operand_value
        if ctx.operandName():
            operand_value: str = self.values[ctx.operandName()]
            self.values[ctx] = operand_value

    @override
    def exitLiteral(self, ctx: GoParser.LiteralContext):
        if ctx.basicLit():
            literal_value: str = self.values[ctx.basicLit()]
            self.values[ctx] = literal_value + DELIMITER

    @override
    def exitPrimaryExpr(self, ctx: GoParser.PrimaryExprContext):
        # 是函数调用
        if ctx.arguments():
            primary_expr_value: list[str] = []

            # 看是否定义
            fun_identity: list[str] = ctx_decoder(self.values[ctx.primaryExpr()])
            identity: str = fun_identity[0]
            fun_symbol: Symbol | None = self.currentScope.resolve(identity)
            if fun_symbol is None:
                logger.error(f"Undefined function: {identity}")
                logger.debug(f"Undefined function: {identity}")
                exit(-1)
            # 找到了函数
            arguments_values: list[str] = ctx_decoder(self.values[ctx.arguments()])
            # 判断参数数量合理
            if len(fun_symbol.fun_para_type_list) != len(arguments_values):
                logger.error(f"Undefined function: {identity}")
                logger.debug(f"Wrong parameter number for : {identity}")
                exit(-1)

            # 函数调用
            blank: str = ""
            for _ in fun_symbol.fun_ret_type_list:
                # TODO: CreateLocalVar应当传入不同变量的type
                tmp: str = self.create_local_var()
                self.push_line(
                    TACOP.RET,
                    Operand(tmp, self.operand_type_resolve(tmp)),
                    Operand(blank, TACOPERANDTYPE.NULL_),
                    Operand(blank, TACOPERANDTYPE.NULL_),
                )
                primary_expr_value.append(tmp)
            for para in arguments_values:
                self.push_line(
                    TACOP.PARA,
                    Operand(para, self.operand_type_resolve(para)),
                    Operand(blank, TACOPERANDTYPE.NULL_),
                    Operand(blank, TACOPERANDTYPE.NULL_),
                )
            self.push_line(
                TACOP.CALL,
                Operand(identity, self.operand_type_resolve(identity)),
                Operand(blank, TACOPERANDTYPE.NULL_),
                Operand(blank, TACOPERANDTYPE.NULL_),
            )
            self.values[ctx] = ctx_encoder(primary_expr_value)

        # 数
        elif ctx.operand():
            primary_expr_value: str = self.values[ctx.operand()]
            self.values[ctx] = primary_expr_value

        # 数组
        elif ctx.index():
            array_name: list[str] = ctx_decoder(self.values[ctx.primaryExpr()])
            identity: str = array_name[0]
            logger.debug(f"identity: {identity}")

            # 名字不是数组而是变量
            array_symbol: Symbol | None = self.currentScope.resolve(identity)
            if not array_symbol.is_array:
                logger.error(f"Only array can be indexed: {identity}")
                logger.debug(f"Only array can be indexed: {identity}")
                exit(-1)

            array_index: list[str] = ctx_decoder(self.values[ctx.index().expression()])
            index_s: str = array_index[0]
            if 1 != len(array_index):
                logger.error("wrong number of array index input")
                logger.debug("wrong number of array index input")
                exit(-1)

            # 若编译时可判断数组越界
            if self.is_digit(index_s):
                idx: int = int(index_s)
                if idx > array_symbol.array_length - 1 or idx < 0:
                    logger.error(f"Array index out of bound: {identity}")
                    logger.debug(f"Array index out of bound: {identity}")
                    logger.debug(str(idx), array_symbol.array_length)
                    exit(-1)

            tmp_ptr_offset: str = self.create_local_var()
            int_size: str = "4"
            self.push_line(
                TACOP.MUL,
                Operand(int_size, self.operand_type_resolve(int_size)),
                Operand(index_s, self.operand_type_resolve(index_s)),
                Operand(tmp_ptr_offset, self.operand_type_resolve(tmp_ptr_offset)),
            )
            tmp_ptr: str = self.create_local_var()
            self.push_line(
                TACOP.ADD,
                Operand(identity, self.operand_type_resolve(identity)),
                Operand(tmp_ptr_offset, self.operand_type_resolve(tmp_ptr_offset)),
                Operand(tmp_ptr, self.operand_type_resolve(tmp_ptr)),
            )
            # todo map
            self.ptrs[tmp_ptr] = True

            # NOTE: You have to encode it before assignment -> suffix must be #
            self.values[ctx] = ctx_encoder([tmp_ptr])

    @override
    def exitPrimaryExpression(self, ctx: GoParser.PrimaryExpressionContext):
        expression_value: str = self.values[ctx.primaryExpr()]
        self.values[ctx] = expression_value

    @override
    def exitPlusMinusOperation(self, ctx: GoParser.PlusMinusOperationContext):
        left: list[str] = ctx_decoder(self.values[ctx.expression(0)])
        right: list[str] = ctx_decoder(self.values[ctx.expression(1)])
        if len(left) != 1 or len(right) != 1:
            logger.error("wrong literal number1")
            logger.debug("wrong literal number1")
            exit(-1)
        dst: str = self.create_local_var()
        plus_minus_operation_values: list[str] = [dst]

        if ctx.PLUS():
            self.push_line(
                TACOP.ADD,
                Operand(left[0], self.operand_type_resolve(left[0])),
                Operand(right[0], self.operand_type_resolve(right[0])),
                Operand(dst, self.operand_type_resolve(dst)),
            )
        elif ctx.MINUS():
            self.push_line(
                TACOP.SUB,
                Operand(left[0], self.operand_type_resolve(left[0])),
                Operand(right[0], self.operand_type_resolve(right[0])),
                Operand(dst, self.operand_type_resolve(dst)),
            )
        self.values[ctx] = ctx_encoder(plus_minus_operation_values)

    @override
    def exitRelationOperation(self, ctx: GoParser.RelationOperationContext):
        left: list[str] = ctx_decoder(self.values[ctx.expression(0)])
        right: list[str] = ctx_decoder(self.values[ctx.expression(1)])
        if len(left) != 1 or len(right) != 1:
            logger.error("wrong literal number1")
            logger.debug("wrong literal number1")
            exit(-1)

        # expression 在for 下的情况，需要添加ifexp 并且反过来 将condition_loop信息写入forstatement里面
        if (
                ctx.parentCtx.parentCtx.children[0].getText() == "for"
                and ctx.parentCtx.parentCtx.children[1] == ctx.parentCtx
        ):
            tmp: ForStatement = self.for_values[ctx.parentCtx.parentCtx]
            dst: str = "ENDFOR" + tmp.cur_index
            dst_: str = "FORLOOP" + tmp.cur_index
            tmpline: TACLine | None = None

            tac_line_partial = partial(
                TACLine,
                line=self.line_index,
                src1=Operand(left[0], self.operand_type_resolve(left[0])),
                src2=Operand(right[0], self.operand_type_resolve(right[0])),
                dst=Operand(dst_, TACOPERANDTYPE.LABEL),
                scope=self.currentScope,
            )
            push_line_partial = partial(
                self.push_line,
                src1=Operand(left[0], self.operand_type_resolve(left[0])),
                src2=Operand(right[0], self.operand_type_resolve(right[0])),
                dst=Operand(dst, TACOPERANDTYPE.LABEL),
            )

            if ctx.EQUALS():
                tmpline = tac_line_partial(op=TACOP.IFEQ)
                push_line_partial(op=TACOP.IFNEQ)
            elif ctx.NOT_EQUALS():
                tmpline = tac_line_partial(op=TACOP.IFNEQ)
                push_line_partial(op=TACOP.IFEQ)
            elif ctx.GREATER():
                tmpline = tac_line_partial(op=TACOP.IFGT)
                push_line_partial(op=TACOP.IFLE)
            elif ctx.GREATER_OR_EQUALS():
                tmpline = tac_line_partial(op=TACOP.IFGE)
                push_line_partial(op=TACOP.IFLT)
            elif ctx.LESS():
                tmpline = tac_line_partial(op=TACOP.IFLT)
                push_line_partial(op=TACOP.IFGE)
            elif ctx.LESS_OR_EQUALS():
                tmpline = tac_line_partial(op=TACOP.IFLE)
                push_line_partial(op=TACOP.IFGT)
            tmp.loop_con = tmpline
            self.for_values[ctx.parentCtx.parentCtx] = tmp

        # expression 在if 下的情况，需要添加ifexp 并且反过来
        if ctx.parentCtx.getText().find("if") == 0 and ctx == ctx.parentCtx.children[1]:
            if ctx.parentCtx.getText().find("else") != -1:  # 有else，就goto else
                dst = "ELSE" + self.if_values[ctx.parentCtx]
            else:
                dst = "ENDIF" + self.if_values[ctx.parentCtx]

            push_line_partial = partial(
                self.push_line,
                src1=Operand(left[0], self.operand_type_resolve(left[0])),
                src2=Operand(right[0], self.operand_type_resolve(right[0])),
                dst=Operand(dst, TACOPERANDTYPE.LABEL),
            )

            if ctx.EQUALS():
                push_line_partial(op=TACOP.IFNEQ)
            elif ctx.NOT_EQUALS():
                push_line_partial(op=TACOP.IFEQ)
            elif ctx.GREATER():
                push_line_partial(op=TACOP.IFLE)
            elif ctx.GREATER_OR_EQUALS():
                push_line_partial(op=TACOP.IFLT)
            elif ctx.LESS():
                push_line_partial(op=TACOP.IFGE)
            elif ctx.LESS_OR_EQUALS():
                push_line_partial(op=TACOP.IFGT)

    @override
    def exitMulDivOperation(self, ctx: GoParser.MulDivOperationContext):
        left: list[str] = ctx_decoder(self.values[ctx.expression(0)])
        right: list[str] = ctx_decoder(self.values[ctx.expression(1)])

        if len(left) != 1 or len(right) != 1:
            logger.error("wrong literal number2")
            logger.debug("wrong literal number2")
            exit(-1)

        dst: str = self.create_local_var()
        mul_div_operation_values: list[str] = [dst]

        if ctx.STAR():
            self.push_line(
                TACOP.MUL,
                Operand(left[0], self.operand_type_resolve(left[0])),
                Operand(right[0], self.operand_type_resolve(right[0])),
                Operand(dst, self.operand_type_resolve(dst)),
            )
        elif ctx.DIV():
            self.push_line(
                TACOP.DIV,
                Operand(left[0], self.operand_type_resolve(left[0])),
                Operand(right[0], self.operand_type_resolve(right[0])),
                Operand(dst, self.operand_type_resolve(dst)),
            )

        self.values[ctx] = ctx_encoder(mul_div_operation_values)

    @override
    def enterSourceFile(self, ctx: GoParser.SourceFileContext):
        self.globalScope = Scope()
        self.currentScope = self.globalScope
        current_block: TACBlock = []
        self.TACBlocks[self.cur_fun] = current_block

    @override
    def exitSourceFile(self, ctx: GoParser.SourceFileContext):
        logger.debug("exit source file")

    @override
    def exitExpressionList(self, ctx: GoParser.ExpressionListContext):
        expression_values: list[str] = []
        for i in range(len(ctx.expression())):
            s: str = self.values[ctx.expression(i)]
            vs: list[str] = ctx_decoder(s)
            # logger.debug(f"{s} -> {vs}")
            for each in vs:
                expression_values.append(each)
        self.values[ctx] = ctx_encoder(expression_values)

    @override
    def enterFunctionDecl(self, ctx: GoParser.FunctionDeclContext):
        identifier: str = ctx.IDENTIFIER().getText()
        # 判断是否函数名字重复
        if self.currentScope.cur_resolve(identifier) == SUCCESS:
            logger.error(f"Redeclaration of function: {identifier}")
            logger.debug(f"Redeclaration of function: {identifier}")
            exit(-1)

        # 更改目前的function
        self.cur_fun = identifier
        current_block: TACBlock = []
        self.TACBlocks[self.cur_fun] = current_block

        # 构建fun symbol
        fun_ret_type_list: list[Symbol.Type] = []
        fun_para_list: list[Symbol.Type] = []

        # 函数ret参数
        if (
                not ctx.signature().result()
                or len(ctx.signature().result().parameters().parameterDecl()) == 0
        ):  # 无返回值
            # do nothing
            pass
        else:  # 有返回值
            n: int = len(ctx.signature().result().parameters().parameterDecl())
            for i in range(n):
                each_type: str = (
                    ctx.signature()
                    .result()
                    .parameters()
                    .parameterDecl(i)
                    .type_()
                    .typeName()
                    .getText()
                )
                each_s_type: Symbol.Type = Symbol.to_type(each_type)
                fun_ret_type_list.append(each_s_type)

        # 函数para参数
        # 无参数
        if len(ctx.signature().parameters().parameterDecl()) == 0:
            # do nothing
            pass
        else:
            # 有参数
            n: int = len(ctx.signature().parameters().parameterDecl())
            for i in range(n):
                each_type: str = (
                    ctx.signature()
                    .parameters()
                    .parameterDecl(i)
                    .type_()
                    .typeName()
                    .getText()
                )
                each_s_type: Symbol.Type = Symbol.to_type(each_type)
                for j in range(
                        len(
                            ctx.signature()
                                    .parameters()
                                    .parameterDecl(i)
                                    .identifierList()
                                    .IDENTIFIER()
                        )
                ):
                    fun_para_list.append(each_s_type)

        symbol: Symbol = Symbol(
            identifier,
            self.currentScope,
            Symbol.SymbolType.FUN,
            fun_ret_type_list=fun_ret_type_list,
            fun_para_type_list=fun_para_list,
        )
        scope: Scope = Scope(self.currentScope)
        self.currentScope.fun_define(symbol)
        self.currentScope = scope

        # 打印 FUN_PARA
        for i in range(len(ctx.signature().parameters().parameterDecl())):
            para_number: int = len(
                ctx.signature()
                .parameters()
                .parameterDecl(i)
                .identifierList()
                .IDENTIFIER()
            )
            for j in range(para_number):
                fun_para: str = (
                    ctx.signature()
                    .parameters()
                    .parameterDecl(i)
                    .identifierList()
                    .IDENTIFIER(j)
                    .getText()
                )
                self.push_line(
                    TACOP.FUN_PARA,
                    Operand(fun_para, self.operand_type_resolve(fun_para)),
                    Operand("", TACOPERANDTYPE.NULL_),
                    Operand("", TACOPERANDTYPE.NULL_),
                )

    @override
    def exitFunctionDecl(self, ctx: GoParser.FunctionDeclContext):
        self.cur_fun = "global"
        self.pop_scope()

    @override
    def exitVarSpec(self, ctx: GoParser.VarSpecContext):
        # 是否是数组
        is_array: bool = False
        array_length: int = 0

        # 如果是数组
        if ctx.type_() and ctx.type_().typeLit():
            is_array = True
            # 如果是数组，长度是多少
            right_values: list[str] = ctx_decoder(
                self.values[
                    ctx.type_().typeLit().arrayType().arrayLength().expression()
                ]
            )
            # 数量是否是1
            if 1 != len(right_values):
                logger.error("wrong number of array length input")
                logger.debug("wrong number of array length input")
                exit(-1)

            array_length_s: str = right_values[0]

            if self.is_digit(array_length_s):
                array_length = int(array_length_s)
                # 数组长度不为负数
                if array_length < 1:
                    logger.error("array length should >=1")
                    logger.debug("array length should >=1")
                    exit(-1)
            else:
                logger.error("array declaration need static capacity")
                logger.debug("array declaration need static capacity")
                exit(-1)

        n: int = len(ctx.identifierList().IDENTIFIER())
        for i in range(n):
            varname: str = ctx.identifierList().IDENTIFIER(i).getText()
            type_: Symbol.Type | None = None
            if not ctx.type_():
                type_ = self.tmp_symbol_type
            else:
                # 变量
                if ctx.type_().typeName():
                    stype: str = ctx.type_().typeName().getText()
                    type_ = Symbol.to_type(stype)
                # 数组
                elif ctx.type_().typeLit() and ctx.type_().typeLit().arrayType():
                    stype: str = (
                        ctx.type_().typeLit().arrayType().elementType().getText()
                    )
                    type_ = Symbol.to_type(stype)

            # 如果已经定义了，报错
            if self.currentScope.cur_resolve(varname) == SUCCESS:
                logger.error(f"Redeclaration of parameter: {varname}")
                logger.debug(f"Redeclaration of parameter: {varname}")
                exit(-1)
            # 不重复，新建
            assert type_ is not None, "type_ is None"
            symbol: Symbol = Symbol(
                varname,
                self.currentScope,
                Symbol.SymbolType.VAR,
                type_=type_,
                is_array=is_array,
                array_length=array_length,
            )

            if is_array:
                self.push_line(
                    TACOP.CREATLIST,
                    Operand(varname, self.operand_type_resolve(varname)),
                    Operand(
                        str(array_length), self.operand_type_resolve(str(array_length))
                    ),
                    Operand("INT", TACOPERANDTYPE.NULL_),
                )
            self.currentScope.para_define(symbol)

        # 定义时赋值
        if ctx.expressionList():
            right_values: list[str] = ctx_decoder(self.values[ctx.expressionList()])
            # 数量是否匹配
            if n != len(right_values):
                logger.error("wrong number matched")
                logger.debug("wrong number matched")
                exit(-1)
            # 赋值
            for i in range(n):
                self.push_line(
                    TACOP.ASSIGN,
                    Operand(
                        right_values[i], self.operand_type_resolve(right_values[i])
                    ),
                    Operand("", TACOPERANDTYPE.NULL_),
                    Operand(
                        ctx.identifierList().IDENTIFIER(i).getText(),
                        self.operand_type_resolve(
                            ctx.identifierList().IDENTIFIER(i).getText()
                        ),
                    ),
                )

    @override
    def enterBlock(self, ctx: GoParser.BlockContext):
        # for 情况
        if ctx.parentCtx.children[0].getText() == "for":
            self.push_line(
                TACOP.LABEL,
                Operand(
                    "FORLOOP" + self.for_values[ctx.parentCtx].cur_index,
                    TACOPERANDTYPE.LABEL,
                ),
                Operand("", TACOPERANDTYPE.NULL_),
                Operand("", TACOPERANDTYPE.NULL_),
            )
        # TODO: need to fix index out of range
        # if else 情况
        if (
                len(ctx.parentCtx.children) > 4
                and ctx.parentCtx.children[4] == ctx
                and ctx.parentCtx.children[3].getText() == "else"
        ):
            self.push_line(
                TACOP.LABEL,
                Operand("ELSE" + self.if_values[ctx.parentCtx], TACOPERANDTYPE.LABEL),
                Operand("", TACOPERANDTYPE.NULL_),
                Operand("", TACOPERANDTYPE.NULL_),
            )
        self.add_scope()

    @override
    def exitBlock(self, ctx: GoParser.BlockContext):
        # for 情况
        if ctx.parentCtx.children[0].getText() == "for":
            for_tmp: ForStatement = self.for_values[ctx.parentCtx]
            update_condition: TACLine = for_tmp.update_con
            loop_condition: TACLine = for_tmp.loop_con
            self.push_line(
                update_condition.op,
                update_condition.src1,
                update_condition.src2,
                update_condition.dst,
            )
            self.push_line(
                loop_condition.op,
                loop_condition.src1,
                loop_condition.src2,
                loop_condition.dst,
            )

        # if 情况
        if (
                ctx.parentCtx.children[2] == ctx
                and ctx.parentCtx.children[0].getText() == "if"
        ):
            self.push_line(
                TACOP.GOTO,
                Operand("ENDIF" + self.if_values[ctx.parentCtx], TACOPERANDTYPE.LABEL),
                Operand("", TACOPERANDTYPE.NULL_),
                Operand("", TACOPERANDTYPE.NULL_),
            )

        self.pop_scope()

    @override
    def exitIncDecStmt(self, ctx: GoParser.IncDecStmtContext):
        if ctx.children[1].getText() == "++":
            left_values: list[str] = ctx_decoder(self.values[ctx.expression()])
            if len(left_values) != 1:
                logger.error('too many parameter for incdec "++"')
                logger.debug('too many parameter for incdec "++"')
                exit(-1)

            # for statement 的情况，不写入3code，但是要记录在forstmt里面，作为update的条件
            if (
                    ctx.parentCtx.parentCtx.parentCtx.children[0].getText() == "for"
                    and ctx.parentCtx.parentCtx
                    == ctx.parentCtx.parentCtx.parentCtx.children[1]
            ):
                varname: str = left_values[0]
                varvalue: str = "1"
                for_tmp: ForStatement = self.for_values[
                    ctx.parentCtx.parentCtx.parentCtx
                ]
                tmpline: TACLine = TACLine(
                    self.line_index,
                    TACOP.ADD,
                    Operand(varname, self.operand_type_resolve(varname)),
                    Operand(varvalue, self.operand_type_resolve(varvalue)),
                    Operand(varname, self.operand_type_resolve(varname)),
                    self.currentScope,
                )
                for_tmp.update_con = tmpline
                self.for_values[ctx.parentCtx.parentCtx.parentCtx] = for_tmp
            else:
                # 普通情况，block里面，写入3code
                varname: str = left_values[0]
                varvalue: str = "1"
                self.push_line(
                    TACOP.ADD,
                    Operand(varname, self.operand_type_resolve(varname)),
                    Operand(varvalue, self.operand_type_resolve(varvalue)),
                    Operand(varname, self.operand_type_resolve(varname)),
                )

    @override
    def exitAssignment(self, ctx: GoParser.AssignmentContext):
        if ctx.assign_op().getText() == "=":
            left_values: list[str] = ctx_decoder(self.values[ctx.expressionList(0)])
            right_values: list[str] = ctx_decoder(self.values[ctx.expressionList(1)])
            # 左右参数量是否相等
            if len(left_values) != len(right_values):
                logger.error("not equal number parameter for assign")
                logger.debug(
                    f"{left_values} != {right_values} not equal number parameter for assign"
                )
                logger.debug(f"{ctx.getText()}")
                exit(-1)

            # ok
            for i in range(len(left_values)):
                varname: str = left_values[i]
                varvalue: str = right_values[i]

                self.push_line(
                    TACOP.ASSIGN,
                    Operand(varvalue, self.operand_type_resolve(varvalue)),
                    Operand("", TACOPERANDTYPE.NULL_),
                    Operand(varname, self.operand_type_resolve(varname)),
                )

        # 未实现数组的
        if ctx.assign_op().getText() == "+=":
            left_values: list[str] = ctx_decoder(self.values[ctx.expressionList(0)])
            right_values: list[str] = ctx_decoder(self.values[ctx.expressionList(1)])
            # 左右参数量是否相等
            if len(left_values) != 1 or len(right_values) != 1:
                logger.error('too many parameter for assign "+="')
                logger.debug('too many parameter for assign "+="')
                exit(-1)

            # for statement 的情况，不写入3code，但是要记录在forstmt里面，作为update的条件
            if (
                    ctx.parentCtx.parentCtx.parentCtx.children[0].getText() == "for"
                    and ctx.parentCtx.parentCtx
                    == ctx.parentCtx.parentCtx.parentCtx.children[1]
            ):
                varname: str = left_values[0]
                varvalue: str = right_values[0]
                for_tmp: ForStatement = self.for_values[
                    ctx.parentCtx.parentCtx.parentCtx
                ]
                tmpline: TACLine = TACLine(
                    self.line_index,
                    TACOP.ADD,
                    Operand(varname, self.operand_type_resolve(varname)),
                    Operand(varvalue, self.operand_type_resolve(varvalue)),
                    Operand(varname, self.operand_type_resolve(varname)),
                    self.currentScope,
                )
                for_tmp.update_con = tmpline
                self.for_values[ctx.parentCtx.parentCtx.parentCtx] = for_tmp
            # 普通情况，block里面，写入3code
            else:
                varname: str = left_values[0]
                varvalue: str = right_values[0]
                self.push_line(
                    TACOP.ADD,
                    Operand(varname, self.operand_type_resolve(varname)),
                    Operand(varvalue, self.operand_type_resolve(varvalue)),
                    Operand(varname, self.operand_type_resolve(varname)),
                )

    @override
    def exitShortVarDecl(self, ctx: GoParser.ShortVarDeclContext):
        n: int = len(ctx.identifierList().IDENTIFIER())
        for i in range(n):
            varname: str = ctx.identifierList().IDENTIFIER(i).getText()
            type_: Symbol.Type = self.tmp_symbol_type

            # 不重复，新建
            symbol: Symbol = Symbol(
                varname, self.currentScope, Symbol.SymbolType.VAR, type_=type_
            )
            self.currentScope.para_define(symbol)

        # TODO:如果是函数调用怎么办？？
        if ctx.expressionList():
            right_values: list[str] = ctx_decoder(self.values[ctx.expressionList()])
            # 数量是否匹配
            if n != len(right_values):
                logger.error("wrong number matched")
                logger.debug("wrong number matched")
                exit(-1)
            # 赋值
            for i in range(n):
                self.push_line(
                    TACOP.ASSIGN,
                    Operand(
                        right_values[i], self.operand_type_resolve(right_values[i])
                    ),
                    Operand("", TACOPERANDTYPE.NULL_),
                    Operand(
                        ctx.identifierList().IDENTIFIER(i).getText(),
                        self.operand_type_resolve(
                            ctx.identifierList().IDENTIFIER(i).getText()
                        ),
                    ),
                )

        if (
                ctx.parentCtx.parentCtx.parentCtx.children[0].getText() == "for"
                and ctx.parentCtx.parentCtx.parentCtx.children[1] == ctx.parentCtx.parentCtx
        ):
            # init 变量添加进forstmt结构体，方便在forstmt结束后销毁变量
            for_tmp: ForStatement = self.for_values[ctx.parentCtx.parentCtx.parentCtx]
            for i in range(n):
                varname: str = ctx.identifierList().IDENTIFIER(i).getText()
                for_tmp.new_paras.append(varname)
            self.for_values[ctx.parentCtx.parentCtx.parentCtx] = for_tmp

    @override
    def exitReturnStmt(self, ctx: GoParser.ReturnStmtContext):
        return_values: list[str] = ctx_decoder(self.values[ctx.expressionList()])
        # 是否声明
        for i in return_values:
            tmp: Symbol | None = self.currentScope.resolve(i)
            if tmp is None:
                logger.error(f"Undefined: {i}")
                logger.debug(f"Undefined: {i}")
                exit(-1)

        # 生成TAC
        for i in return_values:
            self.push_line(
                TACOP.FUN_RET,
                Operand(i, self.operand_type_resolve(i)),
                Operand("", TACOPERANDTYPE.NULL_),
                Operand("", TACOPERANDTYPE.NULL_),
            )

    @override
    def enterIfStmt(self, ctx: GoParser.IfStmtContext):
        if_tmp: str = self.create_else_label()
        self.if_values[ctx] = if_tmp

    @override
    def exitIfStmt(self, ctx: GoParser.IfStmtContext):
        self.push_line(
            TACOP.LABEL,
            Operand("ENDIF" + self.if_values[ctx], TACOPERANDTYPE.LABEL),
            Operand("", TACOPERANDTYPE.NULL_),
            Operand("", TACOPERANDTYPE.NULL_),
        )

    @override
    def enterForStmt(self, ctx: GoParser.ForStmtContext):
        tmp: str = self.create_for_label()
        newfor: ForStatement = ForStatement(tmp)
        logger.debug(newfor.cur_index)
        self.for_values[ctx] = newfor
        self.add_scope()

    @override
    def exitForStmt(self, ctx: GoParser.ForStmtContext):
        tmp: ForStatement = self.for_values[ctx]
        self.pop_scope()
        self.push_line(
            TACOP.LABEL,
            Operand("ENDFOR" + self.for_values[ctx].cur_index, TACOPERANDTYPE.LABEL),
            Operand("", TACOPERANDTYPE.NULL_),
            Operand("", TACOPERANDTYPE.NULL_),
        )

    @override
    def enterParameterDecl(self, ctx: GoParser.ParameterDeclContext):
        if ctx.identifierList():
            # TODO: What if identifierList returns None?
            n: int = len(ctx.identifierList().IDENTIFIER())
            for i in range(n):
                integer: str = ctx.identifierList().IDENTIFIER(i).getText()
                stype: str = ctx.type_().typeName().getText()
                type_: Symbol.Type = Symbol.to_type(stype)
                symbol: Symbol = Symbol(
                    integer, self.currentScope, Symbol.SymbolType.VAR, type_=type_
                )
                self.currentScope.para_define(symbol)

    @override
    def exitOperandName(self, ctx: GoParser.OperandNameContext):
        tmp: Symbol | None = self.currentScope.resolve(ctx.IDENTIFIER().getText())
        if tmp is None:
            logger.error(f"Undefined: {ctx.IDENTIFIER().getText()}")
            exit(-1)
        self.values[ctx] = ctx.IDENTIFIER().getText() + DELIMITER

    @override
    def exitArguments(self, ctx: GoParser.ArgumentsContext):
        if ctx.expressionList():
            self.values[ctx] = self.values[ctx.expressionList()]
