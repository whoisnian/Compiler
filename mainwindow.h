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
#include <QProcess>
#include <QHeaderView>
#include <QTextStream>
#include <QTableWidget>
#include "codeeditor.h"
#include "codehighlighter.h"
#include "LexicalAnalysis.h"
#include "SyntacticAnalysis.h"

const char synText[100][100] = {
"UNKNOWN ERROR",
"<program> := <declaration_list>",
"<declaration_list> := <declaration> <declaration_list_tail>",
"<declaration_list_tail> := <declaration> <deliaration_list_tail>",
"<declaration_list_tail> := <empty>",
"<declaration> := <var_declaration>",
"<declaration> := <fun_declaration>",
"<var_declaration> := <type_specifier> ID ;",
"<var_declaration> := <type_specifier> ID [ NUM ] ;",
"<type_specifier> := int",
"<type_specifier> := void",
"<fun_declaration> := <type_specifier> ID ( <params> ) <compound_stmt>",
"<params> := <param_list>",
"<params> := void",
"<param_list> := <parm> <parm_list_tail>",
"<parm_list_tail> := , <param> <parm_list_tail>",
"<parm_list_tail> := <empty>",
"<param> := <type_specifier> ID",
"<param> := <type_specifier> ID [ ]",
"<compound_stmt> := { <local_declarations> <statement_list> }",
"<local_declarations> := <var_declaration> <local_declarations_tail>",
"<local_declarations> := <empty>",
"<local_declarations_tail> := <var_declaration> <local_declarations_tail>",
"<local_declarations_tail> := <empty>",
"<statement_list> := <statement> <statement_list_tail>",
"<statement_list> := <empty>",
"<statement_list_tail> := <statement> <statement_list_tail>",
"<statement_list_tail> := <empty>",
"<statement> := <expression_stmt>",
"<statement> := <compound_stmt>",
"<statement> := <selection_stmt>",
"<statement> := <iteration_stmt>",
"<statement> := <return_stmt>",
"<expression_stmt> := <expression> ;",
"<expression_stmt> := ;",
"<selection_stmt> := if ( <expression> ) <statement>",
"<selection_stmt> := if ( <expression> ) <statement> else <statement>",
"<iteration_stmt> := while ( <expression> ) <statement>",
"<return_stmt> := return ;",
"<return_stmt> := return <expression> ;",
"<expression> := <var> = <expression>",
"<expression> := <simple_expression>",
"<var> := ID",
"<var> := ID [ <expression> ]",
"<simple_expression> := <additive_expression> <relop> <additive_expression>",
"<simple_expression> := <additive_expression>",
"<relop> := <=",
"<relop> := <",
"<relop> := >",
"<relop> := >=",
"<relop> := ==",
"<relop> := !=",
"<additive_expression> := <term> <additive_expression_tail>",
"<additive_expression_tail> := <addop> <term> <additive_expression_tail>",
"<additive_expression_tail> := <empty>",
"<addop> := +",
"<addop> := -",
"<term> := <factor> <term_tail>",
"<term_tail> := <mulop> <factor> <term_tail>",
"<term_tail> := <empty>",
"<mulop> := *",
"<mulop> := /",
"<mulop> := %",
"<factor> := ( <expression> )",
"<factor> := <var>",
"<factor> := <call>",
"<factor> := NUM",
"<call> := ID ( <args> )",
"<args> := <arg_list>",
"<args> := <empty>",
"<arg_list> := <expression> <arg_list_tail>",
"<arg_list_tail> := , <expression> <arg_list_tail>",
"<arg_list_tail> := <empty>",
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void showError(int lineNumber, std::string errMessage);

public slots:
    void about();
    void showSynText();
    void newFile();
    void openFile();
    void saveFile();
    void saveAs();
    void documentWasModified();
    void runLexical();
    void runSyntactic();
    void show4Elem();
    void showParamTable();
    void runCompile();
    void runRun();

private:
    void setupMenuAndToolBar();

    CodeEditor *codeEditor;
    CodeHighlighter *codeHighlighter;
};

#endif // MAINWINDOW_H
