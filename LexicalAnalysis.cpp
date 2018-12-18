#include "LexicalAnalysis.h"

Scan::Scan()
{
    initTable();
}

Scan::Scan(std::string filename)
{
    initTable();
    FILE *fp = fopen(filename.c_str(), "r");
    if(fp == NULL)
    {
        errMessage = "Error! Can't open file: " + filename;
        errPos = 0;
        return;
    }
    char ch, token[5000];
    int stateCur, stateBefore, tokenLen;
    tokenLen = 0;
    stateCur = STATE_BEGIN;
    while((ch = getc(fp)) != EOF)
    {
        linePos++;
        if(stateCur != STATE_COMMENT_S&&stateCur != STATE_COMMENT_S_END_MAYBE&&stateCur != STATE_COMMENT_S_END)
            oldLine = line;
        if(ch == '\n')
        {
            line++;
            oldLinePos = linePos;
            linePos = 0;
        }
        else
        {
            oldLinePos = linePos;
        }

        stateBefore = stateCur;
        stateCur = stateChange(stateBefore, ch);
        if(stateCur > 0)
        {
            if(stateCur == STATE_BEGIN&&(ch == '\r'||ch == '\n'||ch == ' ')) continue;
            token[tokenLen++] = ch;
        }
        else if(stateCur < 0)
        {
            token[tokenLen] = '\0';
            errMessage = "Error! Failed at " + std::string(token);
            errPos = ftell(fp);
            errLine = oldLine;
            fclose(fp);
            return;
        }
        else
        {
            token[tokenLen] = '\0';
            parseState(stateBefore, token);
            stateCur = stateChange(STATE_BEGIN, ch);
            tokenLen = 0;
            if(stateCur == STATE_BEGIN&&(ch == '\r'||ch == '\n'||ch == ' ')) continue;
            token[tokenLen++] = ch;
        }
    }
    fclose(fp);
    // 生成标识符和常数大表 identifierAndIntTable
    identifierAndIntTable = identifierTable;
    identifierAndIntTable.insert(identifierAndIntTable.end(), intTable.begin(), intTable.end());
    for(auto it = tokens.begin();it != tokens.end();it++)
    {
        if(it->isIdentifier())
            it->identifierAndIntPos = it->pos;
        else if(it->isInt())
            it->identifierAndIntPos = it->pos + identifierTable.size();
    }
    return;
}

void Scan::initFrom(std::string plainText)
{
    if(plainText.empty())
    {
        errMessage = "Error! Can't init from empty string";
        errPos = 0;
        return;
    }
    plainText.append("\n");
    char ch, token[5000];
    int stateCur, stateBefore, tokenLen, cnt;
    tokenLen = 0;
    cnt = 0;
    stateCur = STATE_BEGIN;
    while(cnt < (int)plainText.size())
    {
        ch = plainText.at(cnt++);

        linePos++;
        if(stateCur != STATE_COMMENT_S&&stateCur != STATE_COMMENT_S_END_MAYBE&&stateCur != STATE_COMMENT_S_END)
            oldLine = line;
        if(ch == '\n')
        {
            line++;
            oldLinePos = linePos;
            linePos = 0;
        }
        else
        {
            oldLinePos = linePos;
        }

        stateBefore = stateCur;
        stateCur = stateChange(stateBefore, ch);
        if(stateCur > 0)
        {
            if(stateCur == STATE_BEGIN&&(ch == '\r'||ch == '\n'||ch == ' ')) continue;
            token[tokenLen++] = ch;
        }
        else if(stateCur < 0)
        {
            token[tokenLen] = '\0';
            errMessage = "Error! Failed at " + std::string(token);
            errPos = cnt;
            errLine = oldLine;
            return;
        }
        else
        {
            token[tokenLen] = '\0';
            parseState(stateBefore, token);
            stateCur = stateChange(STATE_BEGIN, ch);
            tokenLen = 0;
            if(stateCur == STATE_BEGIN&&(ch == '\r'||ch == '\n'||ch == ' ')) continue;
            token[tokenLen++] = ch;
        }
    }
    // 生成标识符和常数大表 identifierAndIntTable
    identifierAndIntTable = identifierTable;
    identifierAndIntTable.insert(identifierAndIntTable.end(), intTable.begin(), intTable.end());
    for(auto it = tokens.begin();it != tokens.end();it++)
    {
        if(it->isIdentifier())
            it->identifierAndIntPos = it->pos;
        else if(it->isInt())
            it->identifierAndIntPos = it->pos + identifierTable.size();
    }
}

