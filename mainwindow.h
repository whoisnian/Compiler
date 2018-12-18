#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QToolBar>
#include "codeeditor.h"
#include "codehighlighter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void showError(int lineNumber, std::string errMessage);

public slots:
    void about();
    void newFile();
    void openFile();
    void saveFile();
    void saveAs();
    void documentWasModified();
    void runLexical();
    void runSyntactic();
    void runCompile();
    void runRun();

private:
    void setupMenuAndToolBar();

    CodeEditor *codeEditor;
    CodeHighlighter *codeHighlighter;
};

#endif // MAINWINDOW_H
