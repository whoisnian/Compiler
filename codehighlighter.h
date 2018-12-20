#ifndef CODEHIGHLIGHTER_H
#define CODEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextBlockFormat>
#include <QMessageBox>
#include "LexicalAnalysis.h"

class CodeHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    CodeHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

public slots:
    void closeHighlighter(int, int, int);

private:
    bool enable;        // 开启状态
    int startline;      // 多行注释开始行
    int leftlen;        // 多行注释剩余长度
    Scan *scanner;

    QTextCharFormat highlighterFormat[10];
    std::string tokenTypeName[10];
};

#endif // CODEHIGHLIGHTER_H
