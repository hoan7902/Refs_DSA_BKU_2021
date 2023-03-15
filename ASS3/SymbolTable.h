#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Data
{
public:
    string name;
    string type;
    string para;
    bool isFun = 0;
    int lendata = 0;
    int scope = 0;
    int slot = 0;
    Data() {}
    ~Data(){};
};
bool isASSIGN(string s) 
{
    return (regex_match(s, regex("^ASSIGN [a-z][a-zA-Z0-9_]* ((([a-z][a-zA-Z0-9_]*(([(][)])|([(]((([0-9]+,)|('[0-9a-zA-Z ]+',)|(([a-z][a-zA-Z0-9_]*),))*)(([0-9]+)|('[0-9a-zA-Z ]+')|([a-z][a-zA-Z0-9_]*))[)]))))|(('[0-9a-zA-Z ]+')|([0-9]+))|([a-z][a-zA-Z0-9_]*))")));
}
bool isINSERT(string s) 
{
    return (regex_match(s, regex("^INSERT ([a-z][a-zA-Z0-9_]*|[a-z])( \\d+)?")));
}
bool isLINEAR(string s)
{
    return (regex_match(s, regex("^LINEAR \\d+ \\d+")));
}

bool isQUADRATIC(string s) 
{
    return (regex_match(s, regex("^QUADRATIC \\d+ \\d+ \\d+")));
}

bool isDOUBLE(string s) 
{
    return (regex_match(s, regex("^DOUBLE \\d+ \\d+")));
}

