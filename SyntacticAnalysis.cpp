#include <string>
#include "SyntacticAnalysis.h"

// 语法分析初始化
Syntax::Syntax()
{
    errPos = 0;
    errMessage = "Error! Parsing failed before ";
    syntaxProcess.clear();
    processIndex = 0;
}

// 语法分析初始化
Syntax::Syntax(Scan scanner)
{
    errPos = 0;
    errMessage = "Error! Parsing failed before ";
    syntaxProcess.clear();
    processIndex = 0;
    this->scanner = scanner;
}

// 推导过程遍历，在此插入动作函数
void Syntax::prepare_for_4elem()
{
    switch(syntaxProcess.at(processIndex++))
    {
        case PROCESS_PROGRAM:
            {
                initAll();
                prepare_for_4elem();
                return;
            }
        case PROCESS_DECLARATION_LIST:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_DECLARATION_LIST_TAIL_1:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_DECLARATION_LIST_TAIL_2:
            {
                return;
            }
        case PROCESS_DECLARATION_1:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_DECLARATION_2:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_VAR_DECLARATION_1:
            {
                prepare_for_4elem();
                Token id = scanner.next();
                scanner.next();
                addNum(id.identifierAndIntPos);
                return;
            }
        case PROCESS_VAR_DECLARATION_2:
            {
                prepare_for_4elem();
                Token id = scanner.next();
                scanner.next();
                Token num = scanner.next();
                addArray(id.identifierAndIntPos, num.valueInt);
                scanner.next();
                scanner.next();
                return;
            }
        case PROCESS_TYPE_SPECFIER_1:
            {
                scanner.next();
                return;
            }
        case PROCESS_TYPE_SPECFIER_2:
            {
                scanner.next();
                return;
            }
        case PROCESS_FUN_DECLARATION:
            {
                prepare_for_4elem();
                Token fun = scanner.next();
                addFun(fun.identifierAndIntPos);
                scanner.next();
                prepare_for_4elem();
                scanner.next();
                prepare_for_4elem();
                backFun();
                return;
            }
        case PROCESS_PARAMS_1:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_PARAMS_2:
            {
                scanner.next();
                return;
            }
        case PROCESS_PARAM_LIST:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_PARAM_LIST_TAIL_1:
            {
                scanner.next();
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_PARAM_LIST_TAIL_2:
            {
                return;
            }
        case PROCESS_PARAM_1:
            {
                prepare_for_4elem();
                Token id = scanner.next();
                addNum(id.identifierAndIntPos, true);
                return;
            }
        case PROCESS_PARAM_2:
            {
                prepare_for_4elem();
                Token id = scanner.next();
                addArray(id.identifierAndIntPos, true);
                return;
            }
        case PROCESS_COMPOUND_STMT:
            {
                scanner.next();
                prepare_for_4elem();
                prepare_for_4elem();
                scanner.next();
                return;
            }
        case PROCESS_LOCAL_DECLARATIONS_1:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_LOCAL_DECLARATIONS_2:
            {
                return;
            }
        case PROCESS_LOCAL_DECLARATIONS_TAIL_1:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_LOCAL_DECLARATIONS_TAIL_2:
            {
                return;
            }
        case PROCESS_STATEMENT_LIST_1:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_STATEMENT_LIST_2:
            {
                return;
            }
        case PROCESS_STATEMENT_LIST_TAIL_1:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_STATEMENT_LIST_TAIL_2:
            {
                return;
            }
        case PROCESS_STATEMENT_1:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_STATEMENT_2:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_STATEMENT_3:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_STATEMENT_4:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_STATEMENT_5:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_STATEMENT_6:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_STATEMENT_7:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_EXPRESSION_STMT_1:
            {
                prepare_for_4elem();
                scanner.next();
                return;
            }
        case PROCESS_EXPRESSION_STMT_2:
            {
                scanner.next();
                return;
            }
        case PROCESS_SELECTION_STMT_1:
            {
                scanner.next();
                scanner.next();
                prepare_for_4elem();
                exIf();
                scanner.next();
                prepare_for_4elem();
                exIe();
                return;
            }
        case PROCESS_SELECTION_STMT_2:
            {
                scanner.next();
                scanner.next();
                prepare_for_4elem();
                exIf();
                scanner.next();
                prepare_for_4elem();
                scanner.next();
                exEl();
                prepare_for_4elem();
                exIe();
                return;
            }
        case PROCESS_ITERATION_STMT:
            {
                scanner.next();
                exWh();
                scanner.next();
                prepare_for_4elem();
                exDo();
                scanner.next();
                prepare_for_4elem();
                exWe();
                return;
            }
        case PROCESS_RETURN_STMT_1:
            {
                scanner.next();
                retNonum();
                scanner.next();
                return;
            }
        case PROCESS_RETURN_STMT_2:
            {
                scanner.next();
                prepare_for_4elem();
                retNum();
                scanner.next();
                return;
            }
        case PROCESS_EXPRESSION_1:
            {
                prepare_for_4elem();
                scanner.next();
                prepare_for_4elem();
                alGeq("=");
                return;
            }
        case PROCESS_EXPRESSION_2:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_VAR_1:
            {
                Token id = scanner.next();
                alPush(id.identifierAndIntPos);
                return;
            }
        case PROCESS_VAR_2:
            {
                Token id = scanner.next();
                scanner.next();
                prepare_for_4elem();
                scanner.next();
                alPush(id.identifierAndIntPos);
                return;
            }
        case PROCESS_SIMPLE_EXPRESSION_1:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                scanner.back();
                Token op = scanner.next();
                prepare_for_4elem();
                alGeq(op.name);
                return;
            }
        case PROCESS_SIMPLE_EXPRESSION_2:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_RELOP_1:
            {
                scanner.next();
                return;
            }
        case PROCESS_RELOP_2:
            {
                scanner.next();
                return;
            }
        case PROCESS_RELOP_3:
            {
                scanner.next();
                return;
            }
        case PROCESS_RELOP_4:
            {
                scanner.next();
                return;
            }
        case PROCESS_RELOP_5:
            {
                scanner.next();
                return;
            }
        case PROCESS_RELOP_6:
            {
                scanner.next();
                return;
            }
        case PROCESS_ADDITIVE_EXPRESSION:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_ADDITIVE_EXPRESSION_TAIL_1:
            {
                prepare_for_4elem();
                scanner.back();
                Token op = scanner.next();
                prepare_for_4elem();
                alGeq(op.name);
                prepare_for_4elem();
                return;
            }
        case PROCESS_ADDITIVE_EXPRESSION_TAIL_2:
            {
                return;
            }
        case PROCESS_ADDOP_1:
            {
                scanner.next();
                return;
            }
        case PROCESS_ADDOP_2:
            {
                scanner.next();
                return;
            }
        case PROCESS_TERM:
            {
                prepare_for_4elem();
                prepare_for_4elem();
                return;
            }
        case PROCESS_TERM_TAIL_1:
            {
                prepare_for_4elem();
                scanner.back();
                Token op = scanner.next();
                prepare_for_4elem();
                alGeq(op.name);
                prepare_for_4elem();
                return;
            }
        case PROCESS_TERM_TAIL_2:
            {
                return;
            }
        case PROCESS_MULOP_1:
            {
                scanner.next();
                return;
            }
        case PROCESS_MULOP_2:
            {
                scanner.next();
                return;
            }
        case PROCESS_MULOP_3:
            {
                scanner.next();
                return;
            }
        case PROCESS_FACTOR_1:
            {
                scanner.next();
                prepare_for_4elem();
                scanner.next();
                return;
            }
        case PROCESS_FACTOR_2:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_FACTOR_3:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_FACTOR_4:
            {
                Token num = scanner.next();
                addCon(num.identifierAndIntPos, num.valueInt);
                alPush(num.identifierAndIntPos);
                return;
            }
        case PROCESS_CALL:
            {
                Token id = scanner.next();
                scanner.next();
                callBegin(id.identifierAndIntPos);
                prepare_for_4elem();
                scanner.next();
                callEnd();
                return;
            }
        case PROCESS_ARGS_1:
            {
                prepare_for_4elem();
                return;
            }
        case PROCESS_ARGS_2:
            {
                return;
            }
        case PROCESS_ARG_LIST:
            {
                prepare_for_4elem();
                callParam();
                prepare_for_4elem();
                return;
            }
        case PROCESS_ARG_LIST_TAIL_1:
            {
                scanner.next();
                prepare_for_4elem();
                callParam();
                prepare_for_4elem();
                return;
            }
        case PROCESS_ARG_LIST_TAIL_2:
            {
                return;
            }
        case PROCESS_INPUT_STMT:
            {
                scanner.next();
                scanner.next();
                prepare_for_4elem();
                exIn();
                scanner.next();
                scanner.next();
                return;
            }
        case PROCESS_OUTPUT_STMT:
            {
                scanner.next();
                scanner.next();
                prepare_for_4elem();
                exOut();
                scanner.next();
                scanner.next();
                return;
            }
        default:
            {
                return;
            }
    }
}