void Scan::initTable()
{
    numberOfKeyWord = 6;
    keyWordTable[0] = std::string("else");
    keyWordTable[1] = std::string("if");
    keyWordTable[2] = std::string("int");
    keyWordTable[3] = std::string("return");
    keyWordTable[4] = std::string("void");
    keyWordTable[5] = std::string("while");

    numberOfDelimiter = 19;
    delimiterTable[0] = std::string("+");
    delimiterTable[1] = std::string("-");
    delimiterTable[2] = std::string("*");
    delimiterTable[3] = std::string("/");
    delimiterTable[4] = std::string("<");
    delimiterTable[5] = std::string("<=");
    delimiterTable[6] = std::string(">");
    delimiterTable[7] = std::string(">=");
    delimiterTable[8] = std::string("==");
    delimiterTable[9] = std::string("!=");
    delimiterTable[10] = std::string("=");
    delimiterTable[11] = std::string(";");
    delimiterTable[12] = std::string(",");
    delimiterTable[13] = std::string("(");
    delimiterTable[14] = std::string(")");
    delimiterTable[15] = std::string("[");
    delimiterTable[16] = std::string("]");
    delimiterTable[17] = std::string("{");
    delimiterTable[18] = std::string("}");

    identifierTable.clear();
    intTable.clear();
    floatTable.clear();
    charTable.clear();
    strTable.clear();
    tokens.clear();

    errMessage = "";
    errPos = -1;
    errLine = -1;
    line = 0;
    linePos = 0;
    curIndex = 0;
}

int Scan::stateChange(int stateBefore, char ch)
{
    switch(stateBefore)
    {
        case STATE_BEGIN:
            {
                if((ch >= 'a'&&ch <= 'z')||(ch >= 'A'&&ch <= 'Z'))
                    return STATE_WORD;
                else if(ch >= '0'&&ch <= '9')
                    return STATE_INT;
                else if(ch == '\r'||ch == '\n'||ch == ' ')
                    return STATE_BEGIN;
                else if(ch == '\'')
                    return STATE_CHAR_L;
                else if(ch == '\"')
                    return STATE_STR_L;
                else if(ch == '<'||ch == '>'||ch == '='||ch == '!')
                    return STATE_DELIMITER_L;
                else if(ch == '+'||ch == '-'||ch == '*')
                    return STATE_DELIMITER_R;
                else if(ch == '/')
                    return STATE_COMMENT_MAYBE;
                else if(ch == '{'||ch == '}'||ch == '('||ch == ')'||ch == '['||ch == ']'||ch == ','||ch == ';')
                    return STATE_DELIMITER_R;
                else
                    return STATE_ERROR;
            }
        case STATE_WORD:
            {
                if((ch >= 'a'&&ch <= 'z')||(ch >= 'A'&&ch <= 'Z'))
                    return STATE_WORD;
                else if(ch >= '0'&&ch <= '9')
                    return STATE_WORD;
                else
                    return STATE_END;
            }
        case STATE_DELIMITER_L:
            {
                if(ch == '=')
                    return STATE_DELIMITER_R;
                else
                    return STATE_END;
            }
        case STATE_DELIMITER_R:
            {
                return STATE_END;
            }
        case STATE_COMMENT_MAYBE:
            {
                if(ch == '*')
                    return STATE_COMMENT_S;
                else if(ch == '/')
                    return STATE_COMMENT;
                else
                    return STATE_END;
            }
        case STATE_COMMENT_S:
            {
                if(ch == '*')
                    return STATE_COMMENT_S_END_MAYBE;
                else
                    return STATE_COMMENT_S;
            }
        case STATE_COMMENT_S_END_MAYBE:
            {
                if(ch == '/')
                    return STATE_COMMENT_S_END;
                if(ch == '*')
                    return STATE_COMMENT_S_END_MAYBE;
                else
                    return STATE_COMMENT_S;
            }
        case STATE_COMMENT_S_END:
            {
                return STATE_END;
            }
        case STATE_COMMENT:
            {
                if(ch == '\n')
                    return STATE_END;
                else
                    return STATE_COMMENT;
            }
        case STATE_COMMENT_END:
            {
                return STATE_END;
            }
        case STATE_INT:
            {
                if(ch >= '0'&&ch <= '9')
                    return STATE_INT;
                else if(ch == '.')
                    return STATE_FLOAT;
                else
                    return STATE_END;
            }
        case STATE_FLOAT:
            {
                if(ch >= '0'&&ch <= '9')
                    return STATE_FLOAT;
                else
                    return STATE_END;
            }
        case STATE_CHAR_L:
            {
                if(ch == '\'')
                    return STATE_ERROR;
                else if(ch >= 32&&ch <= 126)
                    return STATE_CHAR;
                else
                    return STATE_ERROR;
            }
        case STATE_CHAR:
            {
                if(ch == '\'')
                    return STATE_CHAR_R;
                else
                    return STATE_ERROR;
            }
        case STATE_CHAR_R:
            {
                return STATE_END;
            }
        case STATE_STR_L:
            {
                if(ch == '\"')
                    return STATE_STR_R;
                else if(ch >= 32&&ch <= 126)
                    return STATE_STR;
                else
                    return STATE_ERROR;
            }
        case STATE_STR:
            {
                if(ch == '\"')
                    return STATE_STR_R;
                else if(ch >= 32&&ch <= 126)
                    return STATE_STR;
                else
                    return STATE_ERROR;
            }
        case STATE_STR_R:
            {
                return STATE_END;
            }
        default:
            {
                return STATE_ERROR;
            }
    }
}

