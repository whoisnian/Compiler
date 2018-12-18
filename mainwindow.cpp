#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    codeEditor = new CodeEditor(this);
    this->setCentralWidget(codeEditor);

    this->setupMenuAndToolBar();
    this->statusBar()->clearMessage();
    connect(codeEditor->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    codeEditor->document()->setModified(false);
    setWindowModified(false);
    setWindowFilePath("untitled.txt");
    this->setWindowIcon(QIcon::fromTheme("run-build"));
    this->resize(800, 594);
}

void MainWindow::showError(int lineNumber, std::string errMessage)
{
    this->statusBar()->showMessage(QString(errMessage.c_str()), 5000);
    this->codeEditor->highlightErrorLine(lineNumber);
}

void MainWindow::about()
{
    QMessageBox::about(this, "About", "<b>Compiler</b>"\
                       "<p>Based on Qt Creator 4.8.0</p>"\
                       "<p>Based on Qt 5.12.0 (GCC 8.2.1 20181127, 64 bit)</p>");
}

void MainWindow::newFile()
{
    this->codeEditor->curFile.clear();
    codeEditor->document()->setModified(false);
    setWindowModified(false);
    setWindowFilePath("untitled.txt");
    this->codeEditor->clear();
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "/");

    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(file.open(QFile::ReadOnly|QFile::Text))
        {
            this->codeEditor->curFile = fileName;
            codeEditor->setPlainText(file.readAll());
            codeEditor->document()->setModified(false);
            setWindowModified(false);
            setWindowFilePath(fileName);
        }
    }
}

void MainWindow::saveFile()
{
    if(this->codeEditor->curFile.isEmpty())
    {
        this->saveAs();
        return;
    }

    QFile file(this->codeEditor->curFile);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        this->statusBar()->showMessage("保存失败", 2000);
        return;
    }

    QTextStream out(&file);
    out << this->codeEditor->toPlainText();

    codeEditor->document()->setModified(false);
    setWindowModified(false);

    statusBar()->showMessage("保存成功", 2000);
    return;
}

void MainWindow::saveAs()
{
    QFileDialog dialog(this);
    if(this->codeEditor->curFile.isEmpty())
        dialog.selectFile("untitled.txt");
    else
        dialog.selectFile(this->codeEditor->curFile);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if(dialog.exec() != QDialog::Accepted)
        return;
    this->codeEditor->curFile = dialog.selectedFiles().first();

    QFile file(this->codeEditor->curFile);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        this->statusBar()->showMessage("保存失败", 2000);
        return;
    }

    QTextStream out(&file);
    out << this->codeEditor->toPlainText();

    codeEditor->document()->setModified(false);
    setWindowModified(false);
    setWindowFilePath(this->codeEditor->curFile);

    statusBar()->showMessage("保存成功", 2000);
    return;
}

