#include "paramtable.h"

void ParamTable::initAll()
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
    table_cons.clear();
    id_tmp = 100; //临时变量的开始值
}
void ParamTable::addFun(int id)
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
void ParamTable::backFun()
{
#ifdef DEBUG
    cout << "backFun" << endl;
#endif
    Funcs.pop();
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("funend", -1, -1, -1));
}
void ParamTable::addNum(int id, bool isparam)
{
#ifdef DEBUG
    cout << "addNum" << ' ' << id << ' ' << isparam << endl;
#endif
    for (int i = 0; i < pfinf[Funcs.top()].synbs.size(); i++)
        if (synb[pfinf[Funcs.top()].synbs[i]].name == id)
        {
            if (id < 100)
            {
                haveErr = true;
                errMessagePT = "Error! redeclaration of 'int " + to_string(id) + "'";
#ifdef DEBUG
                cout << "error: redeclaration of 'int " << id << "'" << endl;
#endif
            }
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
void ParamTable::addArray(int id, int len, bool isparam)
{
#ifdef DEBUG
    cout << "addArray" << ' ' << id << endl;
#endif
    for (int i = 0; i < pfinf[Funcs.top()].synbs.size(); i++)
        if (synb[pfinf[Funcs.top()].synbs[i]].name == id)
        {
            haveErr = true;
            errMessagePT = "Error! redeclaration of 'int* " + to_string(id) + "'";
#ifdef DEBUG
            cout << "error: redeclaration of 'int* " << id << "'" << endl;
#endif
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
void ParamTable::addCon(int id, int data)
{
#ifdef DEBUG
    cout << "addCon" << endl;
#endif
    /*for (int i = 0; i < cons.size(); i++)
    {
        if (cons[i].data == data)
        {
            pfinf[Funcs.top()].synbs.push_back(cons[i].loc);
            return;
        }
    }*/
    synbl newS;
    consl newC;
    newS.name = id;
    newS.cat = 'c';
    newC.data = data;
    newC.loc = synb.size();
    cons.push_back(newC);
    newS.addr = cons.size() - 1;
    synb.push_back(newS);
    pfinf[Funcs.top()].synbs.push_back(synb.size() - 1);
    table_cons.insert(make_pair(id, data));
}
void ParamTable::alGeq(string op)
{
#ifdef DEBUG
    cout << "alGeq" << ' ' << op << endl;
#endif
    int id1, id2;
    if (alNums.empty())
    {
        haveErr = true;
        errMessagePT = "Error! invalid expression";
#ifdef DEBUG
        cout << "error: invalid expression" << endl;
#endif
        return;
    }
    id1 = alNums.top();
    alNums.pop();
    if (alNums.empty())
    {
        haveErr = true;
        errMessagePT = "Error! invalid expression";
#ifdef DEBUG
        cout << "error: invalid expression" << endl;
#endif
        return;
    }
    id2 = alNums.top();
    alNums.pop();
#ifdef DEBUG
    cout << id1 << ' ' << id2 << endl;
#endif
    if (op == "=")
    {
        pfinf[Funcs.top()].elems.push_back(elems.size());
        elems.push_back(elem(op, id1, -1, id2));
        alNums.push(id2);
    }
    else
    {
        addNum(id_tmp);
        pfinf[Funcs.top()].elems.push_back(elems.size());
        addNum(id_tmp);
        elems.push_back(elem(op, id2, id1, id_tmp));
        alNums.push(id_tmp);
        id_tmp++;
    }
}
void ParamTable::alPush(int id, bool isarray)
{ //之后要加一堆处理
#ifdef DEBUG
    cout << "alPush" << ' ' << id << endl;
#endif
    if(!isarray)
        alNums.push(id);
    else
    {
        alNums.push(id);
    }
}
void ParamTable::alPop()
{
#ifdef DEBUG
    cout << "alPop" << endl;
#endif
    if (alNums.empty())
    {
        haveErr = true;
        errMessagePT = "Error! alPop error";
#ifdef DEBUG
        cout << "error: alPop error" << endl;
#endif
        return;
    }
    alNums.pop();
}
void ParamTable::exIf()
{
#ifdef DEBUG
    cout << "exIf" << endl;
#endif
    if (alNums.empty())
    {
        haveErr = true;
        errMessagePT = "Error! expected primary-expression before ')' token";
#ifdef DEBUG
        cout << "error: expected primary-expression before ')' token" << endl;
#endif
        return;
    }
    int id1;
    id1 = alNums.top();
    alNums.pop();
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("if", id1, -1, -1));
}
void ParamTable::exIe()
{
#ifdef DEBUG
    cout << "exIe" << endl;
#endif
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("ie", -1, -1, -1));
}
void ParamTable::exEl()
{
#ifdef DEBUG
    cout << "exEl" << endl;
#endif
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("el", -1, -1, -1));
}
void ParamTable::exWh()
{
#ifdef DEBUG
    cout << "exWh" << endl;
#endif
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("wh", -1, -1, -1));
}
void ParamTable::exDo()
{
#ifdef DEBUG
    cout << "exDo" << endl;
#endif
    if (alNums.empty())
    {
        haveErr = true;
        errMessagePT = "Error! expected primary-expression before ')' token";
#ifdef DEBUG
        cout << "error: expected primary-expression before ')' token" << endl;
#endif
        return;
    }
    int id1;
    id1 = alNums.top();
    alNums.pop();
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("do", id1, -1, -1));
}
void ParamTable::exWe()
{
#ifdef DEBUG
    cout << "exWe" << endl;
#endif
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("we", -1, -1, -1));
}
void ParamTable::callBegin(int id)
{
#ifdef DEBUG
    cout << "callBegin" << ' ' << id << endl;
#endif
    if (!table_pfinf.count(id))
    {
        haveErr = true;
        errMessagePT = "Error! function was not declared in this scope";
#ifdef DEBUG
        cout << "error: '"
             << "' was not declared in this scope" << endl;
#endif
        throw "callError";
        return;
    }
    calls.push(id);
    callParams.push(0);
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("call", id, id_tmp, -1));
    addNum(id_tmp);
    alNums.push(id_tmp);
    id_tmp++;
}
void ParamTable::callEnd()
{
#ifdef DEBUG
    cout << "callEnd" << endl;
#endif
    if (calls.empty() || callParams.empty())
    {
        haveErr = true;
        errMessagePT = "Error! internal error in callEnd";
#ifdef DEBUG
        cout << "internal error in callEnd" << endl;
#endif
        return;
    }
    if (callParams.top() < pfinf[synb[table_pfinf[calls.top()]].addr].param.size())
    {
        haveErr = true;
        errMessagePT = "Error! too few arguments to function";
#ifdef DEBUG
        cout << "error: too few arguments to function '"
             << "'" << endl;
#endif
        return;
    }
    else if (callParams.top() > pfinf[synb[table_pfinf[calls.top()]].addr].param.size())
    {
        haveErr = true;
        errMessagePT = "Error! too many arguments to function";
#ifdef DEBUG
        cout << "error: too many arguments to function '"
             << "'" << endl;
#endif
        return;
    }
    calls.pop();
    callParams.pop();
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("callend", -1, -1, -1));
}
void ParamTable::callParam()
{
#ifdef DEBUG
    cout << "callParam" << endl;
#endif
    if (alNums.empty())
    {
        haveErr = true;
        errMessagePT = "Error! expected primary-expression before ')' token";
#ifdef DEBUG
        cout << "error: expected primary-expression before ')' token" << endl;
#endif
        return;
    }
    int id1;
    id1 = alNums.top();
    alNums.pop();
    int num;
    num = callParams.top();
    callParams.pop();
    callParams.push(++num);
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("params", -1, -1, id1));
}
void ParamTable::retNum()
{
    int id1;
    id1 = alNums.top();
    alNums.pop();
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("retnum", -1, -1, id1));
}
void ParamTable::retNonum()
{
    pfinf[Funcs.top()].elems.push_back(elems.size());
    elems.push_back(elem("ret", -1, -1, -1));
}
void ParamTable::genValls() //生成活动记录表
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
            nowvall.par.insert(make_pair(synb[pfinf[k].param[i]].name, sum));
            nowvall.var.insert(make_pair(synb[pfinf[k].param[i]].name, sum));
            sum += 2;
        }
        nowvall.parsize = sum;
        sum += 2; //为call内的ip留个位置
        for (i = 0; i < pfinf[k].synbs.size(); i++)
        {
            nowvall.var.insert(make_pair(synb[pfinf[k].synbs[i]].name, sum));
            sum += 2;
            if (synb[pfinf[k].synbs[i]].cat == 'l')
            {
                sum += 2 * (ainf[synb[pfinf[k].synbs[i]].addr].up - ainf[synb[pfinf[k].synbs[i]].addr].low);
            }
        }
        nowvall.size = sum;
        basicValls.push_back(nowvall);
    }
}
void ParamTable::gen4elem() //处理四元式的if，while等语句的跳转位置，四元式优化，并输出四元式用来调试
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
            elems[i + 1].needtag = true;
            sem.pop();
            sem.push(i);
        }
        if (elems[i].st == "ie")
        {
            elems[sem.top()].id0 = i + 1;
            elems[i + 1].needtag = true;
            sem.pop();
        }
        if (elems[i].st == "wh")
            sem.push(i);
        if (elems[i].st == "do")
            sem.push(i);
        if (elems[i].st == "we")
        {
            elems[sem.top()].id0 = i + 1;
            elems[i + 1].needtag = true;
            sem.pop();
            elems[i].id0 = sem.top() + 1;
            elems[sem.top() + 1].needtag = true;
            sem.pop();
        }
    }
    vector<int>tmpelems;
    elem tmpelem("",-1,-1,-1);
    int l,r,num;
    for (k = 1; k < pfinf.size(); k++)
    {
        tmpelems.clear();
        for (i = 0; i < pfinf[k].elems.size(); i++)
        {
            tmpelem = elems[pfinf[k].elems[i]];
            if(tmpelem.iscntop())
            {
                if(table_cons.count(tmpelem.id1)&&table_cons.count(tmpelem.id2))
                {
                    synb[basicValls[k].var[tmpelem.id0]].cat='c';
                    l=table_cons[tmpelem.id1];
                    r=table_cons[tmpelem.id2];
                    if(tmpelem.st=="+")
                        num=l+r;
                    else if(tmpelem.st=="-")
                        num=l-r;
                    else if(tmpelem.st=="*")
                        num=l*r;
                    else if(tmpelem.st=="/")
                        num=l/r;
                    table_cons.insert(make_pair(tmpelem.id0,num));
                }else  
                    tmpelems.push_back(pfinf[k].elems[i]);
            }else
                tmpelems.push_back(pfinf[k].elems[i]);
        }
        pfinf[k].elems=tmpelems;
    }
