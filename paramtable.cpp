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
    table_ainf.clear();
    beginidx_id_tmp = 100;
    id_tmp = beginidx_id_tmp; //临时变量的开始值，正常变量数大于100时需要修改这个值
    memset(inalNums, 0, sizeof(inalNums));
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
    newA.low = id_tmp;
    table_ainf.insert(make_pair(id_tmp, 1984));
    newA.up = id_tmp + len - 1;
    ainf.push_back(newA);
    newS.addr = ainf.size() - 1;
    table_ainf.insert(make_pair(id, synb.size()));
    synb.push_back(newS);
    if (isparam)
        pfinf[Funcs.top()].param.push_back(synb.size() - 1);
    else
    {
        pfinf[Funcs.top()].synbs.push_back(synb.size() - 1);
        for (int i = 1; i <= len; i++)
        {
            addNum(id_tmp);
            inalNums[id_tmp] = 1;
            id_tmp++;
        }
    }
}
void ParamTable::addCon(int id, int data)
{
#ifdef DEBUG
    cout << "addCon" << endl;
#endif
    synbl newS;
    consl newC;
    newS.name = id;
    newS.cat = 'c';
    newC.data = data;
    cons.push_back(newC);
    newS.addr = cons.size() - 1;
    synb.push_back(newS);
    table_cons.insert(make_pair(id, data));
}
void ParamTable::alGeq(string op)
{
#ifdef DEBUG
    cout << "alGeq" << ' ' << op << endl;
#endif
    int id1, id2, diffid1 = -1, diffid2 = -1;
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
    if (table_ainf.count(id1))
    {
        diffid1 = alNums.top();
        alNums.pop();
        id1 = ainf[synb[table_ainf[id1]].addr].low;
    }
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
    if (table_ainf.count(id2))
    {
        diffid2 = alNums.top();
        alNums.pop();
        id2 = ainf[synb[table_ainf[id2]].addr].low;
    }
#ifdef DEBUG
    cout << id1 << ' ' << id2 << endl;
#endif
    if (op == "=")
    {
        if (diffid1 != -1)
        {
            pfinf[Funcs.top()].elems.push_back(elems.size());
            elems.push_back(elem("arr1", -1, -1, diffid1));
        }
        if (diffid2 != -1)
        {
            pfinf[Funcs.top()].elems.push_back(elems.size());
            elems.push_back(elem("arr2", -1, -1, diffid2));
        }
        pfinf[Funcs.top()].elems.push_back(elems.size());
        elems.push_back(elem(op, id1, -1, id2));
        alNums.push(id2);
    }
    else
    {
        if (diffid2 != -1)
        {
            pfinf[Funcs.top()].elems.push_back(elems.size());
            elems.push_back(elem("arr1", -1, -1, diffid2));
        }
        if (diffid1 != -1)
        {
            pfinf[Funcs.top()].elems.push_back(elems.size());
            elems.push_back(elem("arr2", -1, -1, diffid1));
        }
        pfinf[Funcs.top()].elems.push_back(elems.size());
        //if (min(id1, id2) >= beginidx_id_tmp) //减少临时变量开销
        //    id_tmp = min(id1, id2);
        //else if (max(id1, id2) >= beginidx_id_tmp)
        //    id_tmp = max(id1, id2);
        //else
            addNum(id_tmp);
        elems.push_back(elem(op, id2, id1, id_tmp));
        alNums.push(id_tmp);
        inalNums[id_tmp] = 1;
#ifdef DEBUG
        cout << "push " << alNums.top() << endl;
#endif
        id_tmp++;
    }
}
void ParamTable::alPush(int id, bool isarray)
{ //之后要加一堆处理
#ifdef DEBUG
    cout << "alPush" << ' ' << id << endl;
#endif
    if (!isarray)
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
void ParamTable::exIn()
{
#ifdef DEBUG
    cout << "exIn" << endl;
#endif
    int id0, diffid0 = -1;
    pfinf[Funcs.top()].elems.push_back(elems.size());
    id0 = alNums.top();
    alNums.pop();
    if (table_ainf.count(id0))
    {
        diffid0 = alNums.top();
        alNums.pop();
        id0 = ainf[synb[table_ainf[id0]].addr].low;
    }
    elems.push_back(elem("in", diffid0, -1, id0));
}
void ParamTable::exOut()
{
#ifdef DEBUG
    cout << "exOut" << endl;
#endif
    int id0, diffid0 = -1;
    pfinf[Funcs.top()].elems.push_back(elems.size());
    id0 = alNums.top();
    alNums.pop();
    if (table_ainf.count(id0))
    {
        diffid0 = alNums.top();
        alNums.pop();
        id0 = ainf[synb[table_ainf[id0]].addr].low;
    }
    elems.push_back(elem("out", diffid0, -1, id0));
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
    inalNums[id_tmp] = 1;
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
    int id1, diffid1 = -1;
    id1 = alNums.top();
    alNums.pop();
    if (table_ainf.count(id1))
    {
        diffid1 = alNums.top();
        alNums.pop();
        id1 = ainf[synb[table_ainf[id1]].addr].low;
    }
    int num;
    num = callParams.top();
    callParams.pop();
    callParams.push(++num);
    if(diffid1!=-1)
    {
        pfinf[Funcs.top()].elems.push_back(elems.size());
        elems.push_back(elem("arr1", -1, -1, diffid1));
    }
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
            /*if (synb[pfinf[k].synbs[i]].cat == 'l')
            {
                sum += 2 * (ainf[synb[pfinf[k].synbs[i]].addr].up - ainf[synb[pfinf[k].synbs[i]].addr].low);
            }*/
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
    vector<int> tmpelems;
    int iscons[1000], used[1000], lastmodify[1000]; //是否现在是常数/是否被使用了（否则就是垃圾语句）/上次修改位置
    map<pair<int, int>, pair<int, int>> oldtonew;   //<id1,id2>-><id0,location>
    map<int, pair<int, int>> newtoold;
    elem tmpelem("", -1, -1, -1);
    int piecel, piecer, l, r, num;
    for (k = 1; k < pfinf.size(); k++) //常数表达式优化(含常数推进)
    {
        memset(iscons, 255, sizeof(iscons));
        for (i = 0; i < pfinf[k].elems.size(); i++)
        {
            tmpelem = elems[pfinf[k].elems[i]];
            if (tmpelem.iscntop())
            {
                if ((table_cons.count(tmpelem.id1) || iscons[tmpelem.id1] != -1) && (table_cons.count(tmpelem.id2) || iscons[tmpelem.id2] != -1))
                {
                    if (iscons[tmpelem.id1] != -1)
                        l = iscons[tmpelem.id1];
                    else
                        l = table_cons[tmpelem.id1];
                    if (iscons[tmpelem.id2] != -1)
                        r = iscons[tmpelem.id2];
                    else
                        r = table_cons[tmpelem.id2];
                    if (tmpelem.st == "+")
                        num = l + r;
                    else if (tmpelem.st == "-")
                        num = l - r;
                    else if (tmpelem.st == "*")
                        num = l * r;
                    else if (tmpelem.st == "/")
                        num = l / r;
                    elems[pfinf[k].elems[i]] = elem("=", id_tmp, -1, tmpelem.id0);
                    addCon(id_tmp, num);
                    id_tmp++;
                    iscons[tmpelem.id0] = num;
                }
                else
                    iscons[tmpelem.id0] = -1;
            }
            else
            {
                if (tmpelem.st == "=")
                {
                    if (table_cons.count(tmpelem.id1) || iscons[tmpelem.id1] != -1)
                    {
                        if (iscons[tmpelem.id1] != -1)
                            l = iscons[tmpelem.id1];
                        else
                            l = table_cons[tmpelem.id1];
                        iscons[tmpelem.id0] = l;
                    }
                    else
                    {
                        iscons[tmpelem.id0] = -1;
                    }
                }
                else
                    memset(iscons, 255, sizeof(iscons));
            }
        }
    }
    for (k = 1; k < pfinf.size(); k++) //dag优化
    {
        memset(used, 255, sizeof(used));
        memset(lastmodify, 255, sizeof(lastmodify));
        oldtonew.clear();
        newtoold.clear();
        for (i = 0; i < pfinf[k].elems.size(); i++)
        {
            tmpelem = elems[pfinf[k].elems[i]];
            if (tmpelem.iscntop() || tmpelem.st == "=" || tmpelem.st == "arr1" || tmpelem.st == "arr2")
            {
                if (tmpelem.iscntop())
                {
                    if (tmpelem.id1 != tmpelem.id0 && tmpelem.id2 != tmpelem.id0 && lastmodify[tmpelem.id0] != -1 && used[tmpelem.id0] < lastmodify[tmpelem.id0]) //如果上次修改后到现在未使用过，删除上次对应的四元式
                    {
                        elems[pfinf[k].elems[lastmodify[tmpelem.id0]]] = elem("deleted", -1, -1, tmpelem.id0);
                        oldtonew.erase(make_pair(tmpelem.id1, tmpelem.id2));
                        if (tmpelem.st == "+" || tmpelem.st == "*")
                            oldtonew.erase(make_pair(tmpelem.id2, tmpelem.id1));
                    }
                    if (oldtonew.count(make_pair(tmpelem.id1, tmpelem.id2))) //之前有过同样的表达式且目标变量未改变过时直接复用
                    {
                        int location = oldtonew[make_pair(tmpelem.id1, tmpelem.id2)].second;
                        int newid = oldtonew[make_pair(tmpelem.id1, tmpelem.id2)].first;
                        if (location > lastmodify[tmpelem.id1] && location > lastmodify[tmpelem.id2])
                        {
                            elems[pfinf[k].elems[i]] = elem("=", newid, -1, tmpelem.id0);
                            used[newid] = i;
                            lastmodify[tmpelem.id0] = i;
                        }
                    }
                    else
                    {
                        used[tmpelem.id1] = used[tmpelem.id2] = i;
                        lastmodify[tmpelem.id0] = i;
                        oldtonew.insert(make_pair(make_pair(tmpelem.id1, tmpelem.id2), make_pair(tmpelem.id0, i)));
                        if (tmpelem.st == "+" || tmpelem.st == "*")
                            oldtonew.insert(make_pair(make_pair(tmpelem.id2, tmpelem.id1), make_pair(tmpelem.id0, i)));
                    }
                }
                else if (tmpelem.st == "=")
                {
                    if (lastmodify[tmpelem.id0] != -1 && used[tmpelem.id0] < lastmodify[tmpelem.id0]) //如果上次修改后到现在未使用过，删除上次对应的四元式
                    {
                        elems[pfinf[k].elems[lastmodify[tmpelem.id0]]] = elem("deleted", tmpelem.id1, -1, tmpelem.id0);
                    }
                    used[tmpelem.id1] = i;
                    lastmodify[tmpelem.id0] = i;
                }
                else
                {
                    used[tmpelem.id0] = i;
                }
            }
            else
            {
                memset(used, 255, sizeof(used));
                memset(lastmodify, 255, sizeof(lastmodify));
                oldtonew.clear();
                newtoold.clear();
            }
        }
    }
    int idx;
    for (i = 0; i < elems.size(); i++) //初始化活跃信息表
    {
        qt[0].push_back(true);
        qt[1].push_back(true);
        qt[2].push_back(true);
    }
    for (k = 1; k < pfinf.size(); k++) //计算活跃信息
    {
        for (i = 0; i < pfinf[k].elems.size(); i++)
        {
            tmpelem = elems[pfinf[k].elems[i]];
            idx = pfinf[k].elems[i];
            if (tmpelem.iscntop() || tmpelem.st == "=" || tmpelem.st == "arr1" || tmpelem.st == "arr2")
            {
                if (tmpelem.iscntop())
                {
                    qt[0][idx] = symbl[tmpelem.id0];
                    symbl[tmpelem.id0] = false;
                    qt[1][idx] = symbl[tmpelem.id1];
                    symbl[tmpelem.id1] = true;
                    qt[2][idx] = symbl[tmpelem.id2];
                    symbl[tmpelem.id2] = true;
                }
                else if (tmpelem.st == "=")
                {
                    qt[0][idx] = symbl[tmpelem.id0];
                    symbl[tmpelem.id0] = false;
                    qt[1][idx] = symbl[tmpelem.id1];
                    symbl[tmpelem.id1] = true;
                }
                else
                {
                    qt[0][idx] = symbl[tmpelem.id0];
                    symbl[tmpelem.id0] = true;
                }
            }
            else
            {
                memset(symbl, 0, sizeof(symbl));
                for (j = 0; j < 100; j++)
                    symbl[i] = true;
            }
        }
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
void ParamTable::toax(int k, int id, int diffid) //输出到从内存提取到ax的汇编指令
{
    if (locra == id && locradiff == diffid) //考虑要找的数据已经在寄存器的情况
        return;
    if (locrc == id && locrcdiff == diffid)
    {
        locra = id;
        locradiff = diffid;
        assemblyRes.push_back("MOV   AX,CX");
        return;
    }
    locra = id;
    locradiff = diffid;
    if (table_cons.count(id)) //为常数
    {
        //cout << "MOV   AX," << table_cons[id] << endl;
        assemblyRes.push_back("MOV   AX," + to_string(table_cons[id]));
    }
    else if (basicValls[k].var.count(id)) //考虑是局部变量还是全局变量
    {
        id = basicValls[k].var[id];
        if (diffid == -1)
            //cout << "MOV   AX,[BP-" << id + 2 << "]" << endl;
            assemblyRes.push_back("MOV   AX,[BP-" + to_string(id + 2) + "]");
        else //为数组，考虑偏移量是否是全局变量
        {
            if (table_cons.count(diffid))
            {
                //cout << "MOV   SI," << table_cons[diffid] << endl;
                assemblyRes.push_back("MOV   SI," + to_string(table_cons[diffid]));
            }
            else if (basicValls[0].var.count(diffid))
            {
                diffid = basicValls[0].var[diffid];
                //cout << "MOV   SI,[BX+" << diffid << "]" << endl;
                assemblyRes.push_back("MOV   SI,[BX+" + to_string(diffid) + "]");
            }
            else
            {
                diffid = basicValls[k].var[diffid];
                //cout << "MOV   SI,[BP-" << diffid + 2 << "]" << endl;
                assemblyRes.push_back("MOV   SI,[BP-" + to_string(diffid + 2) + "]");
            }
            //cout << "SHL   SI,1" << endl;
            //cout << "NEG   SI" << endl;
            //cout << "MOV   AX,[BP+SI-" << id + 2 << "]" << endl;
            assemblyRes.push_back("SHL   SI,1");
            assemblyRes.push_back("NEG   SI");
            assemblyRes.push_back("MOV   AX,[BP+SI-" + to_string(id + 2) + "]");
        }
    }
    else
    {
        id = basicValls[0].var[id];
        if (diffid == -1)
            //cout << "MOV   AX,[BX+" << id << "]" << endl;
            assemblyRes.push_back("MOV   AX,[BX+" + to_string(id) + "]");
        else //为数组，考虑偏移量是否是全局变量
        {
            if (table_cons.count(diffid))
            {
                //cout << "MOV   DI," << table_cons[diffid] << endl;
                assemblyRes.push_back("MOV   DI," + to_string(table_cons[diffid]));
            }
            else if (basicValls[0].var.count(diffid))
            {
                diffid = basicValls[0].var[diffid];
                //cout << "MOV   DI,[BX+" << diffid << "]" << endl;
                assemblyRes.push_back("MOV   DI,[BX+" + to_string(diffid) + "]");
            }
            else
            {
                diffid = basicValls[k].var[diffid];
                //cout << "MOV   DI,[BP-" << diffid + 2 << "]" << endl;
                assemblyRes.push_back("MOV   DI,[BP-" + to_string(diffid + 2) + "]");
            }
            //cout << "SHL   DI,1" << endl;
            //cout << "MOV   AX,[BX+DI+" << id << "]" << endl;
            assemblyRes.push_back("SHL   DI,1");
            assemblyRes.push_back("MOV   AX,[BX+DI+" + to_string(id) + "]");
        }
    }
}
void ParamTable::tocx(int k, int id, int diffid) //输出到从内存提取到ax的汇编指令
{
    if (locrc == id && locrcdiff == diffid) //考虑要找的数据已经在寄存器的情况
        return;
    if (locra == id && locradiff == diffid)
    {
        locrc = id;
        locrcdiff = diffid;
        assemblyRes.push_back("MOV   CX,AX");
        return;
    }
    locrc = id;
    locrcdiff = diffid;
    if (table_cons.count(id))
    {
        //cout << "MOV   CX," << table_cons[id] << endl;
        assemblyRes.push_back("MOV   CX," + to_string(table_cons[id]));
    }
    else if (basicValls[k].var.count(id)) //考虑是局部变量还是全局变量
    {
        id = basicValls[k].var[id];
        if (diffid == -1)
            //cout << "MOV   CX,[BP-" << id + 2 << "]" << endl;
            assemblyRes.push_back("MOV   CX,[BP-" + to_string(id + 2) + "]");
        else //为数组，考虑偏移量是否是全局变量
        {
            if (table_cons.count(diffid))
            {
                //cout << "MOV   SI," << table_cons[diffid] << endl;
                assemblyRes.push_back("MOV   SI," + to_string(table_cons[diffid]));
            }
            else if (basicValls[0].var.count(diffid))
            {
                diffid = basicValls[0].var[diffid];
                //cout << "MOV   SI,[BX+" << diffid << "]" << endl;
                assemblyRes.push_back("MOV   SI,[BX+" + to_string(diffid) + "]");
            }
            else
            {
                diffid = basicValls[k].var[diffid];
                //cout << "MOV   SI,[BP-" << diffid + 2 << "]" << endl;
                assemblyRes.push_back("MOV   SI,[BP-" + to_string(diffid + 2) + "]");
            }
            //cout << "SHL   SI,1" << endl;
            //cout << "NEG   SI" << endl;
            //cout << "MOV   CX,[BP+SI-" << id + 2 << "]" << endl;
            assemblyRes.push_back("SHL   SI,1");
            assemblyRes.push_back("NEG   SI");
            assemblyRes.push_back("MOV   CX,[BP+SI-" + to_string(id + 2) + "]");
        }
    }
    else
    {
        id = basicValls[0].var[id];
        if (diffid == -1)
            //cout << "MOV   CX,[BX+" << id << "]" << endl;
            assemblyRes.push_back("MOV   CX,[BX+" + to_string(id) + "]");
        else //为数组，考虑偏移量是否是全局变量
        {
            if (table_cons.count(diffid))
            {
                //cout << "MOV   DI," << table_cons[diffid] << endl;
                assemblyRes.push_back("MOV   DI," + to_string(table_cons[diffid]));
            }
            else if (basicValls[0].var.count(diffid))
            {
                diffid = basicValls[0].var[diffid];
                //cout << "MOV   DI,[BX+" << diffid << "]" << endl;
                assemblyRes.push_back("MOV   DI,[BX+" + to_string(diffid) + "]");
            }
            else
            {
                diffid = basicValls[k].var[diffid];
                //cout << "MOV   DI,[BP-" << diffid + 2 << "]" << endl;
                assemblyRes.push_back("MOV   DI,[BP-" + to_string(diffid + 2) + "]");
            }
            //cout << "SHL   DI,1" << endl;
            //cout << "MOV   CX,[BX+DI+" << id << "]" << endl;
            assemblyRes.push_back("SHL   DI,1");
            assemblyRes.push_back("MOV   CX,[BX+DI+" + to_string(id) + "]");
        }
    }
}
void ParamTable::axto(int k, int id, int diffid) //输出到从ax提取到内存的汇编指令
{
    if (table_cons.count(id))
    {
        //cout << "MOV   CX," << table_cons[id] << endl;
        assemblyRes.push_back("MOV   CX," + to_string(table_cons[id]));
    }
    else if (basicValls[k].var.count(id)) //考虑是局部变量还是全局变量
    {
        id = basicValls[k].var[id];
        if (diffid == -1)
            //cout << "MOV   [BP-" << id + 2 << "],AX" << endl;
            assemblyRes.push_back("MOV   [BP-" + to_string(id + 2) + "],AX");
        else //为数组，考虑偏移量是否是全局变量
        {
            if (table_cons.count(diffid))
            {
                //cout << "MOV   SI," << table_cons[diffid] << endl;
                assemblyRes.push_back("MOV   SI," + to_string(table_cons[diffid]));
            }
            else if (basicValls[0].var.count(diffid))
            {
                diffid = basicValls[0].var[diffid];
                //cout << "MOV   SI,[BX+" << diffid << "]" << endl;
                assemblyRes.push_back("MOV   SI,[BX+" + to_string(diffid) + "]");
            }
            else
            {
                diffid = basicValls[k].var[diffid];
                //cout << "MOV   SI,[BP-" << diffid + 2 << "]" << endl;
                assemblyRes.push_back("MOV   SI,[BP-" + to_string(diffid + 2) + "]");
            }
            //cout << "SHL   SI,1" << endl;
            //cout << "NEG   SI" << endl;
            //cout << "MOV   [BP+SI-" << id + 2 << "],AX" << endl;
            assemblyRes.push_back("SHL   SI,1");
            assemblyRes.push_back("NEG   SI");
            assemblyRes.push_back("MOV   [BP+SI-" + to_string(id + 2) + "],AX");
        }
    }
    else
    {
        id = basicValls[0].var[id];
        if (diffid == -1)
            //cout << "MOV   [BX+" << id << "],AX" << endl;
            assemblyRes.push_back("MOV   [BX+" + to_string(id) + "],AX");
        else //为数组，考虑偏移量是否是全局变量
        {
            if (table_cons.count(diffid))
            {
                //cout << "MOV   DI," << table_cons[diffid] << endl;
                assemblyRes.push_back("MOV   DI," + to_string(table_cons[diffid]));
            }
            else if (basicValls[0].var.count(diffid))
            {
                diffid = basicValls[0].var[diffid];
                //cout << "MOV   DI,[BX+" << diffid << "]" << endl;
                assemblyRes.push_back("MOV   DI,[BX+" + to_string(diffid) + "]");
            }
            else
            {
                diffid = basicValls[k].var[diffid];
                //cout << "MOV   DI,[BP-" << diffid + 2 << "]" << endl;
                assemblyRes.push_back("MOV   DI,[BP-" + to_string(diffid + 2) + "]");
            }
            //cout << "SHL   DI,1" << endl;
            //cout << "MOV   [BX+DI+" << id << "],AX" << endl;
            assemblyRes.push_back("SHL   DI,1");
            assemblyRes.push_back("MOV   [BX+DI+" + to_string(id) + "],AX");
        }
    }
}
void ParamTable::jgjp(int k, string st, int tg) //符合条件则跳转
{
    if (st == "jmp")
    {
        //cout << "JMP   F" << k << "T" << tg << endl;
        assemblyRes.push_back("JMP   F" + to_string(k) + "T" + to_string(tg));
    }
    else if (st == "==")
    {
        //cout << "JE    F" << k << "T" << tg << endl;
        assemblyRes.push_back("JE    F" + to_string(k) + "T" + to_string(tg));
    }
    else if (st == "!=")
    {
        //cout << "JNE    F" << k << "T" << tg << endl;
        assemblyRes.push_back("JNE    F" + to_string(k) + "T" + to_string(tg));
    }
    else if (st == ">")
    {
        //cout << "JA    F" << k << "T" << tg << endl;
        assemblyRes.push_back("JA    F" + to_string(k) + "T" + to_string(tg));
    }
    else if (st == "<=")
    {
        //cout << "JNA   F" << k << "T" << tg << endl;
        assemblyRes.push_back("JNA   F" + to_string(k) + "T" + to_string(tg));
    }
    else if (st == "<")
    {
        //cout << "JB    F" << k << "T" << tg << endl;
        assemblyRes.push_back("JB    F" + to_string(k) + "T" + to_string(tg));
    }
    else if (st == ">=")
    {
        //cout << "JNB   F" << k << "T" << tg << endl;
        assemblyRes.push_back("JNB   F" + to_string(k) + "T" + to_string(tg));
    }
}
void ParamTable::genAssembly()
{
    int i, j, k, id0, id1, id2, tmpjmp = -1, tmp, tmpsynb, diffid1 = -1, diffid2 = -1;
    string tmpst;
    elem tmpelem("", -1, -1, -1);
    //cout << "SSEG  SEGMENT" << endl;
    //cout << "SKTOP DW 200 DUP(?)" << endl;
    //cout << "SSEG  ENDS" << endl;
    //cout << "DSEG  SEGMENT" << endl;
    //cout << "MAINV DW " << basicValls[0].size << " DUP(?)" << endl;
    //cout << "DSEG  ENDS" << endl;
    //cout << "CSEG  SEGMENT" << endl;
    //cout << "      ASSUME  SS:SSEG,CS:CSEG,DS:DSEG" << endl;
    assemblyRes.push_back("SSEG  SEGMENT");
    assemblyRes.push_back("SKTOP DW 200 DUP(?)");
    assemblyRes.push_back("SSEG  ENDS");
    assemblyRes.push_back("DSEG  SEGMENT");
    assemblyRes.push_back("MAINV DW " + to_string(basicValls[0].size) + " DUP(?)");
    assemblyRes.push_back("DSEG  ENDS");
    assemblyRes.push_back("CSEG  SEGMENT");
    assemblyRes.push_back("      ASSUME  SS:SSEG,CS:CSEG,DS:DSEG");
    // 输入输出子程序
    assemblyRes.push_back("INFUN  PROC NEAR");
    assemblyRes.push_back("PUSH  BX");
    assemblyRes.push_back("PUSH  CX");
    assemblyRes.push_back("MOV   DX,0");
    assemblyRes.push_back("MOV   CX,0");
    assemblyRes.push_back("INFUN1:MOV	  AH,01H");
    assemblyRes.push_back("INT   21H");
    assemblyRes.push_back("CMP   AL,48");
    assemblyRes.push_back("JB    INFUN2");
    assemblyRes.push_back("SUB   AL,48");
    assemblyRes.push_back("MOV   CL,AL");
    assemblyRes.push_back("MOV	  AX,DX");
    assemblyRes.push_back("MOV   DX,10");
    assemblyRes.push_back("MUL   DX");
    assemblyRes.push_back("ADD   AX,CX");
    assemblyRes.push_back("MOV   DX,AX");
    assemblyRes.push_back("JMP   INFUN1");
    assemblyRes.push_back("INFUN2:MOV   AX,DX");
    assemblyRes.push_back("POP   CX");
    assemblyRes.push_back("POP   BX");
    assemblyRes.push_back("RET");
    assemblyRes.push_back("INFUN  ENDP");
    assemblyRes.push_back("OUTFUN PROC NEAR");
    assemblyRes.push_back("PUSH  AX");
    assemblyRes.push_back("PUSH  BX");
    assemblyRes.push_back("PUSH  CX");
    assemblyRes.push_back("MOV   BX,10");
    assemblyRes.push_back("MOV   CX,0");
    assemblyRes.push_back("OUTFUN1:MOV   DX,0");
    assemblyRes.push_back("DIV   BX");
    assemblyRes.push_back("ADD   DL,30H");
    assemblyRes.push_back("PUSH  DX");
    assemblyRes.push_back("INC   CX");
    assemblyRes.push_back("AND   AX,AX");
    assemblyRes.push_back("JZ    OUTFUN2");
    assemblyRes.push_back("JMP   OUTFUN1");
    assemblyRes.push_back("OUTFUN2:POP   DX");
    assemblyRes.push_back("MOV   AH,02H");
    assemblyRes.push_back("INT   21H");
    assemblyRes.push_back("LOOP  OUTFUN2");
    assemblyRes.push_back("MOV   DX,0AH");
    assemblyRes.push_back("MOV   AH,02H");
    assemblyRes.push_back("INT   21H");
    assemblyRes.push_back("POP   CX");
    assemblyRes.push_back("POP   BX");
    assemblyRes.push_back("POP   AX");
    assemblyRes.push_back("RET");
    assemblyRes.push_back("OUTFUN ENDP");
    for (k = 1; k < pfinf.size(); k++)
    {
        locra = locrc = locradiff = locrcdiff = -1;
        if (k != pfinf.size() - 1)
        {
            //cout << "FUN" << k << "  PROC NEAR" << endl;
            //cout << "MOV   CX," << (basicValls[k].size - basicValls[k].parsize - 2) / 2 << endl;
            //cout << "F" << k << "IN: NOP" << endl;
            //cout << "PUSH  AX" << endl; //存临时变量和参数
            //cout << "LOOP  F" << k << "IN" << endl;
            //cout << "MOV   BP,SI" << endl;
            //cout << "PUSH  BP" << endl;
            assemblyRes.push_back("FUN" + to_string(k) + "  PROC NEAR");
            assemblyRes.push_back("MOV   CX," + to_string((basicValls[k].size - basicValls[k].parsize - 2) / 2));
            assemblyRes.push_back("F" + to_string(k) + "IN: NOP");
            assemblyRes.push_back("PUSH  AX");
            assemblyRes.push_back("LOOP  F" + to_string(k) + "IN");
            assemblyRes.push_back("MOV   BP,SI");
            assemblyRes.push_back("PUSH  BP");
        }
        else
        {
            //cout << "START:MOV   AX,DSEG" << endl;
            //cout << "MOV   DS,AX" << endl;
            //cout << "MOV   AX,SSEG" << endl;
            //cout << "MOV   SS,AX" << endl;
            //cout << "MOV   BP,SP" << endl;           //BP：当前函数栈顶
            //cout << "MOV   BX,OFFSET MAINV" << endl; //DI：全局变量
            //cout << "MOV   AX,0" << endl;
            //cout << "MOV   CX," << basicValls[k].size / 2 << endl;
            //cout << "F" << k << "IN: NOP" << endl;
            //cout << "PUSH  AX" << endl; //存临时变量和参数
            //cout << "LOOP  F" << k << "IN" << endl;
            //cout << "PUSH  BP" << endl; //存bp
            assemblyRes.push_back("START:MOV   AX,DSEG");
            assemblyRes.push_back("MOV   DS,AX");
            assemblyRes.push_back("MOV   AX,SSEG");
            assemblyRes.push_back("MOV   SS,AX");
            assemblyRes.push_back("MOV   BP,SP");
            assemblyRes.push_back("MOV   BX,OFFSET MAINV");
            assemblyRes.push_back("MOV   AX,0");
            assemblyRes.push_back("MOV   CX," + to_string(basicValls[k].size / 2));
            assemblyRes.push_back("F" + to_string(k) + "IN: NOP");
            assemblyRes.push_back("PUSH  AX");
            assemblyRes.push_back("LOOP  F" + to_string(k) + "IN");
            assemblyRes.push_back("PUSH  BP");
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
                //cout << "F" << k << "T" << pfinf[k].elems[i] << ": NOP" << endl;
                assemblyRes.push_back("F" + to_string(k) + "T" + to_string(pfinf[k].elems[i]) + ": NOP");
            if (tmpelem.st == "=")
            {
#ifdef DEBUG
                cout << tmpelem.id1 << ' ' << locra << ' ' << diffid1 << ' ' << locradiff << endl;
#endif
                if (!(tmpelem.id1 == locra && diffid1 == locradiff))
                    toax(k, tmpelem.id1, diffid1);
                axto(k, tmpelem.id0, diffid2);
                diffid1 = -1;
                diffid2 = -1;
            }
            else if (tmpelem.iscntop())
            {
                toax(k, tmpelem.id1, diffid1);
                tocx(k, tmpelem.id2, diffid2);
                diffid1 = -1;
                diffid2 = -1;
                if (tmpelem.st == "+")
                {
                    //cout << "ADD   AX,CX" << endl;
                    assemblyRes.push_back("ADD   AX,CX");
                }
                else if (tmpelem.st == "-")
                {
                    //cout << "SUB   AX,CX" << endl;
                    assemblyRes.push_back("SUB   AX,CX");
                }
                else if (tmpelem.st == "*")
                {
                    //cout << "MUL   CX" << endl;
                    assemblyRes.push_back("MUL   CX");
                }
                else if (tmpelem.st == "/")
                {
                    //cout << "MOV   DX,0" << endl;
                    //cout << "DIV   CX" << endl;
                    assemblyRes.push_back("MOV   DX,0");
                    assemblyRes.push_back("DIV   CX");
                }
                else if (tmpelem.st == "%")
                {
                    //cout << "DIV   CX" << endl;
                    //cout << "MOV   AX,DX" << endl;
                    assemblyRes.push_back("DIV   CX");
                    assemblyRes.push_back("MOV   AX,DX");
                }
                locra = tmpelem.id0;
                locradiff = -1;
                axto(k, tmpelem.id0);
            }
            else if (tmpelem.isjugop())
            {
                toax(k, tmpelem.id1, diffid1);
                diffid1 = -1;
                tocx(k, tmpelem.id2, diffid2);
                diffid2 = -1;
                //cout << "CMP   AX,CX" << endl;
                //cout << "MOV   AX,1" << endl;
                assemblyRes.push_back("CMP   AX,CX");
                assemblyRes.push_back("MOV   AX,1");
                jgjp(0, tmpelem.st, ++tmpjmp);
                //cout << "MOV   AX,0" << endl;
                //cout << "F0T" << tmpjmp << ": NOP" << endl;
                assemblyRes.push_back("MOV   AX,0");
                assemblyRes.push_back("F0T" + to_string(tmpjmp) + ": NOP");
                axto(k, tmpelem.id0);
            }
            else if (tmpelem.st == "if")
            {
                toax(k, tmpelem.id1);
                //cout << "CMP   AX,0" << endl;
                assemblyRes.push_back("CMP   AX,0");
                locra = locrc = locradiff = locrcdiff = -1;
                jgjp(k, "==", tmpelem.id0);
            }
            else if (tmpelem.st == "ie")
            {
                locra = locrc = locradiff = locrcdiff = -1;
            }
            else if (tmpelem.st == "el")
            {
                locra = locrc = locradiff = locrcdiff = -1;
                jgjp(k, "jmp", tmpelem.id0);
            }
            else if (tmpelem.st == "wh")
            {
                locra = locrc = locradiff = locrcdiff = -1;
            }
            else if (tmpelem.st == "do")
            {
                toax(k, tmpelem.id1);
                //cout << "CMP   AX,0" << endl;
                locra = locrc = locradiff = locrcdiff = -1;
                assemblyRes.push_back("CMP   AX,0");
                jgjp(k, "==", tmpelem.id0);
            }
            else if (tmpelem.st == "we")
            {
                locra = locrc = locradiff = locrcdiff = -1;
                jgjp(k, "jmp", tmpelem.id0);
            }
            else if (tmpelem.st == "call")
            {
                //cout << "MOV   SI,SP" << endl;
                assemblyRes.push_back("MOV   SI,SP");
                tmp = table_pfinf[tmpelem.id1];
                tmp = synb[tmp].addr;
                tmpsynb = tmpelem.id2;

            }
            else if (tmpelem.st == "params")
            {
                toax(k, tmpelem.id0, diffid1);
                diffid1 = -1;
                //cout << "PUSH  AX" << endl;
                assemblyRes.push_back("PUSH  AX");
            }
            else if (tmpelem.st == "callend")
            {
                //cout << "CALL  FUN" << tmp << endl;
                //cout << "POP   AX" << endl;
                locra = locrc = locradiff = locradiff = -1;
                assemblyRes.push_back("CALL  FUN" + to_string(tmp));
                assemblyRes.push_back("POP   AX");
                axto(k, tmpsynb);
            }
            else if (tmpelem.st == "retnum")
            {
                if (k != pfinf.size() - 1)
                {
                    toax(k, tmpelem.id0);
                    //cout << "JMP   F" << k << "O" << endl;
                    assemblyRes.push_back("JMP   F" + to_string(k) + "O");
                }
                else
                {
                    //cout << "MOV   AH,4CH" << endl;
                    //cout << "INT   21H" << endl;
                    assemblyRes.push_back("MOV   AH,4CH");
                    assemblyRes.push_back("INT   21H");
                }
            }
            else if (tmpelem.st == "ret")
            { //此处需要照retnum修改
                if (k != pfinf.size() - 1)
                {
                    //cout << "JMP   F" << k << "O" << endl;
                    assemblyRes.push_back("JMP   F" + to_string(k) + "O");
                }
                else
                {
                    //cout << "MOV   AH,4CH" << endl;
                    //cout << "INT   21H" << endl;
                    assemblyRes.push_back("MOV   AH,4CH");
                    assemblyRes.push_back("INT   21H");
                }
            }
            else if (tmpelem.st == "arr1")
            {
                diffid1 = tmpelem.id0;
            }
            else if (tmpelem.st == "arr2")
            {
                diffid2 = tmpelem.id0;
            }
            else if (tmpelem.st == "in")
            {
                assemblyRes.push_back("CALL INFUN");
                locra = locradiff = -1;
                axto(k, tmpelem.id0, tmpelem.id1);
                locra = tmpelem.id0;
                locradiff = tmpelem.id1;
            }
            else if (tmpelem.st == "out")
            {
                toax(k, tmpelem.id0, tmpelem.id1);
                assemblyRes.push_back("CALL OUTFUN");
            }
        }
        if (k != pfinf.size() - 1)
        {
            //cout << "F" << k << "O:  NOP" << endl;
            //cout << "MOV   CX," << (basicValls[k].size - basicValls[k].parsize) / 2 << endl;
            //cout << "F" << k << "O1:NOP" << endl;
            //cout << "POP   DX" << endl; //释放临时变量栈空间（size-psize-2)+旧BP（2）
            //cout << "LOOP  F" << k << "O1" << endl;
            //cout << "POP   SI" << endl; //取出IP
            //cout << "MOV   CX," << basicValls[k].parsize / 2 << endl;
            //cout << "F" << k << "O2:NOP" << endl;
            //cout << "POP   DX" << endl; //释放参数栈空间
            //cout << "LOOP  F" << k << "O2" << endl;
            //cout << "POP   BP" << endl;
            //cout << "PUSH  BP" << endl;
            //cout << "PUSH  AX" << endl;
            //cout << "PUSH  SI" << endl; //存回IP
            //cout << "RET" << endl;
            //cout << "FUN" << k << "  ENDP" << endl;
            assemblyRes.push_back("F" + to_string(k) + "O:  NOP");
            assemblyRes.push_back("MOV   CX," + to_string((basicValls[k].size - basicValls[k].parsize) / 2));
            assemblyRes.push_back("F" + to_string(k) + "O1:NOP");
            assemblyRes.push_back("POP   DX");
            assemblyRes.push_back("LOOP  F" + to_string(k) + "O1");
            assemblyRes.push_back("POP   SI");
            assemblyRes.push_back("MOV   CX," + to_string(basicValls[k].parsize / 2));
            assemblyRes.push_back("F" + to_string(k) + "O2:NOP");
            assemblyRes.push_back("POP   DX");
            assemblyRes.push_back("LOOP  F" + to_string(k) + "O2");
            assemblyRes.push_back("POP   BP");
            assemblyRes.push_back("PUSH  BP");
            assemblyRes.push_back("PUSH  AX");
            assemblyRes.push_back("PUSH  SI");
            assemblyRes.push_back("RET");
            assemblyRes.push_back("FUN" + to_string(k) + "  ENDP");
        }
        else
        {
            //cout << "MOV   AH,4CH" << endl;
            //cout << "INT   21H" << endl;
            //cout << "CSEG  ENDS" << endl;
            //cout << "      END   START" << endl;
            assemblyRes.push_back("MOV   AH,4CH");
            assemblyRes.push_back("INT   21H");
            assemblyRes.push_back("CSEG  ENDS");
            assemblyRes.push_back("      END   START");
        }
        locra = locrc = locradiff = locrcdiff = -1;
    }
    valls.clear();
}

void ParamTable::outputAssembly()
{
    cout << "outputAssembly now" << endl;
    for (int i = 0; i < assemblyRes.size(); i++)
    {
        cout << assemblyRes.at(i) << endl;
    }
}
