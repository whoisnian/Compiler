#include "codeeditor.h"

LineNumberArea::LineNumberArea(CodeEditor *parent) : QWidget (parent)
{
    this->codeEditor = parent;
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    this->codeEditor->lineNumberAreaPaintEvent(event);
}

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    this->lineNumberArea = new LineNumberArea(this);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    this->setStyleSheet("CodeEditor{border: none}");
    this->updateLineNumberAreaWidth(0);
    this->highlightCurrentLine();
    this->setCursorWidth(2);
    this->document()->setDocumentMargin(0);
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int maxLineNumber = qMax(10, this->blockCount());
    while(maxLineNumber >= 10)
    {
        maxLineNumber /= 10;
        digits++;
    }
    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(this->lineNumberArea);
    painter.fillRect(event->rect(), QColor("#404244"));
    QFont font = painter.font();

    QTextBlock block = this->firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = int(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + int(blockBoundingRect(block).height());

    while(block.isValid()&&top <= event->rect().bottom())
    {
        if(block.isVisible()&&bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            if(blockNumber == this->textCursor().block().blockNumber())
            {
                font.setBold(true);
                painter.setFont(font);
                painter.setPen((QColor("#d6c540")));
            }
            else
            {
                font.setBold(false);
                painter.setFont(font);
                painter.setPen(QColor("#bec0c2"));
            }
            painter.drawText(0, top, lineNumberArea->width()-4, fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + int(blockBoundingRect(block).height());
        blockNumber++;
    }
}

void CodeEditor::highlightErrorLine(int lineNumber)
{
    int curLineNumber = this->textCursor().blockNumber();
    QTextCursor tempCursor = this->textCursor();
    if(lineNumber <= 0) return;
    if(lineNumber < curLineNumber+1)
        tempCursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor, curLineNumber+1 - lineNumber);
    else
        tempCursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, lineNumber - curLineNumber-1);
    this->setTextCursor(tempCursor);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    this->lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), this->lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        for(auto it = urls.begin();!urls.empty()&&it != urls.end();it++)
        {
            QUrl url = *it;
            QString fileName = url.toLocalFile();
            if(!fileName.isEmpty())
            {
                QFile file(fileName);
                if(file.open(QFile::ReadOnly|QFile::Text))
                {
                    this->curFile = fileName;
                    this->setPlainText(file.readAll());
                    this->document()->setModified(false);
                    static_cast<QWidget*>(this->parent())->setWindowModified(false);
                    return;
                }
            }
        }
    }
    event->accept();
}

void CodeEditor::updateLineNumberAreaWidth(int)
{
    this->setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(QRect rect, int dy)
{
    if(dy)
        this->lineNumberArea->scroll(0, dy);
    else
        this->lineNumberArea->update(0, rect.y(), this->lineNumberArea->width(), rect.height());

    if(rect.contains(viewport()->rect()))
        this->updateLineNumberAreaWidth(0);
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor("#2e2f30");

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    this->setExtraSelections(extraSelections);
}
