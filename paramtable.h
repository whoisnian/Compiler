#ifndef PARAMTABLE_H_INCLUDED
#define PARAMTABLE_H_INCLUDED

#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
using namespace std;
/*
to-do list：
增加数据类型
*/
struct synbl
{ //符号表
    int name;
    char cat; //f:function,i:int,v:void,l:intarray,c:const
    int addr;
};
struct ainfl
{                //数组表
    int low, up; //首地址，末地址
    int clen;    //长度
};
struct pfinfl
{ //函数表
    int level, off, fn;
    vector<int> param; //参数
    vector<int> synbs; //局部变量
    vector<int> elems; //包含的四元式
    int entry;
};
struct consl
{ //常数表
    int data;
};
struct lenl
{
    int len;
};
struct elem
{ //四元式，组成是st,id1,id2,id0
    elem(string ist, int iid1, int iid2, int iid0)
    {
        st = ist;
        id1 = iid1;
        id2 = iid2;
        id0 = iid0;
        needtag = false;
    }
    void output()
    {
        cout << st << ' ' << id1 << ' ' << id2 << ' ' << id0 << endl;
    }
    bool iscntop()
    { //判断是运算符
        if (st == "+" || st == "-" || st == "*" || st == "/" || st == "%")
            return true;
        return false;
    }
    bool isjugop()
    { //判断是比较符
        if (st == "==" || st == "<=" || st == ">=" || st == "!=" || st == "<" || st == ">")
            return true;
        return false;
    }
    string st;
    int id1, id2, id0;
    bool needtag; //表示这个四元式前需要一个汇编内跳转的标签
};
struct vall
{                      //活动记录表
    map<int, int> var; //每个变量的参数表编号<->汇编中内存偏移地址
    map<int, int> par; //每个参数的参数表编号<->汇编中内存偏移地址
    int parsize, size; //参数表大小和总大小
};

class ParamTable
{
  public:
    ParamTable()
    {
        haveErr = false;
        errMessagePT = "";
        assemblyRes.clear();
    }
    bool haveErr;               // 错误标记
    string errMessagePT;        // 错误信息
    vector<string> assemblyRes; // 汇编代码结果

    vector<synbl> synb;   //整体符号表
    vector<pfinfl> pfinf; //整体函数表
    stack<int> Funcs;//语义分析时记录当前所在函数
    stack<int> calls, callParams;//语义分析时记录当前马上要调用的函数和已经输入的变量数量
    vector<ainfl> ainf;//整体数组表
    vector<consl> cons;//整体常数表
    vector<elem> elems;//整体四元式记录
    int size_pfinfl;
    int size_ainfl;
    int size_consl;
    int size_tmp;
    int beginidx_id_tmp; //临时变量开始位置
    int id_tmp;          //临时变量编号
    int inalNums[400];   //判断临时变量是否在用
    stack<int> alNums;   //算术栈
    map<int, int> table_synb, table_pfinf, table_cons, table_ainf;
    vector<vall> valls;
    vector<vall> basicValls;//整体活动记录
    vector<bool> qt[3]; //活跃信息表，对应每个四元式id0，id1和id2的活跃信息
    int symbl[400];
    int locra, locradiff, locrc, locrcdiff; //当前寄存器(ax,cx)内容
    /*
    *语义函数
    *出现在文法的位置见actFun.txt
    */
    void initAll();//整体初始化
    void addFun(int id);//添加函数
    void backFun();//函数结束
    void addNum(int id, bool isparam = false);//添加变量
    void addArray(int id, int len, bool isparam = false);//添加数组
    void addCon(int id, int data);//添加常数
    void alGeq(string op);//算术栈计算
    void alPush(int id, bool isarray = false);//算术栈压栈
    void alPop();//算术栈出栈（未使用）
    void exIf();
    void exIe();
    void exEl();
    void exWh();
    void exDo();
    void exWe();
    void exIn();
    void exOut();
    void callBegin(int id);//调用函数开始
    void callEnd();//调用函数
    void callParam();//调用函数时提供参数
    void retNum();//当前函数退出并返回一变量
    void retNonum();//当前函数退出并无返回值
    void genValls();//生成活动记录
    void gen4elem();//生成四元式
    void outputParam();//输出参数表供调试
    /*
    *目标代码生成部分
    */
    void genAssembly();//生成8086汇编代码
    void outputAssembly();//输出8086汇编代码
    /*
    *汇编辅助函数
    */
    void toax(int k, int id, int diffid = -1);//把编号为id，偏移地址（如果有）为diffid的内存内容送到ax
    void tocx(int k, int id, int diffid = -1);//把编号为id，偏移地址（如果有）为diffid的内存内容送到cx
    void axto(int k, int id, int diffid = -1);//把ax内容送到编号为id，偏移地址（如果有）为diffid的内存内
    void jgjp(int k, string st, int tg);//根据比较符st和跳转位置tg，判断是否跳转
};
#endif // PARAMTABLE_H_INCLUDED
