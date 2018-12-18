#include "paramtable.h"
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
map<int, int> table_synb, table_pfinf;
vector<vall> valls;
vector<vall> basicValls;
void initAll()
{
#ifdef DEBUG
    cout << "initAll" << endl;
#endif
    synb.clear();
    pfinf.clear();
    synbl newS;
    pfinfl newP;
    newS.name = -1;
    newS.cat = 'f';
    newP.level = 0;
    newP.off = 0;
    newP.fn = 0;
    newP.elems.clear();
    newP.param.clear();
    pfinf.push_back(newP);
    newS.addr = pfinf.size() - 1;
    synb.push_back(newS);
    while (!Funcs.empty())
        Funcs.pop();
    Funcs.push(synb[synb.size() - 1].addr);
    size_tmp = 0;
    elems.clear();
    table_synb.clear();
    table_pfinf.clear();
    id_tmp = 100;
}
void addFun(int id)
{
#ifdef DEBUG
    cout << "addFun" << ' ' << id << endl;
#endif
    synbl newS;
    pfinfl newP;
    newS.name = id;
    newS.cat = 'f';
    newP.level = pfinf[Funcs.top()].level + 1;
    newP.off = size_pfinfl;
    newP.fn = 0;
    newP.param.clear();
    newP.synbs.clear();
    newP.elems.clear();
    newP.entry = synb.size();
    pfinf.push_back(newP);
    newS.addr = pfinf.size() - 1;
    synb.push_back(newS);
    table_pfinf.insert(make_pair(id, synb.size() - 1));
    pfinf[Funcs.top()].synbs.push_back(synb.size() - 1);
    pfinf[Funcs.top()].elems.push_back(elems.size());
    Funcs.push(synb[synb.size() - 1].addr);
    elems.push_back(elem("fun", id, -1, -1));
}
void backFun()
{
#ifdef DEBUG
    cout << "backFun" << endl;
#endif
    Funcs.pop();
    elems.push_back(elem("funend", -1, -1, -1));
    pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
}
void addNum(int id, bool isparam)
{
#ifdef DEBUG
    cout << "addNum" << ' ' << id << ' '<<isparam << endl;
#endif
    for (int i = 0; i < pfinf[Funcs.top()].synbs.size(); i++)
        if (synb[pfinf[Funcs.top()].synbs[i]].name == id)
        {
            cout << "error: redeclaration of 'int " << id << "'" << endl;
            return;
        }
    synbl newS;
    newS.name = id;
    newS.cat = 'i';
    synb.push_back(newS);
    if (isparam)
        pfinf[Funcs.top()].param.push_back(synb.size() - 1);
    else
        pfinf[Funcs.top()].synbs.push_back(synb.size() - 1);
}
void addArray(int id, int len, bool isparam)
{
#ifdef DEBUG
    cout << "addArray" << ' ' << id << endl;
#endif
    for (int i = 0; i < pfinf[Funcs.top()].synbs.size(); i++)
        if (synb[pfinf[Funcs.top()].synbs[i]].name == id)
        {
            cout << "error: redeclaration of 'int* " << id << "'" << endl;
            return;
        }
    synbl newS;
    ainfl newA;
    newS.name = id;
    newS.cat = 'l';
    newA.low = synb.size() + 1;
    newA.up = synb.size() + len;
    ainf.push_back(newA);
    newS.addr = ainf.size() - 1;
    synb.push_back(newS);
    if (isparam)
        pfinf[Funcs.top()].param.push_back(synb.size() - 1);
    else
    {
        pfinf[Funcs.top()].synbs.push_back(synb.size() - 1);
        for (int i = 1; i <= len; i++)
        {
            synbl newSS;
            newSS.name = -1;
            newSS.cat = 'i';
            synb.push_back(newSS);
        }
    }
}
void addCon(int id, int data)
{
#ifdef DEBUG
    cout << "addCon" << endl;
#endif
    for (int i = 0; i < cons.size(); i++)
    {
        if (cons[i].data == data)
        {
            return;
        }
    }
    synbl newS;
    consl newC;
    newS.name = id;
    newS.cat = 'c';
    newC.data = data;
    cons.push_back(newC);
    newS.addr = cons.size() - 1;
    synb.push_back(newS);
}
void alGeq(string op)
{
#ifdef DEBUG
    cout << "alGeq" << ' ' << op << endl;
#endif
    int id1, id2;
    if (alNums.empty())
    {
        cout << "error: invalid expression" << endl;
        return;
    }
    id1 = alNums.top();
    alNums.pop();
    if (alNums.empty())
    {
        cout << "error: invalid expression" << endl;
        return;
    }
    id2 = alNums.top();
    alNums.pop();
#ifdef DEBUG
    cout << id1 << ' ' << id2 << endl;
#endif
    if (op == "=")
    {
        elems.push_back(elem(op, id1, -1, id2));
        pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
    }
    else
    {
        addNum(id_tmp);
        elems.push_back(elem(op, id1, id2, id_tmp));
        pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
        alNums.push(id_tmp);
        id_tmp++;
    }
}
void alPush(int id, bool isarray)
{ //之后要加一堆处理
#ifdef DEBUG
    cout << "alPush" << ' ' << id << endl;
#endif
    alNums.push(id);
}
void alPop()
{
#ifdef DEBUG
    cout << "alPop" << endl;
#endif
    if (alNums.empty())
    {
        cout << "error: alPop error" << endl;
        return;
    }
    alNums.pop();
}
void exIf()
{ //需要加跳转地址
#ifdef DEBUG
    cout << "exIf" << endl;
#endif
    if (alNums.empty())
    {
        cout << "error: expected primary-expression before ')' token" << endl;
        return;
    }
    int id1;
    id1 = alNums.top();
    alNums.pop();
    pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
    elems.push_back(elem("if", id1, -1, -1));
}
void exIe()
{
#ifdef DEBUG
    cout << "exIe" << endl;
#endif
    pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
    elems.push_back(elem("ie", -1, -1, -1));
}
void exEl()
{
#ifdef DEBUG
    cout << "exEl" << endl;
#endif
    pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
    elems.push_back(elem("el", -1, -1, -1));
}
void exWh()
{
#ifdef DEBUG
    cout << "exWh" << endl;
#endif
    pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
    elems.push_back(elem("wh", -1, -1, -1));
}
void exDo()
{ //需要加跳转地址
#ifdef DEBUG
    cout << "exDo" << endl;
#endif
    if (alNums.empty())
    {
        cout << "error: expected primary-expression before ')' token" << endl;
        return;
    }
    int id1;
    id1 = alNums.top();
    alNums.pop();
    pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
    elems.push_back(elem("do", id1, -1, -1));
}
void exWe()
{
#ifdef DEBUG
    cout << "exWe" << endl;
#endif
    pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
    elems.push_back(elem("we", -1, -1, -1));
}
void callBegin(int id)
{
#ifdef DEBUG
    cout << "callBegin" << ' ' << id << endl;
#endif
    if (!table_pfinf.count(id))
    {
        cout << "error: '"
             << "' was not declared in this scope" << endl;
        return;
    }
    calls.push(id);
    callParams.push(0);
    pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
    elems.push_back(elem("call", id, -1, -1));
}
void callEnd()
{
#ifdef DEBUG
    cout << "callEnd" << endl;
#endif
    if (calls.empty() || callParams.empty())
    {
        cout << "internal error in callEnd" << endl;
        return;
    }
    if (callParams.top() < pfinf[synb[table_pfinf[calls.top()]].addr].param.size())
    {
        cout << "error: too few arguments to function '"
             << "'" << endl;
        return;
    }
    else if (callParams.top() > pfinf[synb[table_pfinf[calls.top()]].addr].param.size())
    {
        cout << "error: too many arguments to function '"
             << "'" << endl;
        return;
    }
    calls.pop();
    callParams.pop();
}
void callParam()
{
#ifdef DEBUG
    cout << "callParam" << endl;
#endif
    if (alNums.empty())
    {
        cout << "error: expected primary-expression before ')' token" << endl;
        return;
    }
    int id1;
    id1 = alNums.top();
    alNums.pop();
    int num;
    num = callParams.top();
    callParams.pop();
    callParams.push(++num);
    pfinf[Funcs.top()].elems.push_back(elems.size() - 1);
    elems.push_back(elem("params", id1, -1, -1));
}
void gen4elem() //处理四元式的if，while等语句的跳转位置并输出四元式用来调试
{
#ifdef DEBUG
    cout << "wow we are generating elems!" << endl;
#endif
    for (int i = 0; i < synb.size(); i++)
    {
        table_synb.insert(make_pair(synb[i].name, i));
    }
    table_synb[-1] = -1;
    stack<int> sem;
    int i, j, k;
    while (!sem.empty())
        sem.pop();
    for (i = 0; i < elems.size(); i++)
    {
        if (elems[i].st == "if")
            sem.push(i);
        if (elems[i].st == "el")
        {
            elems[sem.top()].id0 = i + 1;
            sem.pop();
            sem.push(i);
        }
        if (elems[i].st == "ie")
        {
            elems[sem.top()].id0 = i + 1;
            sem.pop();
        }
        if (elems[i].st == "wh")
            sem.push(i);
        if (elems[i].st == "do")
            sem.push(i);
        if (elems[i].st == "we")
        {
            elems[sem.top()].id0 = i + 1;
            sem.pop();
            elems[i].id0 = sem.top() + 1;
            sem.pop();
        }
    }
    for (i = 0; i < elems.size(); i++)
    {
        printf("%d: ", i);
        elems[i].output();
    }
}
void genValls() //生成活动记录表
{
    basicValls.clear();
    vall nowvall;
    int i, sum, k;
    for (k = 0; k < pfinf.size(); k++)
    {
        nowvall.var.clear();
        nowvall.par.clear();
        nowvall.size = 0;
        sum = 0;
        for (i = 0; i < pfinf[k].param.size(); i++)
        {
            nowvall.par.insert(make_pair(pfinf[k].param[i], sum++));
        }
        for (i = 0; i < pfinf[k].synbs.size(); i++)
        {
            nowvall.var.insert(make_pair(pfinf[k].synbs[i], sum++));
            if (synb[pfinf[k].synbs[i]].cat == 'l')
            {
                sum += ainf[synb[pfinf[k].synbs[i]].addr].up - ainf[synb[pfinf[k].synbs[i]].addr].low;
            }
        }
        nowvall.size = sum;
        basicValls.push_back(nowvall);
    }
}
void outputParam()
{ //输出参数表内容
    int i, j;
    printf("-----synbl-----\n");
    printf("name--cat--addr\n");
    for (i = 0; i < synb.size(); i++)
    {
        printf("%d: %d  %c  %d\n", i, synb[i].name, synb[i].cat, synb[i].addr);
    }
    printf("\n-----ainfl-----\n");
    printf("low-----up\n");
    for (i = 0; i < ainf.size(); i++)
    {
        printf("%d: %d     %d\n", i, ainf[i].low, ainf[i].up);
    }
    printf("\n-----consl-----\n");
    printf("data\n");
    for (i = 0; i < cons.size(); i++)
    {
        printf("%d: %d\n", i, cons[i].data);
    }
    printf("\n-----pfinfl-----\n");
    printf("synbs-----param\n");
    for (i = 0; i < pfinf.size(); i++)
    {
        printf("params: ");
        for(j=0;j<pfinf[i].param.size();j++)
            printf("%d ",pfinf[i].param[j]);
        printf("\nsynbs: ");
        for(j=0;j<pfinf[i].synbs.size();j++)
            printf("%d ",pfinf[i].synbs[j]);
        printf("\n");
    }
    map<int, int>::iterator it;
    printf("\n-----vall-----\n");
    printf("par---var---size\n");
    for (i = 0; i < basicValls.size(); i++)
    {
        printf("par");
        for (it = basicValls[i].par.begin(); it != basicValls[i].par.end(); it++)
            printf("(%d-%d) ", it->first, it->second);
        printf("\nvar");
        for (it = basicValls[i].var.begin(); it != basicValls[i].var.end(); it++)
            printf("(%d-%d) ", it->first, it->second);
        printf("\nsize");
        printf("%d\n", basicValls[i].size);
    }
}
void genAssembly()
{
    valls.clear();
}