void Scan::parseState(int stateBefore, char *token)
{
    switch(stateBefore)
    {
        case STATE_WORD:
            {
                for(int i = 0;i < numberOfKeyWord;i++)
                {
                    if(!strcmp(token, keyWordTable[i].c_str()))
                    {
                        tokens.push_back(Token(TOKEN_TYPE_KEYWORD, std::string(token), i, oldLine, oldLinePos-strlen(token)-1, strlen(token)));
                        return;
                    }
                }
                auto it = std::find(identifierTable.begin(), identifierTable.end(), std::string(token));
                if(it == identifierTable.end())
                {
                    tokens.push_back(Token(TOKEN_TYPE_IDENTIFIER, std::string(token), identifierTable.size(), oldLine, oldLinePos-strlen(token)-1, strlen(token)));
                    identifierTable.push_back(std::string(token));
                }
                else
                {
                    tokens.push_back(Token(TOKEN_TYPE_IDENTIFIER, std::string(token), it-identifierTable.begin(), oldLine, oldLinePos-strlen(token)-1, strlen(token)));
                }
                return;
            }
        case STATE_DELIMITER_L: case STATE_DELIMITER_R: case STATE_COMMENT_MAYBE:
            {
                for(int i = 0;i < numberOfDelimiter;i++)
                {
                    if(!strcmp(token, delimiterTable[i].c_str()))
                    {
                        tokens.push_back(Token(TOKEN_TYPE_DELIMITER, std::string(token), i, oldLine, oldLinePos-strlen(token)-1, strlen(token)));
                        return;
                    }
                }
                errMessage = "Unknown Delimiter " + std::string(token);
                //tokens.push_back(Token(TOKEN_TYPE_UNKNOWN, std::string(token), 0));
                return;
            }
        case STATE_INT:
            {
                tokens.push_back(Token(TOKEN_TYPE_INT, std::string(token), intTable.size(), oldLine, oldLinePos-strlen(token)-1, strlen(token)));
                intTable.push_back(std::string(token));
                return;
            }
        case STATE_FLOAT:
            {
                tokens.push_back(Token(TOKEN_TYPE_FLOAT, std::string(token), floatTable.size(), oldLine, oldLinePos-strlen(token)-1, strlen(token)));
                floatTable.push_back(std::string(token));
                return;
            }
        case STATE_CHAR_R:
            {
                tokens.push_back(Token(TOKEN_TYPE_CHAR, std::string(token), charTable.size(), oldLine, oldLinePos-strlen(token)-1, strlen(token)));
                charTable.push_back(std::string(token));
                return;
            }
        case STATE_STR_R:
            {
                tokens.push_back(Token(TOKEN_TYPE_STR, std::string(token), strTable.size(), oldLine, oldLinePos-strlen(token)-1, strlen(token)));
                strTable.push_back(std::string(token));
                return;
            }
        case STATE_COMMENT_S_END: case STATE_COMMENT: case STATE_COMMENT_END:
            {
                tokens.push_back(Token(TOKEN_TYPE_COMMENT, std::string(token), 0, oldLine, oldLinePos-strlen(token)-1, strlen(token)));
                return;
            }
        default:
            {
                return;
            }
    }
}

void Scan::setIndex(int index)
{
    this->curIndex = index;
}

Token Scan::next()
{
    // 忽略注释
    while(curIndex < (int)tokens.size()&&tokens.at(curIndex).type == TOKEN_TYPE_COMMENT)
        curIndex++;

    // 到达 Token 表结尾
    if(curIndex >= (int)tokens.size())
        return Token(TOKEN_TYPE_EOF, std::string("EOF"), 0);
    
    curIndex++;
    #ifdef DEBUG
    printf("-----%s\n", tokens.at(curIndex-1).name.c_str());
    #endif
    return tokens.at(curIndex-1);
}

void Scan::back()
{
    if(curIndex < 1)
        return;

    curIndex--;
    while(curIndex > 0&&tokens.at(curIndex).type == TOKEN_TYPE_COMMENT)
        curIndex--;
}
