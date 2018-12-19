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
通过符号表可以很好的指定每个变量在内存中的位置
to-do list：
增加活跃信息处理
alpush数组处理
增加数据类型
增加对无符号数的支持
优化：除去未使用变量
优化：及时清除已使用临时变量
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
    int data,loc;
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
    ParamTable(){}

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
int loc_main;
int id_tmp;
stack<char> alNums;
map<int, int> table_synb, table_pfinf, table_cons;
vector<vall> valls;
vector<vall> basicValls;

void initAll();
void addFun(int id);
void backFun();
void addNum(int id,bool isparam=false);
void addArray(int id,int len,bool isparam=false);
void addCon(int id,int data);
void alGeq(string op);
void alPush(int id,int arrayidx=-1);
void alPop();
void exIf();
void exIe();
void exEl();
void exWh();
void exDo();
void exWe();
void callBegin(int id);
void callEnd();
void callParam();
void retNum();
void retNonum();
void gen4elem();
void genValls();
void outputParam();
void genAssembly();

void toax(int k, int id);
void tobx(int k, int id);
void axto(int k, int id);
void jgjp(int k, string st, int tg);

};
#endif // PARAMTABLE_H_INCLUDED
