#include "codehighlighter.h"

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

    tokenTypeName[TOKEN_TYPE_UNKNOWN]       = "未知类型";
    tokenTypeName[TOKEN_TYPE_KEYWORD]       = "关键字";
    tokenTypeName[TOKEN_TYPE_IDENTIFIER]    = "标识符";
    tokenTypeName[TOKEN_TYPE_DELIMITER]     = "界符";
    tokenTypeName[TOKEN_TYPE_INT]           = "整型";
    tokenTypeName[TOKEN_TYPE_FLOAT]         = "浮点";
    tokenTypeName[TOKEN_TYPE_CHAR]          = "字符";
    tokenTypeName[TOKEN_TYPE_STR]           = "字符串";
    tokenTypeName[TOKEN_TYPE_COMMENT]       = "注释";

    scanner = new Scan;
    scanner->initFrom(this->document()->toPlainText().toStdString());
    if(scanner->errPos != -1)
    {
        closeHighlighter(0, 0, 0);
        QMessageBox msg(static_cast<QWidget*>(this->parent()->parent()->parent()));
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("词法分析遇到问题");
        if(scanner->errPos == 0)
            msg.setText(std::string("错误原因\n\n"+scanner->errMessage).c_str());
        else
            msg.setText(std::string("第"+std::to_string(scanner->errLine+1)+"行\n\n"+scanner->errMessage).c_str());
        msg.addButton(QMessageBox::Ok);
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
        QTextCharFormat tempFormat;
        tempFormat.setForeground(QColor("#ffffff"));
        setFormat(0, text.size(), tempFormat);
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

        QTextCharFormat tempFormat = highlighterFormat[scanner->tokens.at(i).type];
        if(scanner->tokens.at(i).type == TOKEN_TYPE_COMMENT)
            tempFormat.setToolTip(QString(tokenTypeName[scanner->tokens.at(i).type].c_str()));
        else if(scanner->tokens.at(i).isIdentifier()||scanner->tokens.at(i).isInt())
            tempFormat.setToolTip(QString(("<b>" + scanner->tokens.at(i).name + "</b><br/>位置：" + std::to_string(scanner->tokens.at(i).identifierAndIntPos) + "<br/>类型：" + tokenTypeName[scanner->tokens.at(i).type]).c_str()));
        else
            tempFormat.setToolTip(QString(("<b>" + scanner->tokens.at(i).name + "</b><br/>位置：" + std::to_string(scanner->tokens.at(i).pos) + "<br/>类型：" + tokenTypeName[scanner->tokens.at(i).type]).c_str()));
        setFormat(stpos, len, tempFormat);
    }
}
