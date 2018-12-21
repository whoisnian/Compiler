#include "mainwindow.h"

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
    this->statusBar()->showMessage(QString(errMessage.c_str()), 2000);
    this->codeEditor->highlightErrorLine(lineNumber);
}

void MainWindow::about()
{
    QMessageBox::about(this, "About", "<b>Compiler</b>"\
                       "<p>Based on Qt Creator 4.8.0</p>"\
                       "<p>Based on Qt 5.12.0 (GCC 8.2.1 20181127, 64 bit)</p>");
}

void MainWindow::showSynText()
{
    QTableWidget *table = new QTableWidget(72, 2, nullptr);
    for(int i = 0;i < 72;i++)
    {
        table->setItem(i, 0, new QTableWidgetItem(std::to_string(i+1).c_str()));
        table->setItem(i, 1, new QTableWidgetItem(synText[i+1]));
    }
    table->horizontalHeader()->hide();
    table->verticalHeader()->hide();
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->resize(900, 594);
    table->setWindowIcon(QIcon::fromTheme("view-calendar-list"));
    table->setWindowTitle("文法支持");
    table->setWordWrap(false);
    table->resizeColumnsToContents();
    table->show();
    return;
}

void MainWindow::newFile()
{
    this->codeEditor->curFile.clear();
    this->codeEditor->clear();
    codeEditor->document()->setModified(false);
    setWindowModified(false);
    setWindowFilePath("untitled.txt");
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", QDir::homePath());

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

    QAction *showPTAction = new QAction(QIcon::fromTheme("view-media-playlist"), "显示符号表", this);
    showPTAction->setStatusTip("显示符号表");
    connect(showPTAction, SIGNAL(triggered()), this, SLOT(showParamTable()));
    compileMenu->addAction(showPTAction);
    toolBar->addAction(showPTAction);

    QAction *elemAction = new QAction(QIcon::fromTheme("labplot-4x-zoom"), "生成四元式", this);
    elemAction->setStatusTip("生成四元式");
    connect(elemAction, SIGNAL(triggered()), this, SLOT(show4Elem()));
    compileMenu->addAction(elemAction);
    toolBar->addAction(elemAction);

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

    QAction *showSynTextAction = new QAction(QIcon::fromTheme("view-calendar-list"), "查看文法支持", this);
    showSynTextAction->setStatusTip("查看文法支持");
    connect(showSynTextAction, SIGNAL(triggered()), this, SLOT(showSynText()));
    helpMenu->addAction(showSynTextAction);

    helpMenu->addAction(QIcon::fromTheme("help-about"), "关于", this, SLOT(about()));
}

void MainWindow::documentWasModified()
{
    setWindowModified(codeEditor->document()->isModified());
    setWindowFilePath(codeEditor->curFile);
}

void MainWindow::runLexical()
{
    codeHighlighter = new CodeHighlighter(codeEditor->document());
}

void MainWindow::runSyntactic()
{
    Scan scanner;
    scanner.initFrom(codeEditor->document()->toPlainText().toStdString());
    if(scanner.errPos != -1)
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("词法分析遇到问题");
        if(scanner.errPos == 0)
            msg.setText(std::string("错误原因\n\n"+scanner.errMessage).c_str());
        else
            msg.setText(std::string("第"+std::to_string(scanner.errLine+1)+"行\n\n"+scanner.errMessage).c_str());
        msg.addButton(QMessageBox::Ok);
        msg.exec();
        return;
    }
    Syntax syn(scanner);
    if(syn.program() == 0&&syn.scanner.next().isEOF())
    {
        QTableWidget *table = new QTableWidget(syn.syntaxProcess.size(), 3, nullptr);
        for(int i = 0;i < (int)syn.syntaxProcess.size();i++)
        {
            table->setItem(i, 0, new QTableWidgetItem(std::to_string(i).c_str()));
            table->setItem(i, 1, new QTableWidgetItem(std::to_string(syn.syntaxProcess.at(i)).c_str()));
            table->setItem(i, 2, new QTableWidgetItem(synText[syn.syntaxProcess.at(i)]));
        }
        table->horizontalHeader()->hide();
        table->verticalHeader()->hide();
        table->horizontalHeader()->setStretchLastSection(true);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->resize(900, 594);
        table->setWindowIcon(QIcon::fromTheme("code-context"));
        table->setWindowTitle("语法分析过程");
        table->setWordWrap(false);
        table->resizeColumnsToContents();
        table->show();
        return;
    }
    else
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("语法分析遇到问题");
        msg.setText(std::string("第"+std::to_string(syn.scanner.tokens.at(syn.errPos).lineNumber+1)+"行\n\n"+syn.errMessage+syn.scanner.tokens.at(syn.errPos).name).c_str());
        msg.addButton(QMessageBox::Ok);
        msg.exec();
        showError(syn.scanner.tokens.at(syn.errPos).lineNumber+1, syn.errMessage+syn.scanner.tokens.at(syn.errPos).name);
        return;
    }
}

