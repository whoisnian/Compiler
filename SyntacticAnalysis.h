#ifndef SYNTACTICANALYSIS_H_INCLUDED
#define SYNTACTICANALYSIS_H_INCLUDED

#include "LexicalAnalysis.h"
#include "paramtable.h"

// 推导过程标记
const int PROCESS_PROGRAM                   = 1;
const int PROCESS_DECLARATION_LIST          = 2;
const int PROCESS_DECLARATION_LIST_TAIL_1   = 3;
const int PROCESS_DECLARATION_LIST_TAIL_2   = 4;
const int PROCESS_DECLARATION_1             = 5;
const int PROCESS_DECLARATION_2             = 6;
const int PROCESS_VAR_DECLARATION_1         = 7;
const int PROCESS_VAR_DECLARATION_2         = 8;
const int PROCESS_TYPE_SPECFIER_1           = 9;
const int PROCESS_TYPE_SPECFIER_2           = 10;
const int PROCESS_FUN_DECLARATION           = 11;
const int PROCESS_PARAMS_1                  = 12;
const int PROCESS_PARAMS_2                  = 13;
const int PROCESS_PARAM_LIST                = 14;
const int PROCESS_PARAM_LIST_TAIL_1         = 15;
const int PROCESS_PARAM_LIST_TAIL_2         = 16;
const int PROCESS_PARAM_1                   = 17;
const int PROCESS_PARAM_2                   = 18;
const int PROCESS_COMPOUND_STMT             = 19;
const int PROCESS_LOCAL_DECLARATIONS_1      = 20;
const int PROCESS_LOCAL_DECLARATIONS_2      = 21;
const int PROCESS_LOCAL_DECLARATIONS_TAIL_1 = 22;
const int PROCESS_LOCAL_DECLARATIONS_TAIL_2 = 23;
const int PROCESS_STATEMENT_LIST_1          = 24;
const int PROCESS_STATEMENT_LIST_2          = 25;
const int PROCESS_STATEMENT_LIST_TAIL_1     = 26;
const int PROCESS_STATEMENT_LIST_TAIL_2     = 27;
const int PROCESS_STATEMENT_1               = 28;
const int PROCESS_STATEMENT_2               = 29;
const int PROCESS_STATEMENT_3               = 30;
const int PROCESS_STATEMENT_4               = 31;
const int PROCESS_STATEMENT_5               = 32;
const int PROCESS_STATEMENT_6               = 33;
const int PROCESS_STATEMENT_7               = 34;
const int PROCESS_EXPRESSION_STMT_1         = 35;
const int PROCESS_EXPRESSION_STMT_2         = 36;
const int PROCESS_SELECTION_STMT_1          = 37;
const int PROCESS_SELECTION_STMT_2          = 38;
const int PROCESS_ITERATION_STMT            = 39;
const int PROCESS_RETURN_STMT_1             = 40;
const int PROCESS_RETURN_STMT_2             = 41;
const int PROCESS_EXPRESSION_1              = 42;
const int PROCESS_EXPRESSION_2              = 43;
const int PROCESS_VAR_1                     = 44;
const int PROCESS_VAR_2                     = 45;
const int PROCESS_SIMPLE_EXPRESSION_1       = 46;
const int PROCESS_SIMPLE_EXPRESSION_2       = 47;
const int PROCESS_RELOP_1                   = 48;
const int PROCESS_RELOP_2                   = 49;
const int PROCESS_RELOP_3                   = 50;
const int PROCESS_RELOP_4                   = 51;
const int PROCESS_RELOP_5                   = 52;
const int PROCESS_RELOP_6                   = 53;
const int PROCESS_ADDITIVE_EXPRESSION       = 54;
const int PROCESS_ADDITIVE_EXPRESSION_TAIL_1= 55;
const int PROCESS_ADDITIVE_EXPRESSION_TAIL_2= 56;
const int PROCESS_ADDOP_1                   = 57;
const int PROCESS_ADDOP_2                   = 58;
const int PROCESS_TERM                      = 59;
const int PROCESS_TERM_TAIL_1               = 60;
const int PROCESS_TERM_TAIL_2               = 61;
const int PROCESS_MULOP_1                   = 62;
const int PROCESS_MULOP_2                   = 63;
const int PROCESS_MULOP_3                   = 64;
const int PROCESS_FACTOR_1                  = 65;
const int PROCESS_FACTOR_2                  = 66;
const int PROCESS_FACTOR_3                  = 67;
const int PROCESS_FACTOR_4                  = 68;
const int PROCESS_CALL                      = 69;
const int PROCESS_ARGS_1                    = 70;
const int PROCESS_ARGS_2                    = 71;
const int PROCESS_ARG_LIST                  = 72;
const int PROCESS_ARG_LIST_TAIL_1           = 73;
const int PROCESS_ARG_LIST_TAIL_2           = 74;
const int PROCESS_INPUT_STMT                = 75;
const int PROCESS_OUTPUT_STMT               = 76;

class Syntax : public ParamTable
{
    public:
        Scan scanner;                   // 词法扫描器
        int errPos;                     // 错误位置
        std::string errMessage;         // 错误原因
        std::vector<int> syntaxProcess; // 推导过程
        int processIndex;               // 推导过程标记

        Syntax();
        Syntax(Scan scanner);

        // 重复遍历一遍推导过程
        void prepare_for_4elem();
        
        // 递归下降中的非终结符对应函数
        int program();
        int declaration_list();
        int declaration_list_tail();
        int declaration();
        int var_declaration();
        int fun_declaration();
        int type_specfier();
        int params();
        int param_list();
        int param_list_tail();
        int param();
        int compound_stmt();
        int local_declarations();
        int local_declarations_tail();
        int statement_list();
        int statement_list_tail();
        int statement();
        int expression_stmt();
        int selection_stmt();
        int iteration_stmt();
        int return_stmt();
        int expression();
        int var();
        int simple_expression();
        int additive_expression();
        int additive_expression_tail();
        int relop();
        int term();
        int term_tail();
        int addop();
        int factor();
        int mulop();
        int call();
        int args();
        int arg_list();
        int arg_list_tail();
        int input_stmt();
        int output_stmt();
};

#endif // SYNTACTICANALYSIS_H_INCLUDED