int Syntax::program()
{
    #ifdef DEBUG
    printf("----------program\n");
    #endif
    int res, ori_index, ori_process;
    //initAll();
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_PROGRAM);
    res = declaration_list();
    if(res == 0)
    {
        return 0;   // <program> := <declaration_list>
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::declaration_list()
{
    #ifdef DEBUG
    printf("----------declaration_list\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_DECLARATION_LIST);
    res = declaration();
    if(res == 0)
    {
        res = declaration_list_tail();
        if(res == 0)
        {
            return 0;   // <declaration_list> := <declaration> <declaration_list_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::declaration_list_tail()
{
    #ifdef DEBUG
    printf("----------declaration_list_tail\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_DECLARATION_LIST_TAIL_1);
    res = declaration();
    if(res == 0)
    {
        res = declaration_list_tail();
        if(res == 0)
        {
            return 0;   // <declaration_list_tail> := <declaration> <deliaration_list_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_DECLARATION_LIST_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // <declaration_list_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_DECLARATION_LIST_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // <declaration_list_tail> := <empty>
    }
}

int Syntax::declaration()
{
    #ifdef DEBUG
    printf("----------declaration\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_DECLARATION_1);
    res = var_declaration();
    if(res == 0)
    {
        return 0;   // <declaration> := <var_declaration>
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_DECLARATION_2);
        scanner.setIndex(ori_index);
        res = fun_declaration();
        if(res == 0)
        {
            return 0;   // <declaration> := <fun_declaration>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            scanner.setIndex(ori_index);
            return res;
        }
    }
}

int Syntax::var_declaration()
{
    #ifdef DEBUG
    printf("----------var_declaration\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_VAR_DECLARATION_1);
    res = type_specfier();
    if(res == 0)
    {
        Token temp = scanner.next(),pretemp;
        if(temp.isIdentifier())
        {
            pretemp=temp;
            temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string(";"))
            {
                //addNum(pretemp.identifierAndIntPos);
                return 0;   // <var_declaration> := <type_specifier> ID ;
            }
            else if(temp.isDelimiter()&&temp.name == std::string("["))
            {
                temp = scanner.next();
                if(temp.isInt())
                {
                    //addArray(pretemp.identifierAndIntPos,temp.valueInt);
                    temp = scanner.next();
                    if(temp.isDelimiter()&&temp.name == std::string("]"))
                    {
                        temp = scanner.next();
                        if(temp.isDelimiter()&&temp.name == std::string(";"))
                        {
                            syntaxProcess.at(ori_process) = PROCESS_VAR_DECLARATION_2;
                            return 0;   // <var_declaration> := <type_specifier> ID [ NUM ] ;
                        }
                        else
                        {
                            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                            if(scanner.curIndex - 1 > errPos)
                            {
                                errPos = scanner.curIndex - 1;
                                errMessage = "Error! Need ; before ";
                            }
                            scanner.setIndex(ori_index);
                            return -1;
                        }
                    }
                    else
                    {
                        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                        if(scanner.curIndex - 1 > errPos)
                        {
                            errPos = scanner.curIndex - 1;
                            errMessage = "Error! Need ] before ";
                        }
                        scanner.setIndex(ori_index);
                        return -1;
                    }
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "Error! Need NUM before ";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "Error! Need ; or [ before ";
                }
                scanner.setIndex(ori_index);
                return -1;
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need ID before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::fun_declaration()
{
    #ifdef DEBUG
    printf("----------fun_declaration\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_FUN_DECLARATION);
    res = type_specfier();
    if(res == 0)
    {
        Token temp = scanner.next();
        if(temp.isIdentifier())
        {
            //addFun(temp.identifierAndIntPos);
            temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string("("))
            {
                res = params();
                if(res == 0)
                {
                    temp = scanner.next();
                    if(temp.isDelimiter()&&temp.name == std::string(")"))
                    {
                        res = compound_stmt();
                        if(res == 0)
                        {
                            //backFun();
                            return 0;   // <fun_declaration> := <type_specifier> ID ( <params> ) <compound_stmt>
                        }
                        else
                        {
                            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                            scanner.setIndex(ori_index);
                            return res;
                        }
                        
                    }
                    else
                    {
                        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                        if(scanner.curIndex - 1 > errPos)
                        {
                            errPos = scanner.curIndex - 1;
                            errMessage = "Error! Need ) before ";
                        }
                        scanner.setIndex(ori_index);
                        return -1;
                    }
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    scanner.setIndex(ori_index);
                    return res;
                }
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "Error! Need ( before ";
                }
                scanner.setIndex(ori_index);
                return -1;
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need ID before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::type_specfier()
{
    #ifdef DEBUG
    printf("----------type_specfier\n");
    #endif
    int ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("int"))
    {
        syntaxProcess.push_back(PROCESS_TYPE_SPECFIER_1);
        return 0;   // <type_specifier> := int
    }
    else if(temp.isKeyWord()&&temp.name == std::string("void"))
    {
        syntaxProcess.push_back(PROCESS_TYPE_SPECFIER_2);
        return 0;   // <type_specifier> := void
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need int or void before ";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::params()
{
    #ifdef DEBUG
    printf("----------params\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_PARAMS_1);
    res = param_list();
    if(res == 0)
    {
        return 0;   // <params> := <param_list>
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_PARAMS_2);
        scanner.setIndex(ori_index);
        Token temp = scanner.next();
        if(temp.isKeyWord()&&temp.name == std::string("void"))
        {
            return 0;   // <params> := void
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need void before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
}

int Syntax::param_list()
{
    #ifdef DEBUG
    printf("----------param_list\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_PARAM_LIST);
    res = param();
    if(res == 0)
    {
        res = param_list_tail();
        if(res == 0)
        {
            return 0;   // <param_list> := <parm> <parm_list_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::param_list_tail()
{
    #ifdef DEBUG
    printf("----------param_list_tail\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_PARAM_LIST_TAIL_1);
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string(","))
    {
        res = param();
        if(res == 0)
        {
            res = param_list_tail();
            if(res == 0)
            {
                return 0;   // <parm_list_tail> := , <param> <parm_list_tail>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_PARAM_LIST_TAIL_2);
                scanner.setIndex(ori_index);
                return 0;   // <parm_list_tail> := <empty>
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_PARAM_LIST_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // <parm_list_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_PARAM_LIST_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // <parm_list_tail> := <empty>
    }
}

int Syntax::param()
{
    #ifdef DEBUG
    printf("----------param\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_PARAM_2);
    res = type_specfier();
    if(res == 0)
    {
        Token temp = scanner.next(),pretemp;
        if(temp.isIdentifier())
        {
            pretemp=temp;
            temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string("["))
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string("]"))
                {
                    //addArray(pretemp.identifierAndIntPos,true);
                    return 0;   // <param> := <type_specifier> ID [ ]
                }
                else
                {
                    //addNum(pretemp.identifierAndIntPos,true);
                    syntaxProcess.at(ori_process) = PROCESS_PARAM_1;
                    scanner.back();
                    scanner.back();
                    return 0;   // <param> := <type_specifier> ID
                }
            }
            else
            {
                //addNum(pretemp.identifierAndIntPos,true);
                syntaxProcess.at(ori_process) = PROCESS_PARAM_1;
                scanner.back();
                return 0;   // <param> := <type_specifier> ID
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need ID before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::compound_stmt()
{
    #ifdef DEBUG
    printf("----------compound_stmt\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_COMPOUND_STMT);
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("{"))
    {
        res = local_declarations();
        if(res == 0)
        {
            res = statement_list();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string("}"))
                {
                    return 0;   // <compound_stmt> := { <local_declarations> <statement_list> }
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "Error! Need } before ";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {

            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need { before ";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::local_declarations()
{
    #ifdef DEBUG
    printf("----------local_declarations\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_1);
    res = var_declaration();
    if(res == 0)
    {
        res = local_declarations_tail();
        if(res == 0)
        {
            return 0;   // <local_declarations> := <var_declaration> <local_declarations_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_2);
            scanner.setIndex(ori_index);
            return 0;   // <local_declarations> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_2);
        scanner.setIndex(ori_index);
        return 0;   // <local_declarations> := <empty>
    }
}

int Syntax::local_declarations_tail()
{
    #ifdef DEBUG
    printf("----------local_declarations_tail\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_TAIL_1);
    res = var_declaration();
    if(res == 0)
    {
        res = local_declarations_tail();
        if(res == 0)
        {
            return 0;   // <local_declarations_tail> := <var_declaration> <local_declarations_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // <local_declarations_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // <local_declarations_tail> := <empty>
    }
}

int Syntax::statement_list()
{
    #ifdef DEBUG
    printf("----------statement_list\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_STATEMENT_LIST_1);
    res = statement();
    if(res == 0)
    {
        res = statement_list_tail();
        if(res == 0)
        {
            return 0;   // <statement_list> := <statement> <statement_list_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_STATEMENT_LIST_2);
            scanner.setIndex(ori_index);
            return 0;   // <statement_list> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_STATEMENT_LIST_2);
        scanner.setIndex(ori_index);
        return 0;   // <statement_list> := <empty>
    }
}

int Syntax::statement_list_tail()
{
    #ifdef DEBUG
    printf("----------statement_list_tail\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_STATEMENT_LIST_TAIL_1);
    res = statement();
    if(res == 0)
    {
        res = statement_list_tail();
        if(res == 0)
        {
            return 0;   // <statement_list_tail> := <statement> <statement_list_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_STATEMENT_LIST_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // <statement_list_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_STATEMENT_LIST_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // <statement_list_tail> := <empty>
    }
}

int Syntax::statement()
{
    #ifdef DEBUG
    printf("----------statement\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_STATEMENT_1);
    res = expression_stmt();
    if(res == 0)
    {
        return 0;   // <statement> := <expression_stmt>
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_STATEMENT_2);
        scanner.setIndex(ori_index);
        res = compound_stmt();
        if(res == 0)
        {
            return 0;   // <statement> := <compound_stmt>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_STATEMENT_3);
            scanner.setIndex(ori_index);
            res = selection_stmt();
            if(res == 0)
            {
                return 0;   // <statement> := <selection_stmt>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_STATEMENT_4);
                scanner.setIndex(ori_index);
                res = iteration_stmt();
                if(res == 0)
                {
                    return 0;   // <statement> := <iteration_stmt>
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    syntaxProcess.push_back(PROCESS_STATEMENT_5);
                    scanner.setIndex(ori_index);
                    res = return_stmt();
                    if(res == 0)
                    {
                        return 0;   // <statement> := <return_stmt>
                    }
                    else
                    {
                        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                        syntaxProcess.push_back(PROCESS_STATEMENT_6);
                        scanner.setIndex(ori_index);
                        res = input_stmt();
                        if(res == 0)
                        {
                            return 0;   // <statement> := <input_stmt>
                        }
                        else
                        {
                            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                            syntaxProcess.push_back(PROCESS_STATEMENT_7);
                            scanner.setIndex(ori_index);
                            res = output_stmt();
                            if(res == 0)
                            {
                                return 0;   // <statement> := <output_stmt>
                            }
                            else
                            {
                                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                                scanner.setIndex(ori_index);
                                return res;
                            }
                        }
                    }
                }
            }
        }
    }
}

int Syntax::expression_stmt()
{
    #ifdef DEBUG
    printf("----------expression_stmt\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_EXPRESSION_STMT_1);
    res = expression();
    if(res == 0)
    {
        Token temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string(";"))
        {
            return 0;   // <expression_stmt> := <expression> ;
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_EXPRESSION_STMT_2);
            scanner.setIndex(ori_index);
            Token temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string(";"))
            {
                return 0;   // <expression_stmt> := ;
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "Error! Need ; before ";
                }
                scanner.setIndex(ori_index);
                return -1;
            }
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_EXPRESSION_STMT_2);
        scanner.setIndex(ori_index);
        Token temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string(";"))
        {
            return 0;   // <expression_stmt> := ;
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need ; before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
}

int Syntax::selection_stmt()
{
    #ifdef DEBUG
    printf("----------selection_stmt\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_SELECTION_STMT_2);
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("if"))
    {
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("("))
        {
            res = expression();
            if(res == 0)
            {
                //exIf();
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string(")"))
                {
                    res = statement();
                    if(res == 0)
                    {
                        int another_index = scanner.curIndex;
                        int another_process = syntaxProcess.size();
                        temp = scanner.next();
                        if(temp.isKeyWord()&&temp.name == std::string("else"))
                        {
                            //exEl();
                            res = statement();
                            if(res == 0)
                            {
                                //exIe();
                                return 0;   // <selection_stmt> := if ( <expression> ) <statement> else <statement>
                            }
                            else
                            {
                                //exIe();
                                while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
                                syntaxProcess.at(ori_process) = PROCESS_SELECTION_STMT_1;
                                scanner.setIndex(another_index);
                                return 0;   // <selection_stmt> := if ( <expression> ) <statement>
                            }
                        }
                        else
                        {
                            //exIe();
                            syntaxProcess.at(ori_process) = PROCESS_SELECTION_STMT_1;
                            scanner.back();
                            return 0;   // <selection_stmt> := if ( <expression> ) <statement>
                        }
                    }
                    else
                    {
                        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                        scanner.setIndex(ori_index);
                        return res;
                    }
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "Error! Need ) before ";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need ( before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need if before ";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::iteration_stmt()
{
    #ifdef DEBUG
    printf("----------iteration_stmt\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_ITERATION_STMT);
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("while"))
    {
        //exWh();
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("("))
        {
            res = expression();
            if(res == 0)
            {
                temp = scanner.next();
                //exDo();
                if(temp.isDelimiter()&&temp.name == std::string(")"))
                {
                    res = statement();
                    if(res == 0)
                    {
                        //exWe();
                        return 0;   // <iteration_stmt> := while ( <expression> ) <statement>
                    }
                    else
                    {
                        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                        scanner.setIndex(ori_index);
                        return res;
                    }
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "Error! Need ) before ";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need ( before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need while before ";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::return_stmt()
{
    #ifdef DEBUG
    printf("----------return_stmt\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_RETURN_STMT_1);
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("return"))
    {
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string(";"))
        {
            return 0;   // <return_stmt> := return ;
        }
        else
        {
            scanner.back();
            res = expression();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string(";"))
                {
                    syntaxProcess.at(ori_process) = PROCESS_RETURN_STMT_2;
                    return 0;   // <return_stmt> := return <expression> ;
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "Error! Need ; before ";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                scanner.setIndex(ori_index);
                return res;
            }
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need return before ";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::expression()
{
    #ifdef DEBUG
    printf("----------expression\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_EXPRESSION_1);
    res = var();
    if(res == 0)
    {
        Token temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("="))
        {
            res = expression();
            if(res == 0)
            {
                //alGeq("=");
                return 0;   // <expression> := <var> = <expression>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_EXPRESSION_2);
                scanner.setIndex(ori_index);
                res = simple_expression();
                if(res == 0)
                {
                    return 0;   // 41 <expression> := <simple_expression>
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    scanner.setIndex(ori_index);
                    return res;
                }
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_EXPRESSION_2);
            scanner.setIndex(ori_index);
            res = simple_expression();
            if(res == 0)
            {
                return 0;   // <expression> := <simple_expression>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                scanner.setIndex(ori_index);
                return res;
            }
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_EXPRESSION_2);
        scanner.setIndex(ori_index);
        res = simple_expression();
        if(res == 0)
        {
            return 0;   // <expression> := <simple_expression>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            scanner.setIndex(ori_index);
            return res;
        }
    }
}

int Syntax::var()
{
    #ifdef DEBUG
    printf("----------var\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_VAR_2);
    Token temp = scanner.next(),pretemp;
    if(temp.isIdentifier())
    {
        pretemp=temp;
        int another_index = scanner.curIndex;
        int another_process = syntaxProcess.size();
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == "[")
        {
            res = expression();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == "]")
                {
                    //alPush(pretemp.identifierAndIntPos,true);
                    return 0;   // <var> := ID [ <expression> ]
                }
                else
                {
                    //alPush(pretemp.identifierAndIntPos);
                    while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
                    syntaxProcess.at(ori_process) = PROCESS_VAR_1;
                    scanner.setIndex(another_index);
                    return 0;   // <var> := ID
                }
            }
            else
            {
                //alPush(pretemp.identifierAndIntPos);
                while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
                syntaxProcess.at(ori_process) = PROCESS_VAR_1;
                scanner.setIndex(another_index);
                return 0;   // <var> := ID
            }
        }
        else
        {
            //alPush(pretemp.identifierAndIntPos);
            syntaxProcess.at(ori_process) = PROCESS_VAR_1;
            scanner.back();
            return 0;   // <var> := ID
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need ID before ";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::simple_expression()
{
    #ifdef DEBUG
    printf("----------simple_expression\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_SIMPLE_EXPRESSION_1);
    res = additive_expression();
    if(res == 0)
    {
        int another_index = scanner.curIndex;
        int another_process = syntaxProcess.size();
        res = relop();
        if(res == 0)
        {
            scanner.back();
            Token temp = scanner.next();    // temp.name: "<=" "<" ">" ">=" "==" "!="
            res = additive_expression();
            if(res == 0)
            {
                //alGeq(temp.name);
                return 0;   // <simple_expression> := <additive_expression> <relop> <additive_expression>
            }
            else
            {
                while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
                syntaxProcess.at(ori_process) = PROCESS_SIMPLE_EXPRESSION_2;
                scanner.setIndex(another_index);
                return 0;   // <simple_expression> := <additive_expression>
            }
        }
        else
        {
            while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
            syntaxProcess.at(ori_process) = PROCESS_SIMPLE_EXPRESSION_2;
            scanner.setIndex(another_index);
            return 0;   // <simple_expression> := <additive_expression>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::additive_expression()
{
    #ifdef DEBUG
    printf("----------additive_expression\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_ADDITIVE_EXPRESSION);
    res = term();
    if(res == 0)
    {
        res = additive_expression_tail();
        if(res == 0)
        {
            return 0;   // <additive_expression> := <term> <additive_expression_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::additive_expression_tail()
{
    #ifdef DEBUG
    printf("----------additive_expression_tail\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_ADDITIVE_EXPRESSION_TAIL_1);
    res = addop();
    if(res == 0)
    {
        scanner.back();
        Token temp = scanner.next();    // temp.name: "+" "-"
        res = term();
        if(res == 0)
        {
            //alGeq(temp.name);
            res = additive_expression_tail();
            if(res == 0)
            {
                return 0;   // <additive_expression_tail> := <addop> <term> <additive_expression_tail>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_ADDITIVE_EXPRESSION_TAIL_2);
                scanner.setIndex(ori_index);
                return 0;   // <additive_expression_tail> := <empty>
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_ADDITIVE_EXPRESSION_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // <additive_expression_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_ADDITIVE_EXPRESSION_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // <additive_expression_tail> := <empty>
    }
}

int Syntax::relop()
{
    #ifdef DEBUG
    printf("----------relop\n");
    #endif
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("<="))
    {
        syntaxProcess.push_back(PROCESS_RELOP_1);
        return 0;   // <relop> := <=
    }
    else if(temp.isDelimiter()&&temp.name == std::string("<"))
    {
        syntaxProcess.push_back(PROCESS_RELOP_2);
        return 0;   // <relop> := <
    }
    else if(temp.isDelimiter()&&temp.name == std::string(">"))
    {
        syntaxProcess.push_back(PROCESS_RELOP_3);
        return 0;   // <relop> := >
    }
    else if(temp.isDelimiter()&&temp.name == std::string(">="))
    {
        syntaxProcess.push_back(PROCESS_RELOP_4);
        return 0;   // <relop> := >=
    }
    else if(temp.isDelimiter()&&temp.name == std::string("=="))
    {
        syntaxProcess.push_back(PROCESS_RELOP_5);
        return 0;   // <relop> := ==
    }
    else if(temp.isDelimiter()&&temp.name == std::string("!="))
    {
        syntaxProcess.push_back(PROCESS_RELOP_6);
        return 0;   // <relop> := !=
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need <=, <, >, >=, == or != before ";
        }
        scanner.back();
        return -1;
    }
}

int Syntax::term()
{
    #ifdef DEBUG
    printf("----------term\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_TERM);
    res = factor();
    if(res == 0)
    {
        res = term_tail();
        if(res == 0)
        {
            return 0;   // <term> := <factor> <term_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::term_tail()
{
    #ifdef DEBUG
    printf("----------term_tail\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_TERM_TAIL_1);
    res = mulop();
    if(res == 0)
    {
        scanner.back();
        Token temp = scanner.next();    // temp.name: "*" "/"
        res = factor();
        if(res == 0)
        {
            //alGeq(temp.name);
            res = term_tail();
            if(res == 0)
            {
                return 0;   // <term_tail> := <mulop> <factor> <term_tail>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_TERM_TAIL_2);
                scanner.setIndex(ori_index);
                return 0;   // <term_tail> := <empty>
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_TERM_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // <term_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_TERM_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // <term_tail> := <empty>
    }
}

int Syntax::addop()
{
    #ifdef DEBUG
    printf("----------addop\n");
    #endif
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("+"))
    {
        syntaxProcess.push_back(PROCESS_ADDOP_1);
        return 0;   // <addop> := +
    }
    else if(temp.isDelimiter()&&temp.name == std::string("-"))
    {
        syntaxProcess.push_back(PROCESS_ADDOP_2);
        return 0;   // <addop> := -
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need + or - before ";
        }
        scanner.back();
        return -1;
    }
}

int Syntax::factor()
{
    #ifdef DEBUG
    printf("----------factor\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_FACTOR_1);
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("("))
    {
        res = expression();
        if(res == 0)
        {
            temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string(")"))
            {
                return 0;   // <factor> := ( <expression> )
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "Error! Need ) before ";
                }
                scanner.setIndex(ori_index);
                return -1;
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else if(temp.isInt())
    {
        //addCon(temp.identifierAndIntPos,temp.valueInt);
        //alPush(temp.identifierAndIntPos);
        syntaxProcess.at(ori_process) = PROCESS_FACTOR_4;
        return 0;   // <factor> := NUM
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_FACTOR_3);
        scanner.setIndex(ori_index);
        res = call();
        if(res == 0)
        {
            return 0;   // <factor> := <call>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_FACTOR_2);
            scanner.setIndex(ori_index);
            res = var();
            if(res == 0)
            {
                return 0;   // <factor> := <var>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                scanner.setIndex(ori_index);
                return res;
            }
        }
    }
}

int Syntax::mulop()
{
    #ifdef DEBUG
    printf("----------mulop\n");
    #endif
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("*"))
    {
        syntaxProcess.push_back(PROCESS_MULOP_1);
        return 0;   // <mulop> := *
    }
    else if(temp.isDelimiter()&&temp.name == std::string("/"))
    {
        syntaxProcess.push_back(PROCESS_MULOP_2);
        return 0;   // <mulop> := /
    }
    else if(temp.isDelimiter()&&temp.name == std::string("%"))
    {
        syntaxProcess.push_back(PROCESS_MULOP_3);
        return 0;   // <mulop> := %
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need * or / before ";
        }
        scanner.back();
        return -1;
    }
}

int Syntax::call()
{
    #ifdef DEBUG
    printf("----------call\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_CALL);
    Token temp = scanner.next(),pretemp;
    if(temp.isIdentifier())
    {
        pretemp=temp;
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("("))
        {
            //callBegin(pretemp.identifierAndIntPos);
            res = args();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string(")"))
                {
                    //callEnd();
                    return 0;   // <call> := ID ( <args> )
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "Error! Need ) before ";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need ( before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need ID before ";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::args()
{
    #ifdef DEBUG
    printf("----------args\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_ARGS_1);
    res = arg_list();
    if(res == 0)
    {
        return 0;   // <args> := <arg_list>
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_ARGS_2);
        scanner.setIndex(ori_index);
        return 0;   // <args> := <empty>
    }
}

int Syntax::arg_list()
{
    #ifdef DEBUG
    printf("----------arg_list\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_ARG_LIST);
    res = expression();
    if(res == 0)
    {
        //callParam();
        res = arg_list_tail();
        if(res == 0)
        {
            return 0;   // <arg_list> := <expression> <arg_list_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::arg_list_tail()
{
    #ifdef DEBUG
    printf("----------arg_list_tail\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_ARG_LIST_TAIL_1);
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string(","))
    {
        res = expression();
        if(res == 0)
        {
            //callParam();
            res = arg_list_tail();
            if(res == 0)
            {
                return 0;   // <arg_list_tail> := , <expression> <arg_list_tail>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_ARG_LIST_TAIL_2);
                scanner.setIndex(ori_index);
                return 0;   // <arg_list_tail> := <empty>
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_ARG_LIST_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // <arg_list_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_ARG_LIST_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // <arg_list_tail> := <empty>
    }
}

int Syntax::input_stmt()
{
    #ifdef DEBUG
    printf("----------input\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_INPUT_STMT);
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("input"))
    {
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("("))
        {
            res = var();
            if(res == 0)
            {
                //exIn();
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string(")"))
                {
                    temp = scanner.next();
                    if(temp.isDelimiter()&&temp.name == std::string(";"))
                    {
                        return 0;   // <input_stmt> := input ( <var> )
                    }
                    else
                    {
                        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                        if(scanner.curIndex - 1 > errPos)
                        {
                            errPos = scanner.curIndex - 1;
                            errMessage = "Error! Need ; before ";
                        }
                        scanner.setIndex(ori_index);
                        return -1;
                    }
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "Error! Need ) before ";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need ( before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need input before ";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::output_stmt()
{
    #ifdef DEBUG
    printf("----------output\n");
    #endif
    int res, ori_index, ori_process;
    ori_index = scanner.curIndex;
    ori_process = syntaxProcess.size();
    syntaxProcess.push_back(PROCESS_OUTPUT_STMT);
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("output"))
    {
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("("))
        {
            res = var();
            if(res == 0)
            {
                //exOut();
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string(")"))
                {
                    temp = scanner.next();
                    if(temp.isDelimiter()&&temp.name == std::string(";"))
                    {
                        return 0;   // <output_stmt> := output ( <var> ) ;
                    }
                    else
                    {
                        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                        if(scanner.curIndex - 1 > errPos)
                        {
                            errPos = scanner.curIndex - 1;
                            errMessage = "Error! Need ; before ";
                        }
                        scanner.setIndex(ori_index);
                        return -1;
                    }
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "Error! Need ) before ";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "Error! Need ( before ";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "Error! Need output before ";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}