void MainWindow::showParamTable()
{
    Scan scanner;
    scanner.initFrom(codeEditor->document()->toPlainText().toStdString());
    if(scanner.errPos != -1)
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("词法分析遇到问题");
        if(scanner.errPos == 0)
            msg.setText(std::string("错误原因\n\n"+scanner.errMessage).c_str());
        else
            msg.setText(std::string("第"+std::to_string(scanner.errLine+1)+"行\n\n"+scanner.errMessage).c_str());
        msg.addButton(QMessageBox::Ok);
        msg.exec();
        return;
    }
    Syntax syn(scanner);
    if(syn.program() == 0&&syn.scanner.next().isEOF())
    {
        try
        {
            syn.scanner.curIndex = 0;
            syn.prepare_for_4elem();
            syn.gen4elem();
        }catch(...){}

        if(syn.haveErr)
        {
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Critical);
            msg.setWindowTitle("生成四元式遇到问题");
            msg.setText(syn.errMessagePT.c_str());
            msg.addButton(QMessageBox::Ok);
            msg.exec();
            return;
        }

        QTableWidget *table = new QTableWidget(syn.synb.size(), 4, nullptr);
        for(int i = 0;i < (int)syn.synb.size();i++)
        {
            table->setItem(i, 0, new QTableWidgetItem(std::to_string(i).c_str()));
            table->setItem(i, 1, new QTableWidgetItem(std::to_string(syn.synb.at(i).name).c_str()));
            if(syn.synb.at(i).name >= 0&&syn.synb.at(i).name < syn.scanner.identifierAndIntTable.size())
            {
                table->item(i, 1)->setToolTip(("name: " + syn.scanner.identifierAndIntTable.at(syn.synb.at(i).name)).c_str());
                table->item(i, 1)->setTextColor(Qt::yellow);
            }

            switch(syn.synb.at(i).cat)
            {
            case 'f':
                table->setItem(i, 2, new QTableWidgetItem("function"));
                break;
            case 'i':
                table->setItem(i, 2, new QTableWidgetItem("int"));
                break;
            case 'v':
                table->setItem(i, 2, new QTableWidgetItem("void"));
                break;
            case 'l':
                table->setItem(i, 2, new QTableWidgetItem("int array"));
                break;
            case 'c':
                table->setItem(i, 2, new QTableWidgetItem("const"));
                break;
            default:
                table->setItem(i, 2, new QTableWidgetItem("unknown"));
            }
            table->setItem(i, 3, new QTableWidgetItem(std::to_string(syn.synb.at(i).addr).c_str()));
            switch(syn.synb.at(i).cat)
            {
            case 'l':
                table->item(i, 3)->setToolTip(("low: " + std::to_string(syn.ainf.at(syn.synb.at(i).addr).low) +
                                               "\nup : " + std::to_string(syn.ainf.at(syn.synb.at(i).addr).up)
                                               ).c_str());
                table->item(i, 3)->setTextColor(Qt::yellow);
                break;
            case 'c':
                table->item(i, 3)->setToolTip(("data: " + std::to_string(syn.cons.at(syn.synb.at(i).addr).data)).c_str());
                table->item(i, 3)->setTextColor(Qt::yellow);
                break;
            }
        }
        table->horizontalHeader()->hide();
        table->verticalHeader()->hide();
        table->horizontalHeader()->setStretchLastSection(true);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->resize(330, 594);
        table->setWindowIcon(QIcon::fromTheme("view-media-playlist"));
        table->setWindowTitle("符号表");
        table->setWordWrap(false);
        table->resizeColumnsToContents();
        table->show();
        return;
    }
    else
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("语法分析遇到问题");
        msg.setText(std::string("第"+std::to_string(syn.scanner.tokens.at(syn.errPos).lineNumber+1)+"行\n\n"+syn.errMessage+syn.scanner.tokens.at(syn.errPos).name).c_str());
        msg.addButton(QMessageBox::Ok);
        msg.exec();
        showError(syn.scanner.tokens.at(syn.errPos).lineNumber+1, syn.errMessage+syn.scanner.tokens.at(syn.errPos).name);
        return;
    }
}

