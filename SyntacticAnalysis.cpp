#include <string>
#include "SyntacticAnalysis.h"

Syntax::Syntax()
{
    errPos = -1;
    errMessage = "";
    syntaxProcess.clear();
    processIndex = 0;
}

Syntax::Syntax(Scan scanner)
{
    errPos = -1;
    errMessage = "";
    syntaxProcess.clear();
    processIndex = 0;
    this->scanner = scanner;
}

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
                prepare_for_4elem();
                alGeq(op.name);
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
        return 0;   // 1 <program> := <declaration_list>
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
            return 0;   // 2 <declaration_list> := <declaration> <declaration_list_tail>
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
            return 0;   // 3 <declaration_list_tail> := <declaration> <deliaration_list_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_DECLARATION_LIST_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // 4 <declaration_list_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_DECLARATION_LIST_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // 4 <declaration_list_tail> := <empty>
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
        return 0;   // 5 <declaration> := <var_declaration>
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_DECLARATION_2);
        scanner.setIndex(ori_index);
        res = fun_declaration();
        if(res == 0)
        {
            return 0;   // 6 <declaration> := <fun_declaration>
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
                return 0;   // 7 <var_declaration> := <type_specifier> ID ;
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
                            return 0;   // 8 <var_declaration> := <type_specifier> ID [ NUM ] ;
                        }
                        else
                        {
                            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                            if(scanner.curIndex - 1 > errPos)
                            {
                                errPos = scanner.curIndex - 1;
                                errMessage = "need ; before";
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
                            errMessage = "need ] before";
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
                        errMessage = "need NUM before";
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
                    errMessage = "need ; or [ before";
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
                errMessage = "need ID before";
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
                            return 0;   // 11 <fun_declaration> := <type_specifier> ID ( <params> ) <compound_stmt>
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
                            errMessage = "need ) before";
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
                    errMessage = "need ( before";
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
                errMessage = "need ID before";
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
        return 0;   // 9 <type_specifier> := int
    }
    else if(temp.isKeyWord()&&temp.name == std::string("void"))
    {
        syntaxProcess.push_back(PROCESS_TYPE_SPECFIER_2);
        return 0;   // 10 <type_specifier> := void
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
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
        return 0;   // 12 <params> := <param_list>
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_PARAMS_2);
        scanner.setIndex(ori_index);
        Token temp = scanner.next();
        if(temp.isKeyWord()&&temp.name == std::string("void"))
        {
            return 0;   // 13 <params> := void
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need void before";
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
            return 0;   // 14 <param_list> := <parm> <parm_list_tail>
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
                return 0;   // 15 <parm_list_tail> := , <param> <parm_list_tail>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_PARAM_LIST_TAIL_2);
                scanner.setIndex(ori_index);
                return 0;   // 16 <parm_list_tail> := <empty>
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_PARAM_LIST_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // 16 <parm_list_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_PARAM_LIST_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // 16 <parm_list_tail> := <empty>
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
                    return 0;   // 18 <param> := <type_specifier> ID [ ]
                }
                else
                {
                    //addNum(pretemp.identifierAndIntPos,true);
                    syntaxProcess.at(ori_process) = PROCESS_PARAM_1;
                    scanner.back();
                    scanner.back();
                    return 0;   // 17 <param> := <type_specifier> ID
                }
            }
            else
            {
                //addNum(pretemp.identifierAndIntPos,true);
                syntaxProcess.at(ori_process) = PROCESS_PARAM_1;
                scanner.back();
                return 0;   // 17 <param> := <type_specifier> ID
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need ID before";
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
                    return 0;   // 19 <compound_stmt> := { <local_declarations> <statement_list> }
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "need } before";
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
            errMessage = "need { before";
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
            return 0;   // 20 <local_declarations> := <var_declaration> <local_declarations_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_2);
            scanner.setIndex(ori_index);
            return 0;   // 21 <local_declarations> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_2);
        scanner.setIndex(ori_index);
        return 0;   // 21 <local_declarations> := <empty>
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
            return 0;   // 22 <local_declarations_tail> := <var_declaration> <local_declarations_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // 23 <local_declarations_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_LOCAL_DECLARATIONS_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // 23 <local_declarations_tail> := <empty>
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
            return 0;   // 24 <statement_list> := <statement> <statement_list_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_STATEMENT_LIST_2);
            scanner.setIndex(ori_index);
            return 0;   // 25 <statement_list> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_STATEMENT_LIST_2);
        scanner.setIndex(ori_index);
        return 0;   // 25 <statement_list> := <empty>
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
            return 0;   // 26 <statement_list_tail> := <statement> <statement_list_tail>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_STATEMENT_LIST_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // 27 <statement_list_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_STATEMENT_LIST_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // 27 <statement_list_tail> := <empty>
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
        return 0;   // 28 <statement> := <expression_stmt>
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_STATEMENT_2);
        scanner.setIndex(ori_index);
        res = compound_stmt();
        if(res == 0)
        {
            return 0;   // 29 <statement> := <compound_stmt>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_STATEMENT_3);
            scanner.setIndex(ori_index);
            res = selection_stmt();
            if(res == 0)
            {
                return 0;   // 30 <statement> := <selection_stmt>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_STATEMENT_4);
                scanner.setIndex(ori_index);
                res = iteration_stmt();
                if(res == 0)
                {
                    return 0;   // 31 <statement> := <iteration_stmt>
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    syntaxProcess.push_back(PROCESS_STATEMENT_5);
                    scanner.setIndex(ori_index);
                    res = return_stmt();
                    if(res == 0)
                    {
                        return 0;   // 32 <statement> := <return_stmt>
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
            return 0;   // 33 <expression_stmt> := <expression> ;
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_EXPRESSION_STMT_2);
            scanner.setIndex(ori_index);
            Token temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string(";"))
            {
                return 0;   // 34 <expression_stmt> := ;
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "need ; before";
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
            return 0;   // 34 <expression_stmt> := ;
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need ; before";
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
                                return 0;   // 36 <selection_stmt> := if ( <expression> ) <statement> else <statement>
                            }
                            else
                            {
                                //exIe();
                                while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
                                syntaxProcess.at(ori_process) = PROCESS_SELECTION_STMT_1;
                                scanner.setIndex(another_index);
                                return 0;   // 35 <selection_stmt> := if ( <expression> ) <statement>
                            }
                        }
                        else
                        {
                            //exIe();
                            syntaxProcess.at(ori_process) = PROCESS_SELECTION_STMT_1;
                            scanner.back();
                            return 0;   // 35 <selection_stmt> := if ( <expression> ) <statement>
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
                        errMessage = "need ) before";
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
                errMessage = "need ( before";
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
            errMessage = "need if before";
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
                        return 0;   // 37 <iteration_stmt> := while ( <expression> ) <statement>
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
                        errMessage = "need ) before";
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
                errMessage = "need ( before";
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
            errMessage = "need while before";
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
            return 0;   // 38 <return_stmt> := return ;
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
                    return 0;   // 39 <return_stmt> := return <expression> ;
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "need ; before";
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
            errMessage = "need return before";
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
                return 0;   // 40 <expression> := <var> = <expression>
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
                    return 0;   // 43 <var> := ID [ <expression> ]
                }
                else
                {
                    //alPush(pretemp.identifierAndIntPos);
                    while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
                    syntaxProcess.at(ori_process) = PROCESS_VAR_1;
                    scanner.setIndex(another_index);
                    return 0;   // 42 <var> := ID
                }
            }
            else
            {
                //alPush(pretemp.identifierAndIntPos);
                while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
                syntaxProcess.at(ori_process) = PROCESS_VAR_1;
                scanner.setIndex(another_index);
                return 0;   // 42 <var> := ID
            }
        }
        else
        {
            //alPush(pretemp.identifierAndIntPos);
            syntaxProcess.at(ori_process) = PROCESS_VAR_1;
            scanner.back();
            return 0;   // 42 <var> := ID
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need ID before";
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
                return 0;   // 44 <simple_expression> := <additive_expression> <relop> <additive_expression>
            }
            else
            {
                while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
                syntaxProcess.at(ori_process) = PROCESS_SIMPLE_EXPRESSION_2;
                scanner.setIndex(another_index);
                return 0;   // 45 <simple_expression> := <additive_expression>
            }
        }
        else
        {
            while((int)syntaxProcess.size() > another_process) syntaxProcess.pop_back();
            syntaxProcess.at(ori_process) = PROCESS_SIMPLE_EXPRESSION_2;
            scanner.setIndex(another_index);
            return 0;   // 45 <simple_expression> := <additive_expression>
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
            return 0;   // 52 <additive_expression> := <term> <additive_expression_tail>
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
            res = additive_expression_tail();
            if(res == 0)
            {
                //alGeq(temp.name);
                return 0;   // 53 <additive_expression_tail> := <addop> <term> <additive_expression_tail>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_ADDITIVE_EXPRESSION_TAIL_2);
                scanner.setIndex(ori_index);
                return 0;   // 54 <additive_expression_tail> := <empty>
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_ADDITIVE_EXPRESSION_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // 54 <additive_expression_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_ADDITIVE_EXPRESSION_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // 54 <additive_expression_tail> := <empty>
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
        return 0;   // 46 <relop> := <=
    }
    else if(temp.isDelimiter()&&temp.name == std::string("<"))
    {
        syntaxProcess.push_back(PROCESS_RELOP_2);
        return 0;   // 47 <relop> := <
    }
    else if(temp.isDelimiter()&&temp.name == std::string(">"))
    {
        syntaxProcess.push_back(PROCESS_RELOP_3);
        return 0;   // 48 <relop> := >
    }
    else if(temp.isDelimiter()&&temp.name == std::string(">="))
    {
        syntaxProcess.push_back(PROCESS_RELOP_4);
        return 0;   // 49 <relop> := >=
    }
    else if(temp.isDelimiter()&&temp.name == std::string("=="))
    {
        syntaxProcess.push_back(PROCESS_RELOP_5);
        return 0;   // 50 <relop> := ==
    }
    else if(temp.isDelimiter()&&temp.name == std::string("!="))
    {
        syntaxProcess.push_back(PROCESS_RELOP_6);
        return 0;   // 51 <relop> := !=
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need <=, <, >, >=, == or != before";
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
            return 0;   // 57 <term> := <factor> <term_tail>
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
            return 0;   // 58 <term_tail> := <mulop> <factor>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_TERM_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // 59 <term_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_TERM_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // 59 <term_tail> := <empty>
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
        return 0;   // 55 <addop> := +
    }
    else if(temp.isDelimiter()&&temp.name == std::string("-"))
    {
        syntaxProcess.push_back(PROCESS_ADDOP_2);
        return 0;   // 56 <addop> := -
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need + or - before";
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
                return 0;   // 62 <factor> := ( <expression> )
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "need ) before";
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
        return 0;   // 65 <factor> := NUM
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_FACTOR_3);
        scanner.setIndex(ori_index);
        res = call();
        if(res == 0)
        {
            return 0;   // 64 <factor> := <call>
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_FACTOR_2);
            scanner.setIndex(ori_index);
            res = var();
            if(res == 0)
            {
                return 0;   // 63 <factor> := <var>
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
        return 0;   // 60 <mulop> := *
    }
    else if(temp.isDelimiter()&&temp.name == std::string("/"))
    {
        syntaxProcess.push_back(PROCESS_MULOP_2);
        return 0;   // 61 <mulop> := /
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need * or / before";
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
                    return 0;   // 66 <call> := ID ( <args> )
                }
                else
                {
                    while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "need ) before";
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
                errMessage = "need ( before";
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
            errMessage = "need ID before";
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
        return 0;   // 67 <args> := <arg_list>
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_ARGS_2);
        scanner.setIndex(ori_index);
        return 0;   // 68 <args> := <empty>
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
            return 0;   // 69 <arg_list> := <expression> <arg_list_tail>
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
                return 0;   // 70 <arg_list_tail> := , <expression> <arg_list_tail>
            }
            else
            {
                while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
                syntaxProcess.push_back(PROCESS_ARG_LIST_TAIL_2);
                scanner.setIndex(ori_index);
                return 0;   // 71 <arg_list_tail> := <empty>
            }
        }
        else
        {
            while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
            syntaxProcess.push_back(PROCESS_ARG_LIST_TAIL_2);
            scanner.setIndex(ori_index);
            return 0;   // 71 <arg_list_tail> := <empty>
        }
    }
    else
    {
        while((int)syntaxProcess.size() > ori_process) syntaxProcess.pop_back();
        syntaxProcess.push_back(PROCESS_ARG_LIST_TAIL_2);
        scanner.setIndex(ori_index);
        return 0;   // 71 <arg_list_tail> := <empty>
    }
}
