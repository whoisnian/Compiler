#ifndef LEXICALANALYSIS_H_INCLUDED
#define LEXICALANALYSIS_H_INCLUDED

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

// token 类型
const int TOKEN_TYPE_EOF            = -1;   // Token 末尾
const int TOKEN_TYPE_UNKNOWN        = 0;    // 未知
const int TOKEN_TYPE_KEYWORD        = 1;    // 关键字
const int TOKEN_TYPE_IDENTIFIER     = 2;    // 标识符
const int TOKEN_TYPE_DELIMITER      = 3;    // 界符
const int TOKEN_TYPE_INT            = 4;    // 整型
const int TOKEN_TYPE_FLOAT          = 5;    // 浮点
const int TOKEN_TYPE_CHAR           = 6;    // 字符
const int TOKEN_TYPE_STR            = 7;    // 字符串
const int TOKEN_TYPE_COMMENT        = 8;    // 注释

// 自动机状态
const int STATE_BEGIN               = 1;
const int STATE_WORD                = 2;
const int STATE_DELIMITER_L         = 3;
const int STATE_DELIMITER_R         = 4;
const int STATE_COMMENT_MAYBE       = 5;
const int STATE_COMMENT_S           = 6;
const int STATE_COMMENT_S_END_MAYBE = 7;
const int STATE_COMMENT_S_END       = 8;
const int STATE_COMMENT             = 9;
const int STATE_COMMENT_END         = 10;
const int STATE_INT                 = 11;
const int STATE_FLOAT               = 12;
const int STATE_CHAR_L              = 13;
const int STATE_CHAR                = 14;
const int STATE_CHAR_R              = 15;
const int STATE_STR_L               = 16;
const int STATE_STR                 = 17;
const int STATE_STR_R               = 18;
const int STATE_END                 = 0;
const int STATE_ERROR               = -1;

class Token
{
    public:
        int type;               // token 类型： 关键字 标识符 界符 整型 浮点 字符 字符串
        std::string name;       // token 内容： int    qwe    (    123  0.1  a    abc
        int pos;                // 对应表中的位置
        int identifierAndIntPos;// 标识符和常数大表中的位置（只对于标识符和常数来说）
        int valueInt = 0;       // 当类型为 TOKEN_TYPE_INT 时 token 的值
        float valueFloat = 0;   // 当类型为 TOKEN_TYPE_FLOAT 时 token 的值

        // 用于语法高亮以及报错时找到对应行
        int startPos;           // 所在行开始位置
        int length;             // Token 长度
        int lineNumber;         // 所处行号

        Token(){};
        Token(int type, std::string name, int pos, int lineNumber = 0, int startPos = 0, int length = 0)
        {
            this->type = type;
            this->pos = pos;
            this->lineNumber = lineNumber;
            this->startPos = startPos;
            this->length = length;
            if(type == TOKEN_TYPE_CHAR||type == TOKEN_TYPE_STR)
                this->name = name.substr(1, name.size()-2);
            else
                this->name = name;

            if(type == TOKEN_TYPE_INT)
                sscanf(name.c_str(), "%d", &valueInt);
            else if(type == TOKEN_TYPE_FLOAT)
                sscanf(name.c_str(), "%f", &valueFloat);
        }
        Token(const Token& oldToken){
            type=oldToken.type;
            name=oldToken.name;
            pos=oldToken.pos;
            lineNumber = oldToken.lineNumber;
            startPos = oldToken.startPos;
            length = oldToken.length;
            identifierAndIntPos=oldToken.identifierAndIntPos;
            valueInt=oldToken.valueInt;
            valueFloat=oldToken.valueFloat;
        }
        bool isEOF()
        {
            if(this->type == TOKEN_TYPE_EOF)
                return true;
            return false;
        }
        bool isKeyWord()
        {
            if(this->type == TOKEN_TYPE_KEYWORD)
                return true;
            return false;
        }
        bool isIdentifier()
        {
            if(this->type == TOKEN_TYPE_IDENTIFIER)
                return true;
            return false;
        }
        bool isDelimiter()
        {
            if(this->type == TOKEN_TYPE_DELIMITER)
                return true;
            return false;
        }
        bool isInt()
        {
            if(this->type == TOKEN_TYPE_INT)
                return true;
            return false;
        }
        bool isFloat()
        {
            if(this->type == TOKEN_TYPE_FLOAT)
                return true;
            return false;
        }
};

class Scan
{
    public:
        int numberOfKeyWord;                        // 关键字个数
        int numberOfDelimiter;                      // 界符个数
        std::string keyWordTable[30];               // 关键字表
        std::string delimiterTable[30];             // 界符表
        std::vector<std::string> identifierTable;   // 标识符表
        std::vector<std::string> intTable;          // 整型常量表
        std::vector<std::string> floatTable;        // 浮点常量表
        std::vector<std::string> charTable;         // 字符常量表
        std::vector<std::string> strTable;          // 字符串常量表
        std::vector<std::string> identifierAndIntTable; // 标识符和常数大表
        std::vector<Token> tokens;                  // 词法分析完成后得到的 token 表
        int errPos;                                 // 错误位置，-1 表示无错误，0 表示其它错误
        int errLine;                                // 错误行号
        std::string errMessage;                     // 错误原因
        int line;                                   // 当前行
        int oldLine;                                // 上一行
        int linePos;                                // 当前行位置
        int oldLinePos;                             // 上一行位置

        Scan();
        Scan(std::string filename);                 // 从文件构造
        void initFrom(std::string plainText);       // 从字符串构造
        void initTable();                           // 初始化各表，构造函数调用，不需要手动调用
        
        int stateChange(int stateBefore, char ch);      // 转移函数
        void parseState(int stateBefore, char *token);  // 解析读取的 token，填入 token 表
        int curIndex;                                   // 下个 Token 位置
        void setIndex(int index);                       // 设置下个 Token 位置
        Token next();                                   // 语法分析调用接口，返回一个 Token
        void back();                                    // 语法分析调用接口，回退一个 Token
};

#endif // LEXICALANALYSIS_H_INCLUDED
