#ifndef PARAMTABLE_H_INCLUDED
#define PARAMTABLE_H_INCLUDED

#include<iostream>
#include<cmath>
#include<cstring>
#include<string>
#include<vector>
#include<stack>
#include<map>
#include<algorithm>
using namespace std;
/*
to-do list：
增加数据类型
*/
struct synbl{
    int name;
    char cat;//f:function,i:int,v:void,l:intarray,c:const
    int addr;
};
struct ainfl{
    int low,up;
    int clen;
};
struct pfinfl{
    int level,off,fn;
    vector<int>param;
    vector<int>synbs;
    vector<int>elems;
    int entry;
};
struct consl{
    int data;
};
struct lenl{
    int len;
};
struct elem{
    elem(string ist,int iid1,int iid2,int iid0){
        st=ist;id1=iid1;id2=iid2;id0=iid0;needtag=false;
    }
    void output(){
        cout<<st<<' '<<id1<<' '<<id2<<' '<<id0<<endl;
    }
    bool iscntop(){
        if(st=="+"||st=="-"||st=="*"||st=="/"||st=="%")return true;
        return false;
    }
    bool isjugop(){
        if(st=="=="||st=="<="||st==">="||st=="!="||st=="<"||st==">")return true;
        return false;
    }
    string st;
    int id1,id2,id0;
    bool needtag;
};
struct vall{
    map<int,int>var;
    map<int,int>par;
    int parsize,size;
    int retaddr;
};

class ParamTable
{
public:
    ParamTable(){haveErr = false;errMessagePT = "";assemblyRes.clear();}
bool haveErr;               // 错误标记
string errMessagePT;        // 错误信息
vector<string> assemblyRes; // 汇编代码结果

vector<synbl> synb;
vector<pfinfl> pfinf;
stack<int> Funcs;
stack<int> calls, callParams;
vector<ainfl> ainf;
vector<consl> cons;
vector<elem> elems;
int size_pfinfl;
int size_ainfl;
int size_consl;
int size_tmp;
int beginidx_id_tmp;//临时变量开始位置
int id_tmp;//临时变量编号
int inalNums[400];//判断临时变量是否在用
stack<int> alNums;//算术栈
map<int, int> table_synb, table_pfinf, table_cons, table_ainf;
vector<vall> valls;
vector<vall> basicValls;
vector<bool>qt[3];//活跃信息表，对应每个四元式id0，id1和id2的活跃信息
int symbl[400];
int locra,locradiff,locrc,locrcdiff;//当前寄存器内容

void initAll();
void addFun(int id);
void backFun();
void addNum(int id,bool isparam=false);
void addArray(int id,int len,bool isparam=false);
void addCon(int id,int data);
void alGeq(string op);
void alPush(int id,bool isarray=false);
void alPop();
void exIf();
void exIe();
void exEl();
void exWh();
void exDo();
void exWe();
void exIn();
void exOut();
void callBegin(int id);
void callEnd();
void callParam();
void retNum();
void retNonum();
void genValls();
void gen4elem();
void outputParam();
void genAssembly();
void outputAssembly();

void toax(int k, int id,int diffid=-1);
void tocx(int k, int id,int diffid=-1);
void axto(int k, int id,int diffid=-1);
void jgjp(int k, string st, int tg);

};
#endif // PARAMTABLE_H_INCLUDED
