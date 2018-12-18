#include "codehighlighter.h"
#include <QDebug>
#include <QToolTip>

CodeHighlighter::CodeHighlighter(QTextDocument *parent) : QSyntaxHighlighter (parent)
{
    enable = false;
    leftlen = 0;
    startline = -1;
    highlighterFormat[TOKEN_TYPE_UNKNOWN].setForeground(QColor("#ffffff"));
    highlighterFormat[TOKEN_TYPE_KEYWORD].setForeground(QColor("#45c6d6"));
    highlighterFormat[TOKEN_TYPE_IDENTIFIER].setForeground(QColor("#d6cf9a"));
    highlighterFormat[TOKEN_TYPE_DELIMITER].setForeground(QColor("#c0c0c0"));
    highlighterFormat[TOKEN_TYPE_INT].setForeground(QColor("#8a602c"));
    highlighterFormat[TOKEN_TYPE_FLOAT].setForeground(QColor("#8a602c"));
    highlighterFormat[TOKEN_TYPE_CHAR].setForeground(QColor("#9aa7d6"));
    highlighterFormat[TOKEN_TYPE_STR].setForeground(QColor("#d69545"));
    highlighterFormat[TOKEN_TYPE_COMMENT].setForeground(QColor("#808080"));

    highlighterFormat[TOKEN_TYPE_UNKNOWN].setToolTip("TOKEN_TYPE_UNKNOWN");
    highlighterFormat[TOKEN_TYPE_KEYWORD].setToolTip("TOKEN_TYPE_KEYWORD");
    highlighterFormat[TOKEN_TYPE_IDENTIFIER].setToolTip("TOKEN_TYPE_IDENTIFIER");
    highlighterFormat[TOKEN_TYPE_DELIMITER].setToolTip("TOKEN_TYPE_DELIMITER");
    highlighterFormat[TOKEN_TYPE_INT].setToolTip("TOKEN_TYPE_INT");
    highlighterFormat[TOKEN_TYPE_FLOAT].setToolTip("TOKEN_TYPE_FLOAT");
    highlighterFormat[TOKEN_TYPE_CHAR].setToolTip("TOKEN_TYPE_CHAR");
    highlighterFormat[TOKEN_TYPE_STR].setToolTip("TOKEN_TYPE_STR");
    highlighterFormat[TOKEN_TYPE_COMMENT].setToolTip("TOKEN_TYPE_COMMENT");

    scanner = new Scan;
    scanner->tokens.clear();
    scanner->initFrom(this->document()->toPlainText().toStdString());
    if(scanner->errPos != -1)
    {
        closeHighlighter(0, 0, 0);
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle("语法分析遇到问题");
        if(scanner->errPos == 0)
            msg.setText(std::string("错误原因\n\n"+scanner->errMessage).c_str());
        else
            msg.setText(std::string("第"+std::to_string(scanner->errLine+1)+"行\n\n"+scanner->errMessage).c_str());
        msg.addButton(QMessageBox::Yes);
        msg.exec();

        return;
    }
    enable = true;

    connect(this->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(closeHighlighter(int, int, int)));
}

void CodeHighlighter::closeHighlighter(int, int charsRemoved, int charsAdded)
{
    if((charsRemoved != 0||charsAdded != 0)&&enable)
    {
        enable = false;
        this->rehighlight();
    }
}

void CodeHighlighter::highlightBlock(const QString &text)
{
    if(!enable)
    {
        setFormat(0, text.size(), highlighterFormat[TOKEN_TYPE_UNKNOWN]);
        return;
    }
    for(int i = 0;i < (int)scanner->tokens.size()&&scanner->tokens.at(i).lineNumber <= currentBlock().blockNumber();i++)
    {
        if((leftlen <= 0||scanner->tokens.at(i).lineNumber != startline)&&scanner->tokens.at(i).lineNumber < currentBlock().blockNumber()) continue;
        int stpos, len;
        if(scanner->tokens.at(i).startPos >= 0&&text.toStdString().size() >= scanner->tokens.at(i).startPos + scanner->tokens.at(i).length)
        {
            stpos = QString(text.toStdString().substr(0, scanner->tokens.at(i).startPos).c_str()).size();
            len = QString(text.toStdString().substr(scanner->tokens.at(i).startPos, scanner->tokens.at(i).length).c_str()).size();
        }
        else if(scanner->tokens.at(i).lineNumber == currentBlock().blockNumber())
        {
            startline = currentBlock().blockNumber();
            leftlen = scanner->tokens.at(i).length;
            stpos = text.indexOf("/*");
            len = text.size() - stpos;
            leftlen -= text.right(len).toStdString().size();
        }
        else
        {
            stpos = 0;
            len = text.size();
            if(leftlen > len)
                leftlen -= text.right(len).toStdString().size();
            else
            {
                len = QString(text.toStdString().substr(0, leftlen).c_str()).size();
                leftlen = 0;
                startline = -1;
            }
        }

        setFormat(stpos, len, highlighterFormat[scanner->tokens.at(i).type]);
    }
}
