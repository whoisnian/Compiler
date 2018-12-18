#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <QPainter>
#include <QMimeData>
#include <QTextBlock>
#include <QDragEnterEvent>
#include <QPlainTextEdit>

class CodeEditor;
class LineNumberArea;

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

private:
    CodeEditor *codeEditor;
};

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    QString curFile;
    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    void highlightErrorLine(int lineNumber);

protected:
    void resizeEvent(QResizeEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

public slots:
    void updateLineNumberAreaWidth(int);
    void updateLineNumberArea(QRect, int);
    void highlightCurrentLine();

private:
    LineNumberArea *lineNumberArea;
};

#endif // CODEEDITOR_H