void MainWindow::show4Elem()
{
    Scan scanner;
    scanner.initFrom(codeEditor->document()->toPlainText().toStdString());
    if(scanner.errPos != -1)
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("词法分析遇到问题");
        if(scanner.errPos == 0)
            msg.setText(std::string("错误原因\n\n"+scanner.errMessage).c_str());
        else
            msg.setText(std::string("第"+std::to_string(scanner.errLine+1)+"行\n\n"+scanner.errMessage).c_str());
        msg.addButton(QMessageBox::Ok);
        msg.exec();
        return;
    }
    Syntax syn(scanner);
    if(syn.program() == 0&&syn.scanner.next().isEOF())
    {
        try
        {
            syn.scanner.curIndex = 0;
            syn.prepare_for_4elem();
            syn.gen4elem();
        }catch(...){}

        if(syn.haveErr)
        {
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Critical);
            msg.setWindowTitle("生成四元式遇到问题");
            msg.setText(syn.errMessagePT.c_str());
            msg.addButton(QMessageBox::Ok);
            msg.exec();
            return;
        }

        QTableWidget *table = new QTableWidget(syn.elems.size(), 5, nullptr);
        for(int i = 0;i < (int)syn.elems.size();i++)
        {
            table->setItem(i, 0, new QTableWidgetItem(std::to_string(i).c_str()));
            table->setItem(i, 1, new QTableWidgetItem(syn.elems.at(i).st.c_str()));
            table->setItem(i, 2, new QTableWidgetItem(std::to_string(syn.elems.at(i).id1).c_str()));
            if(syn.elems.at(i).id1 >= 0&&syn.elems.at(i).id1 < syn.scanner.identifierAndIntTable.size())
            {
                table->item(i, 2)->setToolTip(("name: " + syn.scanner.identifierAndIntTable.at(syn.elems.at(i).id1)).c_str());
                table->item(i, 2)->setTextColor(Qt::yellow);
            }
            table->setItem(i, 3, new QTableWidgetItem(std::to_string(syn.elems.at(i).id2).c_str()));
            if(syn.elems.at(i).id2 >= 0&&syn.elems.at(i).id2 < syn.scanner.identifierAndIntTable.size())
            {
                table->item(i, 3)->setToolTip(("name: " + syn.scanner.identifierAndIntTable.at(syn.elems.at(i).id2)).c_str());
                table->item(i, 3)->setTextColor(Qt::yellow);
            }
            table->setItem(i, 4, new QTableWidgetItem(std::to_string(syn.elems.at(i).id0).c_str()));
            if(syn.elems.at(i).id0 >= 0&&syn.elems.at(i).id0 < syn.scanner.identifierAndIntTable.size())
            {
                table->item(i, 4)->setToolTip(("name: " + syn.scanner.identifierAndIntTable.at(syn.elems.at(i).id0)).c_str());
                table->item(i, 4)->setTextColor(Qt::yellow);
            }
            //table->setItem(i, 5, new QTableWidgetItem((syn.elems.at(i).needtag?"true":"false")));
        }
        table->horizontalHeader()->hide();
        table->verticalHeader()->hide();
        table->horizontalHeader()->setStretchLastSection(true);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        //table->resize(400, 594);
        table->resize(330, 594);
        table->setWindowIcon(QIcon::fromTheme("labplot-4x-zoom"));
        table->setWindowTitle("四元式结果");
        table->setWordWrap(false);
        table->resizeColumnsToContents();
        table->show();
        return;
    }
    else
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("语法分析遇到问题");
        msg.setText(std::string("第"+std::to_string(syn.scanner.tokens.at(syn.errPos).lineNumber+1)+"行\n\n"+syn.errMessage+syn.scanner.tokens.at(syn.errPos).name).c_str());
        msg.addButton(QMessageBox::Ok);
        msg.exec();
        showError(syn.scanner.tokens.at(syn.errPos).lineNumber+1, syn.errMessage+syn.scanner.tokens.at(syn.errPos).name);
        return;
    }
}

void MainWindow::runCompile()
{
    Scan scanner;
    scanner.initFrom(codeEditor->document()->toPlainText().toStdString());
    if(scanner.errPos != -1)
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("词法分析遇到问题");
        if(scanner.errPos == 0)
            msg.setText(std::string("错误原因\n\n"+scanner.errMessage).c_str());
        else
            msg.setText(std::string("第"+std::to_string(scanner.errLine+1)+"行\n\n"+scanner.errMessage).c_str());
        msg.addButton(QMessageBox::Ok);
        msg.exec();
        return;
    }
    Syntax syn(scanner);
    if(syn.program() == 0&&syn.scanner.next().isEOF())
    {
        try
        {
            syn.scanner.curIndex = 0;
            syn.prepare_for_4elem();
            syn.gen4elem();
            syn.genValls();
            syn.genAssembly();
        }catch(...){}

        if(syn.haveErr)
        {
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Critical);
            msg.setWindowTitle("生成四元式遇到问题");
            msg.setText(syn.errMessagePT.c_str());
            msg.addButton(QMessageBox::Ok);
            msg.exec();
            return;
        }

        QString srcCode = "";
        for(int i = 0;i < syn.assemblyRes.size();i++)
        {
            srcCode.append(syn.assemblyRes.at(i).c_str());
            srcCode.append("\n");
        }
        QPlainTextEdit *showEditor = new CodeEditor(nullptr);
        showEditor->setPlainText(srcCode);
        showEditor->setWindowTitle("汇编代码");
        showEditor->setWindowIcon(QIcon::fromTheme("format-text-code"));
        showEditor->resize(450, 594);
        showEditor->show();
        return;
    }
    else
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("语法分析遇到问题");
        msg.setText(std::string("第"+std::to_string(syn.scanner.tokens.at(syn.errPos).lineNumber+1)+"行\n\n"+syn.errMessage+syn.scanner.tokens.at(syn.errPos).name).c_str());
        msg.addButton(QMessageBox::Ok);
        msg.exec();
        showError(syn.scanner.tokens.at(syn.errPos).lineNumber+1, syn.errMessage+syn.scanner.tokens.at(syn.errPos).name);
        return;
    }
}