bool isLOOKUP(string s) 
{
    return (regex_match(s, regex("^LOOKUP [a-z][a-zA-Z0-9_]*|[a-z]")));
}
bool isCALL(string s) 
{
    return (regex_match(s, regex("^CALL ((([a-z][a-zA-Z0-9_]*(([(][)])|([(]((([0-9]+,)|('[0-9a-zA-Z ]+',)|(([a-z][a-zA-Z0-9_]*),))*)(([0-9]+)|('[0-9a-zA-Z ]+')|([a-z][a-zA-Z0-9_]*))[)])))))")));
}
Data** createTable(int cap)
{
    Data** temp = new Data*[cap];
    for (int i = 0; i < cap; i++)
    {   
        temp[i] = NULL;
    }
    return temp;
}
int LOOKUP(Data** table, int cap, string name_exam)
{
    int arr[10001];
    int run = 0;
    for (int i = 0; i < cap; i++)
    {
        if (table[i] && table[i]->name == name_exam)
        {
            arr[run] = i;
            run += 1;
        }
    }
    if (run == 0) return -1;
    int max_scope = arr[0];
    for (int i = 1; i < run; i++)
    {
        if (table[arr[i]]->scope > table[max_scope]->scope)
        {
            max_scope = arr[i];
        }
    }
    return max_scope;
}
void split_DetecMethod(string s, int &m, int &c1, int &c2)
{
    int space1 = s.find(" ");
    int space2 = s.find(" ", space1 + 1);
    m = stoi(s.substr(space1 + 1, space2 - space1 + 1));
    int space3 = s.find(" ", space2 + 1);
    if (space3 != (int)string :: npos)
    {
        c1 = stoi(s.substr(space2 + 1, space3 - space2 + 1));
        c2 = stoi(s.substr(space3 + 1));
    }
    else 
    {
        c1= stoi(s.substr(space2 + 1));
    }
}
long long getKey(Data data)
{
    string res = "";
    res += to_string(data.scope);
    for (int i = 0; i < (int)data.name.length(); i++)
    {
        res += to_string((int)data.name[i] -  48);
    }
    return stoll(res);
}
void splitInsert(string s, Data &tempData, int scope)
{   
    tempData.scope = scope;
    int space2 = s.find(" ", 7);
    if (space2 != (int)string :: npos)
    {
        tempData.name = s.substr(7, space2 - 7);
        tempData.isFun = true;
        tempData.lendata = stoi(s.substr(space2 + 1));
        string t(tempData.lendata, '_');
        tempData.para = t;
    }
    else
    {
        tempData.name = s.substr(7);
    }
}
void destroyTable(Data** table, int m)
{
    for (int i = 0; i < m; i++)
    {
        delete table[i];
        table[i] = NULL;
    }
    delete[] table;
}
void splitAssign(string s, string &name_now, string& value )
{
    //assign
    int space2 = s.find(" ", 7);
    name_now = s.substr(7, space2 - 7);
    value = s.substr(space2 + 1);
}
string xuliValueAssign(Data** table, int cap, string value, int &idxSecond, int &slot, string &charRedecl)
{   
    if (value[0] == '\'') // value: string
    {
        return "string";
    }
    else if (value[0] >= '0' && value[0] <= '9') // value: number
    {
        return "number";
    }
    else if (value[(int)value.length() - 1] == ')') //value: function
    {   
        slot = 0;
        string nameFun = "";
        while (value[0] != '(')
        {
            nameFun += value[0];
            value.erase(0, 1);
        }
        idxSecond = LOOKUP(table, cap, nameFun);
        if (idxSecond == -1) 
        {   
            charRedecl = nameFun;
            return "Undeclared";
        }
        else if (table[idxSecond]->isFun == 0) return "Invalid";
        // kiem tra ham void
        //else if (table[idxSecond]->type == "void") return "void";
        slot += table[idxSecond]->slot;
        //Xoa dau ngoac tron
        value = value.substr(1, value.length() - 2);
        // Dem tham so truyen vao Ham
        int numCom = 0;
        int numArg = 0;
        for (int i = 0; i < (int)value.length(); i++) 
        {
            if (value[i] == ',') numCom++;
        }
        if (numCom == 0)
        {
            if (value == "") numArg = 0;
            else numArg = 1;
        }
        else numArg = numCom + 1;
        if (numArg == table[idxSecond]->lendata) // so luong doi so truyen vao True
        {
            if (value == "") return "function";
            else 
            {
                int i = 0;
                int j = value.find(",", 0);
                string paraTemp = "";
                while (j != (int)string :: npos)
                {   
                    paraTemp = value.substr(0, j);
                    value = value.substr(j + 1);
                    //begin process Para
                    if (paraTemp[0] == '\'')//doi so la string
                    {
                        if (table[idxSecond]->para[i] == '_')
                        {
                            table[idxSecond]->para[i] = '1';
                        }
                        else if (table[idxSecond]->para[i] == '0')
                        {
                            return "TypeMisMatch";
                        }
                    }
                    else if (paraTemp[0] >= '0' && paraTemp[0] <= '9') // doi so la number
                    {
                        if (table[idxSecond]->para[i] == '_')
                        {
                            table[idxSecond]->para[i] = '0';
                        }
                        else if (table[idxSecond]->para[i] == '1')
                        {
                            return "TypeMisMatch";
                        }
                    }
                    else // doi so la variable
                    {
                        int idxVar = LOOKUP(table, cap, paraTemp);
                        if (idxVar == -1) 
                        {   
                            charRedecl =  paraTemp;
                            return "Undeclared";
                        }
                        else if (table[idxVar]->isFun) return "TypeMisMatch";
                        else if (table[idxVar]->type == "" && table[idxSecond]->para[i] == '_')
                        {   
                            slot += table[idxVar]->slot;
                            return "Khong the suy dien";
                        }
                        else if (table[idxVar]->type == "number" && table[idxSecond]->para[i] == '_')
                        {
                            slot += table[idxVar]->slot;
                            table[idxSecond]->para[i] = '0';
                        }
                        else if (table[idxVar]->type == "string" && table[idxSecond]->para[i] == '_')
                        {
                            slot += table[idxVar]->slot;
                            table[idxSecond]->para[i] = '1';
                        }
                        else if (table[idxVar]->type == "" && table[idxSecond]->para[i] == '1')
                        {
                            table[idxVar]->type = "string";
                        }
                        else if (table[idxVar]->type == "" && table[idxSecond]->para[i] == '0')
                        {
                            table[idxVar]->type = "number";
                        }
                        else if (table[idxVar]->type == "number" && table[idxSecond]->para[i] == '1')
                        {
                            return "TypeMisMatch";
                        }
                        else if (table[idxVar]->type == "string" && table[idxSecond]->para[i] == '0')
                        {
                            return "TypeMisMatch";
                        }
                    }
                    //end para
                    j = value.find(",", 0);
                    i++;
                }
                paraTemp = value;
                if (paraTemp[0] == '\'')//doi so la string
                    {
                        if (table[idxSecond]->para[i] == '_')
                        {
                            table[idxSecond]->para[i] = '1';
                        }
                        else if (table[idxSecond]->para[i] == '0')
                        {
                            return "TypeMisMatch";
                        }
                    }
                    else if (paraTemp[0] >= '0' && paraTemp[0] <= '9') // doi so la number
                    {
                        if (table[idxSecond]->para[i] == '_')
                        {
                            table[idxSecond]->para[i] = '0';
                        }
                        else if (table[idxSecond]->para[i] == '1')
                        {
                            return "TypeMisMatch";
                        }
                    }
                    else // doi so la variable
                    {
                        int idxVar = LOOKUP(table, cap, paraTemp);
                        if (idxVar == -1) 
                        {   
                            charRedecl = paraTemp;
                            return "Undeclared";
                        }
                        else if (table[idxVar]->isFun) return "TypeMisMatch";
                        else if (table[idxVar]->type == "" && table[idxSecond]->para[i] == '_')
                        {   
                            slot += table[idxVar]->slot;
                            return "Khong the suy dien";
                        }
                        else if (table[idxVar]->type == "number" && table[idxSecond]->para[i] == '_')
                        {
                            slot += table[idxVar]->slot;
                            table[idxSecond]->para[i] = '0';
                        }
                        else if (table[idxVar]->type == "string" && table[idxSecond]->para[i] == '_')
                        {
                            slot += table[idxVar]->slot;
                            table[idxSecond]->para[i] = '1';
                        }
                        else if (table[idxVar]->type == "" && table[idxSecond]->para[i] == '1')
                        {
                            table[idxVar]->type = "string";
                        }
                        else if (table[idxVar]->type == "" && table[idxSecond]->para[i] == '0')
                        {
                            table[idxVar]->type = "number";
                        }
                        else if (table[idxVar]->type == "number" && table[idxSecond]->para[i] == '1')
                        {
                            return "TypeMisMatch";
                        }
                        else if (table[idxVar]->type == "string" && table[idxSecond]->para[i] == '0')
                        {
                            return "TypeMisMatch";
                        }
                    }
            }
            if (table[idxSecond]->type == "void") return "void";
            return "function";
        }
        else return "TypeMisMatch"; // doi so truyen vao sai
    }
    else 
    {   
        idxSecond = LOOKUP(table, cap, value);
        if (idxSecond == -1)
        {   
            charRedecl = value;
            return "Undeclared";
        }
        else if (table[idxSecond]->isFun) return "Invalid";
        return "variable";
    }
}
int compareData(Data a, Data b)
{
    if (a.scope > b.scope) return 1; // a > b
    else if (a.scope < b.scope) return -1; //a < b
    else
    {
        if (a.name == b.name) return 0; //trung nhau
        else return 2; //a != b
    }
}
int insertLINEAR(Data** table, Data data, int cap, int c1,int &i)
{       
    int idx = LOOKUP(table, cap, data.name);
    if (data.scope > 0 && data.isFun) return 2; // Error: InvalidDecleration
    if (idx != -1 && table[idx]->scope == data.scope) return 1; // Redecle 
    int h = (long long)getKey(data) % cap;
    long long hp = h;
    while (i < cap)
    {   
        if (table[hp] == NULL)
        {
            Data* newIn = new Data;
            *newIn = data;
            table[hp] = newIn;
            table[hp]->slot = i;
            return 0; // True
        }
        else if (compareData(data, *table[hp]) == 0)
        {
            return 1; // Redecle
        }
        i++;
        hp = (h + c1 * i) % cap;
    }
    return 3;// Overflow
}
int insertDOUBLE(Data** table, Data data, int cap, int c1, int &i)
{       
    int idx = LOOKUP(table, cap, data.name);
    if (data.scope > 0 && data.isFun) return 2; // Error: InvalidDecleration
    if (idx != -1 && table[idx]->scope == data.scope) return 1; // Redecle  
    int h1 = (long long)getKey(data) % cap;
    int h2 = 1 + (long long)getKey(data) % (cap - 2);
    int hp = h1;
    while (i < cap)
    {
        if (table[hp] == NULL)
        {
            Data* newIn = new Data;
            *newIn = data;
            table[hp] = newIn;
            table[hp]->slot = i;
            return 0; // True
        }
        else if (compareData(data, *table[hp]) == 0)
        {
            return 1; // Redecle
        }
        i++;
        hp = (h1 + c1 * i * h2) % cap;
    }
    return 3; // Overflow
}
int insertQUADRATIC(Data** table, Data data, int cap, int c1, int c2, int &i)
{   
    int idx = LOOKUP(table, cap, data.name);
    if (data.scope > 0 && data.isFun) return 2; // Error: InvalidDecleration
    if (idx != -1 && table[idx]->scope == data.scope) return 1; // Redecle 
    long long k = getKey(data);
    int h = k % cap;
    int hp = h;
    while (i < cap)
    {
        if (table[hp] == NULL)
        {
            Data* newIn = new Data;
            *newIn = data;
            table[hp] = newIn;
            table[hp]->slot = i;
            return 0; // True
        }
        else if (compareData(data, *table[hp]) == 0)
        {
            return 1; // Redecle
        }
        i++;
        hp = (h + c1 * i + c2 * i * i) % cap;
    }
    return 3; // Overflow
}
void PRINT(Data** table, int cap)
{   
    string res;
    for (int i = 0; i <  cap; i++)
    {
        if (table[i] != NULL)
        {
            res += to_string(i)+" "+table[i]->name+"//"+to_string(table[i]->scope)+";";
        }
    }
    if(res != "")
    {
        res.erase((int)res.length() - 1, 1);
        cout<<res<<endl;
    }
}
string CallOperator(Data** table, int cap, string &value, int &idxFun, string &name, int &slot, string &charRe)
{
   name = "";
   while (value[0] != '(')
   {
       name += value[0];
       value.erase(0,1);
   }
   value = value.substr(1, value.length() - 2);
   idxFun = LOOKUP(table, cap, name);
   if (idxFun == -1)
   {    
       charRe = name;
       return "Undeclared";
   }
   else if (table[idxFun]->isFun == 0)
   {
       return "TypeMisMatch";
   }
   slot += table[idxFun]->slot;
   int numCom = 0;
   int numPara = 0;
   for (int i = 0; i < (int)value.length(); i++)
   {
       if (value[i] == ',') numCom++;
   }
   if (numCom == 0)
   {
       if (value == "") numPara = 0;
       else numPara = 1;
   }
   else numPara = numCom + 1;\
   ////////////////////////////
   string namePara = "";
   if (numPara == table[idxFun]->lendata)
   {
       if (value != "")
       {
           int i = 0;
           int idxCom = value.find(',', 0);
           while (idxCom != (int)string :: npos)
           {
               namePara = value.substr(0, idxCom);
               value.erase(0, idxCom + 1);
               if (namePara[0] == '\'')
               {
                   if (table[idxFun]->para[i] == '0') return "TypeMisMatch";
                   else if (table[idxFun]->para[i] == '_')
                   {
                       table[idxFun]->para[i] = '1';
                   }
               }
               else if (namePara[0] >= '0' && namePara[0] <= '9')
               {
                   if (table[idxFun]->para[i] == '1') return "TypeMisMatch";
                   else if (table[idxFun]->para[i] == '_')
                   {
                       table[idxFun]->para[i] = '0';
                   }
               }
               else
               {
                   int idxPara = LOOKUP(table, cap, namePara);
                   if (idxPara == -1) 
                   {    
                       charRe = namePara;
                       return "Undeclared";
                   }
                   else if (table[idxPara]->isFun) return "TypeMisMatch";
                   else 
                   {
                       slot += table[idxPara]->slot;
                       if (table[idxFun]->para[i] == '_' && table[idxPara]->type == "")
                       {
                           return "Khong the suy dien";
                       }
                       else if (table[idxFun]->para[i] == '_' && table[idxPara]->type == "number")
                       {
                           table[idxFun]->para[i] = '0';
                       }
                       else if (table[idxFun]->para[i] == '_' && table[idxPara]->type == "string")
                       {
                           table[idxFun]->para[i] = '1';
                       }
                       else if (table[idxFun]->para[i] == '0' && table[idxPara]->type == "")
                       {
                           table[idxPara]->type = "number";
                       }
                       else if (table[idxFun]->para[i] == '1' && table[idxPara]->type == "")
                       {
                           table[idxPara]->type = "string";
                       }
                       else if (table[idxFun]->para[i] == '1' && table[idxPara]->type == "number")
                       {
                           return "TypeMisMatch";
                       }
                       else if (table[idxFun]->para[i] == '0' && table[idxPara]->type == "string")
                       {
                           return "TypeMisMatch";
                       }
                    }
                }
               idxCom = value.find(',', 0);
               i++;
           }
            namePara = value;
            if (namePara[0] == '\'')
            {
                if (table[idxFun]->para[i] == '0') return "TypeMisMatch";
                else if (table[idxFun]->para[i] == '_')
                {
                    table[idxFun]->para[i] = '1';
                }
            }
            else if (namePara[0] >= '0' && namePara[0] <= '9')
            {
                if (table[idxFun]->para[i] == '1') return "TypeMisMatch";
                else if (table[idxFun]->para[i] == '_')
                {
                    table[idxFun]->para[i] = '0';
                }
            }
            else
            {
                int idxPara = LOOKUP(table, cap, namePara);
                if (idxPara == -1) 
                {   
                    charRe = namePara;
                    return "Undeclared";
                }
                else if (table[idxPara]->isFun) return "TypeMisMatch";
                else 
                {
                    slot += table[idxPara]->slot;
                    if (table[idxFun]->para[i] == '_' && table[idxPara]->type == "")
                    {
                        return "Khong the suy dien";
                    }
                    else if (table[idxFun]->para[i] == '_' && table[idxPara]->type == "number")
                    {
                        table[idxFun]->para[i] = '0';
                    }
                    else if (table[idxFun]->para[i] == '_' && table[idxPara]->type == "string")
                    {
                        table[idxFun]->para[i] = '1';
                    }
                    else if (table[idxFun]->para[i] == '0' && table[idxPara]->type == "")
                    {
                        table[idxPara]->type = "number";
                    }
                    else if (table[idxFun]->para[i] == '1' && table[idxPara]->type == "")
                    {
                        table[idxPara]->type = "string";
                    }
                    else if (table[idxFun]->para[i] == '1' && table[idxPara]->type == "number")
                    {
                        return "TypeMisMatch";
                    }
                    else if (table[idxFun]->para[i] == '0' && table[idxPara]->type == "string")
                    {
                        return "TypeMisMatch";
                    }
                }
            }
       }
       if (table[idxFun]->type == "") 
       {
           table[idxFun]->type = "void";
           return "true";
       }
       else if (table[idxFun]->type == "number" || table[idxFun]->type == "string")
       {
           return "TypeMisMatch";
       }
       else
       {
           return "true";
       }
   }
   else 
   {
       return "TypeMisMatch";
   }
}
void deleteData(Data** table, int i)
{
    delete table[i];
    table[i] = NULL;
}
class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};
#endif