#ifdef DEBUG
    for (i = 0; i < elems.size(); i++)
    {
        printf("%d: ", i);
        elems[i].output();
    }
#endif
}

void ParamTable::outputParam()
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
        for (j = 0; j < pfinf[i].param.size(); j++)
            printf("%d ", pfinf[i].param[j]);
        printf("\nsynbs: ");
        for (j = 0; j < pfinf[i].synbs.size(); j++)
            printf("%d ", pfinf[i].synbs[j]);
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
void ParamTable::toax(int k, int id) //输出到从内存提取到ax的汇编指令
{
    if (table_cons.count(id))
    {
        cout << "MOV   AX," << table_cons[id] << endl;
    }
    else if (basicValls[0].var.count(id))
    {
        id = basicValls[0].var[id];
        cout << "MOV   AX,[DI+" << id << "]" << endl;
    }
    else
    {
        id = basicValls[k].var[id];
        cout << "MOV   AX,[BP-" << id + 2 << "]" << endl;
    }
}
void ParamTable::tobx(int k, int id) //输出到从内存提取到ax的汇编指令
{
    if (table_cons.count(id))
    {
        cout << "MOV   BX," << table_cons[id] << endl;
    }
    else if (basicValls[0].var.count(id))
    {
        id = basicValls[0].var[id];
        cout << "MOV   BX,[DI+" << id << "]" << endl;
    }
    else
    {
        id = basicValls[k].var[id];
        cout << "MOV   BX,[BP-" << id + 2 << "]" << endl;
    }
}
void ParamTable::axto(int k, int id) //输出到从ax提取到内存的汇编指令
{
    if (basicValls[0].var.count(id))
    {
        id = basicValls[0].var[id];
        cout << "MOV   [DI+" << id << "],AX" << endl;
    }
    else
    {
        id = basicValls[k].var[id];
        cout << "MOV   [BP-" << id + 2 << "],AX" << endl;
    }
}
void ParamTable::jgjp(int k, string st, int tg) //符合条件则跳转
{
    if (st == "jmp")
    {
        cout << "JMP   F" << k << "T" << tg << endl;
    }
    else if (st == "==")
    {
        cout << "JE    F" << k << "T" << tg << endl;
    }
    else if (st == "!=")
    {
        cout << "JNE    F" << k << "T" << tg << endl;
    }
    else if (st == ">")
    {
        cout << "JA    F" << k << "T" << tg << endl;
    }
    else if (st == "<=")
    {
        cout << "JNA   F" << k << "T" << tg << endl;
    }
    else if (st == "<")
    {
        cout << "JB    F" << k << "T" << tg << endl;
    }
    else if (st == ">=")
    {
        cout << "JNB   F" << k << "T" << tg << endl;
    }
}
void ParamTable::genAssembly()
{
    int i, j, k, id0, id1, id2, tmpjmp = -1, tmp, tmpsynb;
    string tmpst;
    elem tmpelem("", -1, -1, -1);
    cout << "SSEG  SEGMENT" << endl;
    cout << "SKTOP DW 200 DUP(?)" << endl;
    cout << "SSEG  ENDS" << endl;
    cout << "DSEG  SEGMENT" << endl;
    cout << "MAINV DW " << basicValls[0].size << " DUP(?)" << endl;
    cout << "DSEG  ENDS" << endl;
    cout << "CSEG  SEGMENT" << endl;
    cout << "      ASSUME  SS:SSEG,CS:CSEG,DS:DSEG" << endl;
    for (k = 1; k < pfinf.size(); k++)
    {
        if (k != pfinf.size() - 1)
        {
            cout << "FUN" << k << "  PROC NEAR" << endl;
            cout << "MOV   CX," << (basicValls[k].size - basicValls[k].parsize - 2) / 2 << endl;
            cout << "F" << k << "IN: NOP" << endl;
            cout << "PUSH  AX" << endl; //存临时变量和参数
            cout << "LOOP  F" << k << "IN" << endl;
            cout << "MOV   BP,SI" << endl;
            cout << "PUSH  BP" << endl;
        }
        else
        {
            cout << "START:MOV   AX,DSEG" << endl;
            cout << "MOV   DS,AX" << endl;
            cout << "MOV   AX,SSEG" << endl;
            cout << "MOV   SS,AX" << endl;
            cout << "MOV   BP,SP" << endl;           //BP：当前函数栈顶
            cout << "MOV   DI,OFFSET MAINV" << endl; //DI：全局变量
            cout << "MOV   AX,0" << endl;
            cout << "MOV   CX," << basicValls[k].size / 2 << endl;
            cout << "F" << k << "IN: NOP" << endl;
            cout << "PUSH  AX" << endl; //存临时变量和参数
            cout << "LOOP  F" << k << "IN" << endl;
            cout << "PUSH  BP" << endl; //存bp
        }
        /*for (i = 0; i < pfinf[k].param.size(); i++)
            if (synb[pfinf[k].param[i]].cat == 'c')
            {
                cout<<"MOV   AX,"<<cons[synb[pfinf[k].param[i]].addr].data<<endl;
                axto(k,pfinf[k].param[i]);
            }*/
        for (i = 0; i < pfinf[k].elems.size(); i++)
        {
            tmpelem = elems[pfinf[k].elems[i]];
#ifdef DEBUG
            cout << ";";
            tmpelem.output();
#endif
            if (tmpelem.needtag)
                cout << "F" << k << "T" << pfinf[k].elems[i] << ": NOP" << endl;
            if (tmpelem.st == "=")
            {
                toax(k, tmpelem.id1);
                axto(k, tmpelem.id0);
            }
            else if (tmpelem.iscntop())
            {
                toax(k, tmpelem.id1);
                tobx(k, tmpelem.id2);
                if (tmpelem.st == "+")
                {
                    cout << "ADD   AX,BX" << endl;
                }
                else if (tmpelem.st == "-")
                {
                    cout << "SUB   AX,BX" << endl;
                }
                else if (tmpelem.st == "*")
                {
                    cout << "MUL   BX" << endl;
                }
                else if (tmpelem.st == "/")
                {
                    cout << "MOV   DX,0" << endl;
                    cout << "DIV   BX" << endl;
                }
                else if (tmpelem.st == "%")
                {
                    cout << "DIV   BX" << endl;
                    cout << "MOV   AX,DX" << endl;
                }
                axto(k, tmpelem.id0);
            }
            else if (tmpelem.isjugop())
            {
                toax(k, tmpelem.id1);
                tobx(k, tmpelem.id2);
                cout << "CMP   AX,BX" << endl;
                cout << "MOV   AX,1" << endl;
                jgjp(0, tmpelem.st, ++tmpjmp);
                cout << "MOV   AX,0" << endl;
                cout << "F0T" << tmpjmp << ": NOP" << endl;
                axto(k, tmpelem.id0);
            }
            else if (tmpelem.st == "if")
            {
                toax(k, tmpelem.id1);
                cout << "CMP  AX,0" << endl;
                jgjp(k, "==", tmpelem.id0);
            }
            else if (tmpelem.st == "ie")
            {
            }
            else if (tmpelem.st == "el")
            {
                jgjp(k, "jmp", tmpelem.id0);
            }
            else if (tmpelem.st == "wh")
            {
            }
            else if (tmpelem.st == "do")
            {
                toax(k, tmpelem.id1);
                cout << "CMP  AX,0" << endl;
                jgjp(k, "==", tmpelem.id0);
            }
            else if (tmpelem.st == "we")
            {
                jgjp(k, "jmp", tmpelem.id0);
            }
            else if (tmpelem.st == "call")
            {
                cout << "MOV   SI,SP" << endl;
                tmp = table_pfinf[tmpelem.id1];
                tmp = synb[tmp].addr;
                tmpsynb = tmpelem.id2;
            }
            else if (tmpelem.st == "params")
            {
                toax(k, tmpelem.id0);
                cout << "PUSH  AX" << endl;
            }
            else if (tmpelem.st == "callend")
            {
                cout << "CALL  FUN" << tmp << endl;
                cout << "POP   AX" << endl;
                axto(k, tmpsynb);
            }
            else if (tmpelem.st == "retnum")
            {
                if (k != pfinf.size() - 1)
                {
                    toax(k, tmpelem.id0);
                    cout << "JMP   F" << k << "O" << endl;
                }
                else
                {
                    cout << "MOV   AH,4CH" << endl;
                    cout << "INT   21H" << endl;
                }
            }
            else if (tmpelem.st == "ret")
            { //此处需要照retnum修改
                if (k != pfinf.size() - 1)
                {
                    cout << "JMP   F" << k << "O" << endl;
                }
                else
                {
                    cout << "MOV   AH,4CH" << endl;
                    cout << "INT   21H" << endl;
                }
            }
        }
        if (k != pfinf.size() - 1)
        {
            cout << "F" << k << "O:  NOP" << endl;
            cout << "MOV   CX," << (basicValls[k].size - basicValls[k].parsize) / 2 << endl;
            cout << "F" << k << "O1:NOP" << endl;
            cout << "POP   DX" << endl; //释放临时变量栈空间（size-psize-2)+旧BP（2）
            cout << "LOOP  F" << k << "O1" << endl;
            cout << "POP   SI" << endl; //取出IP
            cout << "MOV   CX," << basicValls[k].parsize / 2 << endl;
            cout << "F" << k << "O2:NOP" << endl;
            cout << "POP   DX" << endl; //释放参数栈空间
            cout << "LOOP  F" << k << "O2" << endl;
            cout << "POP   BP" << endl;
            cout << "PUSH  BP" << endl;
            cout << "PUSH  AX" << endl;
            cout << "PUSH  SI" << endl; //存回IP
            cout << "RET" << endl;
            cout << "FUN" << k << "  ENDP" << endl;
        }
        else
        {
            cout << "MOV   AH,4CH" << endl;
            cout << "INT   21H" << endl;
            cout << "CSEG  ENDS" << endl;
            cout << "      END   START" << endl;
        }
    }
    valls.clear();
}
/*
要解决的问题：出栈入栈的位置对不对？代码段和堆栈段冲突如何解决？
*/