void MainWindow::runRun()
{
    QString command;
    QStringList arguments;
    QProcess *haveDosbox = new QProcess(this);
    haveDosbox->setProcessChannelMode(QProcess::MergedChannels);
    command = "/usr/bin/dosbox";

    arguments.clear();
    arguments << "--version";
    haveDosbox->start(command, arguments);
    haveDosbox->waitForFinished(-1);
    if(haveDosbox->readAllStandardOutput().size() == 0)
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("缺少运行环境");
        msg.setTextFormat(Qt::RichText);
        msg.setText("需要x86模拟器 DOSBox<br/><br/>官方网站：<a href=\"https://www.dosbox.com/\">https://www.dosbox.com/</a>");
        msg.addButton(QMessageBox::Ok);
        msg.exec();
    }
    else
    {
        Scan scanner;
        scanner.initFrom(codeEditor->document()->toPlainText().toStdString());
        if(scanner.errPos != -1)
        {
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Critical);
            msg.setWindowTitle("词法分析遇到问题");
            if(scanner.errPos == 0)
                msg.setText(std::string("错误原因\n\n"+scanner.errMessage).c_str());
            else
                msg.setText(std::string("第"+std::to_string(scanner.errLine+1)+"行\n\n"+scanner.errMessage).c_str());
            msg.addButton(QMessageBox::Ok);
            msg.exec();
            return;
        }
        Syntax syn(scanner);
        if(syn.program() == 0&&syn.scanner.next().isEOF())
        {
            try
            {
                syn.scanner.curIndex = 0;
                syn.prepare_for_4elem();
                syn.gen4elem();
                syn.genValls();
                syn.genAssembly();
            }catch(...){}

            if(syn.haveErr)
            {
                QMessageBox msg(this);
                msg.setIcon(QMessageBox::Critical);
                msg.setWindowTitle("生成四元式遇到问题");
                msg.setText(syn.errMessagePT.c_str());
                msg.addButton(QMessageBox::Ok);
                msg.exec();
                return;
            }

            QString srcCode = "";
            for(int i = 0;i < syn.assemblyRes.size();i++)
            {
                srcCode.append(syn.assemblyRes.at(i).c_str());
                srcCode.append("\n");
            }

            QString dosDir = "/home/nian/dos/";
            QString fileBaseName = "output";

            QFile file(dosDir + fileBaseName + ".asm");
            if(!file.open(QFile::WriteOnly|QFile::Text))
            {
                this->statusBar()->showMessage("写入汇编失败", 2000);
                return;
            }

            QTextStream out(&file);
            out << srcCode;

            QProcess *runDosbox = new QProcess(this);
            arguments.clear();
            arguments << "-c" << "mount C: " + dosDir
                      << "-c" << "path %path%;C:\\tools\\"
                      << "-c" << "C:"
                      << "-c" << "masm " + fileBaseName + ".asm," + fileBaseName + ".obj;"
                      << "-c" << "link " + fileBaseName + ".obj," + fileBaseName + ".exe;"
                      << "-c" << fileBaseName + ".exe";
            runDosbox->start(command, arguments);
            return;
        }
        else
        {
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Critical);
            msg.setWindowTitle("语法分析遇到问题");
            msg.setText(std::string("第"+std::to_string(syn.scanner.tokens.at(syn.errPos).lineNumber+1)+"行\n\n"+syn.errMessage+syn.scanner.tokens.at(syn.errPos).name).c_str());
            msg.addButton(QMessageBox::Ok);
            msg.exec();
            showError(syn.scanner.tokens.at(syn.errPos).lineNumber+1, syn.errMessage+syn.scanner.tokens.at(syn.errPos).name);
            return;
        }
    }
}