void MainWindow::setupMenuAndToolBar()
{
    QMenu *fileMenu = new QMenu("文件", this);
    menuBar()->addMenu(fileMenu);

    QMenu *editMenu = new QMenu("编辑", this);
    menuBar()->addMenu(editMenu);

    QMenu *compileMenu = new QMenu("编译", this);
    menuBar()->addMenu(compileMenu);

    QMenu *helpMenu = new QMenu("帮助", this);
    menuBar()->addMenu(helpMenu);

    QToolBar *toolBar = this->addToolBar("工具栏");
    toolBar->setMovable(false);

    QAction *newAction = new QAction(QIcon::fromTheme("document-new"), "新建", this);
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip("新建");
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
    fileMenu->addAction(newAction);
    toolBar->addAction(newAction);

    QAction *openAction = new QAction(QIcon::fromTheme("document-open"), "打开", this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip("打开");
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    fileMenu->addAction(openAction);
    toolBar->addAction(openAction);

    QAction *saveAction = new QAction(QIcon::fromTheme("document-save"), "保存", this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip("保存");
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    fileMenu->addAction(saveAction);
    toolBar->addAction(saveAction);

    QAction *saveAsAction = new QAction(QIcon::fromTheme("document-save-as"), "另存为", this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip("另存为");
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    fileMenu->addAction(saveAsAction);
    toolBar->addAction(saveAsAction);

    toolBar->addSeparator();
    fileMenu->addSeparator();

    fileMenu->addAction(QIcon::fromTheme("application-exit"), "退出", qApp, SLOT(quit()), QKeySequence::Quit);

    QAction *cutAction = new QAction(QIcon::fromTheme("edit-cut"), "剪切", this);
    cutAction->setShortcuts(QKeySequence::Cut);
    cutAction->setStatusTip("剪切");
    cutAction->setEnabled(false);
    connect(cutAction, SIGNAL(triggered()), codeEditor, SLOT(cut()));
    connect(codeEditor, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
    editMenu->addAction(cutAction);
    toolBar->addAction(cutAction);

    QAction *copyAction = new QAction(QIcon::fromTheme("edit-copy"), "复制", this);
    copyAction->setShortcuts(QKeySequence::Copy);
    copyAction->setStatusTip("复制");
    copyAction->setEnabled(false);
    connect(copyAction, SIGNAL(triggered()), codeEditor, SLOT(copy()));
    connect(codeEditor, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));
    editMenu->addAction(copyAction);
    toolBar->addAction(copyAction);

    QAction *pasteAction = new QAction(QIcon::fromTheme("edit-paste"), "粘贴", this);
    pasteAction->setShortcuts(QKeySequence::Paste);
    pasteAction->setStatusTip("粘贴");
    connect(pasteAction, SIGNAL(triggered()), codeEditor, SLOT(paste()));
    editMenu->addAction(pasteAction);
    toolBar->addAction(pasteAction);

    QAction *undoAction = new QAction(QIcon::fromTheme("edit-undo"), "撤销", this);
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setStatusTip("撤销");
    connect(undoAction, SIGNAL(triggered()), codeEditor, SLOT(undo()));
    connect(codeEditor, SIGNAL(undoAvailable(bool)), undoAction, SLOT(setEnabled(bool)));
    editMenu->addAction(undoAction);
    toolBar->addAction(undoAction);

    QAction *redoAction = new QAction(QIcon::fromTheme("edit-redo"), "重做", this);
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setStatusTip("重做");
    connect(redoAction, SIGNAL(triggered()), codeEditor, SLOT(redo()));
    connect(codeEditor, SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
    editMenu->addAction(redoAction);
    toolBar->addAction(redoAction);

    toolBar->addSeparator();

    QAction *lexAction = new QAction(QIcon::fromTheme("tools-check-spelling"), "词法分析（语法高亮）", this);
    lexAction->setStatusTip("词法分析（语法高亮）");
    connect(lexAction, SIGNAL(triggered()), this, SLOT(runLexical()));
    compileMenu->addAction(lexAction);
    toolBar->addAction(lexAction);

    QAction *synAction = new QAction(QIcon::fromTheme("code-context"), "语法分析（查看推导过程）", this);
    synAction->setStatusTip("语法分析（查看推导过程）");
    connect(synAction, SIGNAL(triggered()), this, SLOT(runSyntactic()));
    compileMenu->addAction(synAction);
    toolBar->addAction(synAction);

    QAction *compileAction = new QAction(QIcon::fromTheme("run-build-configure"), "编译（查看汇编代码）", this);
    compileAction->setStatusTip("编译（查看汇编代码）");
    connect(compileAction, SIGNAL(triggered()), this, SLOT(runCompile()));
    compileMenu->addAction(compileAction);
    toolBar->addAction(compileAction);

    QAction *runAction = new QAction(QIcon::fromTheme("media-playback-start"), "运行（使用Dosbox运行）", this);
    runAction->setStatusTip("运行（使用Dosbox运行）");
    connect(runAction, SIGNAL(triggered()), this, SLOT(runRun()));
    compileMenu->addAction(runAction);
    toolBar->addAction(runAction);

    helpMenu->addAction(QIcon::fromTheme("help-about"), "关于", this, SLOT(about()));
}

void MainWindow::documentWasModified()
{
    setWindowFilePath(codeEditor->curFile);
    setWindowModified(codeEditor->document()->isModified());
}

void MainWindow::runLexical()
{
    codeHighlighter = new CodeHighlighter(codeEditor->document());
}

void MainWindow::runSyntactic()
{

}

void MainWindow::runCompile()
{

}

void MainWindow::runRun()
{

}
