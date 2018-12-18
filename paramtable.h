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
return处理
alpush数组处理
增加数据类型
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
    int data;
};
struct lenl{
    int len;
};
struct elem{
    elem(string ist,int iid1,int iid2,int iid0){
        st=ist;id1=iid1;id2=iid2;id0=iid0;
    }
    void output(){
        cout<<st<<' '<<id1<<' '<<id2<<' '<<id0<<endl;
    }
    string st;
    int id1,id2,id0;
};
struct vall{
    map<int,int>var;
    map<int,int>par;
    int size;
    int retaddr;
};
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
void callBegin(int id);
void callEnd();
void callParam();
void gen4elem();
void genValls();
void outputParam();
void genAssembly();
#endif // PARAMTABLE_H_INCLUDED
