#include "SymbolTable.h"
void SymbolTable::run(string filename)
{   
    Data** table = NULL;
    int scope = 0;
    Data tempData;
    string dectecMethod = "";
    int m = 0, c1 = -1, c2 = -1;
    fstream ifs(filename,ios::in);
    string s;
    while(!ifs.eof())
    {   
        tempData = Data();
        getline(ifs, s);
        if (isLINEAR(s))
        {
            if(dectecMethod == "")
            {
                dectecMethod = "LINEAR";
                split_DetecMethod(s, m, c1, c2);
                table = createTable(m);
            }
            else throw  InvalidInstruction(s);
        }
        else if (isQUADRATIC(s))
        {
            if(dectecMethod == "")
            {
                dectecMethod = "QUADRATIC";
                split_DetecMethod(s, m, c1, c2);
                table = createTable(m);
            }
            else throw  InvalidInstruction(s);
        }
        else if (isDOUBLE(s))
        {
            if(dectecMethod == "")
            {
                dectecMethod = "DOUBLE";
                split_DetecMethod(s, m, c1, c2);
                table = createTable(m);
            }
            else throw  InvalidInstruction(s);
        }
        else if (isINSERT(s) && dectecMethod != "")
        {
            splitInsert(s, tempData, scope);
            if (dectecMethod == "LINEAR")
            {
                int i = 0;
                int test = insertLINEAR(table, tempData, m, c1, i);
                if (test == 0) cout << i << endl;
                else if (test == 1)
                {
                    destroyTable(table, m);
                    throw Redeclared(tempData.name);
                }
                else if (test ==2)
                {
                    destroyTable(table, m);
                    throw InvalidDeclaration(tempData.name);
                }
                else if (test ==3)
                {
                    destroyTable(table, m);
                    throw Overflow(s);
                }
            }
            else if (dectecMethod == "DOUBLE")
            {
                int i = 0;
                int test = insertDOUBLE(table, tempData, m, c1, i);
                if (test == 0) cout << i << endl;
                else if (test == 1)
                {
                    destroyTable(table, m);
                    throw Redeclared(tempData.name);
                }
                else if (test ==2)
                {
                    destroyTable(table, m);
                    throw InvalidDeclaration(tempData.name);
                }
                 else if (test ==3)
                {
                    destroyTable(table, m);
                    throw Overflow(s);
                }
            }
            else if (dectecMethod == "QUADRATIC")
            {
                int i = 0;
                int test = insertQUADRATIC(table, tempData, m, c1, c2, i);
                if (test == 0) cout << i << endl;
                else if (test == 1)
                {
                    destroyTable(table, m);
                    throw Redeclared(tempData.name);
                }
                else if (test ==2)
                {
                    destroyTable(table, m);
                    throw InvalidDeclaration(tempData.name);
                }
                 else if (test == 3)
                {
                    destroyTable(table, m);
                    throw Overflow(s);
                }
            }
        }
        else if (isASSIGN(s) && dectecMethod != "")
        {
           string name = "", value = "";
           int idxFirst = -1, idxSecond = -1;
           int slot = 0;
           string charRe = "";
           splitAssign(s, name, value);
           string test = xuliValueAssign(table, m, value, idxSecond, slot, charRe);
           idxFirst = LOOKUP(table, m, name);
           if (test == "Undeclared")
           {
               destroyTable(table, m);
               throw Undeclared(charRe);
           }
           else if (test == "void")
           {
                destroyTable(table, m);
                throw TypeMismatch(s);
           }
           else if (test == "TypeMisMatch")
           {
               destroyTable(table, m);
               throw TypeMismatch(s);
           }
           else if (test == "Khong the suy dien")
           {
               destroyTable(table, m);
               throw TypeCannotBeInferred(s);
           }
           else if (test == "number")
           {
               if (idxFirst == -1)
               {
                   destroyTable(table, m);
                   throw Undeclared(s);
               }
               else if (table[idxFirst]->isFun)
               {
                    destroyTable(table, m);
                    throw TypeMismatch(s);
               }
               else if (table[idxFirst]->type == "")
               {
                   table[idxFirst]->type = "number";
                    cout<<table[idxFirst]->slot<<endl;
               }
               else if (table[idxFirst]->type != "number")
               {
                   destroyTable(table, m);
                   throw TypeMismatch(s);
               }
               else if (table[idxFirst]->type == "number")
               {
                   cout<<table[idxFirst]->slot<<endl;
               }
           }
           else if (test == "string")
           {
               if (idxFirst == -1)
               {
                   destroyTable(table, m);
                   throw Undeclared(s);
               }
               else if (table[idxFirst]->isFun)
               {
                   destroyTable(table, m);
                   throw TypeMismatch(s);
               }
               else if (table[idxFirst]->type == "")
               {
                    table[idxFirst]->type = "string";
                    cout<<table[idxFirst]->slot<<endl;
               }
               else if (table[idxFirst]->type != "string")
               {
                   destroyTable(table, m);
                   throw TypeMismatch(s);
               }
               else if (table[idxFirst]->type == "string")
               {
                   cout<<table[idxFirst]->slot<<endl;
               }
           }
           else if (test == "function")
           {
                if (idxFirst == -1)
                {
                    destroyTable(table, m);
                    throw Undeclared(name);
                }
                else if (table[idxFirst]->isFun)
                {
                    destroyTable(table, m);
                    throw TypeMismatch(s);
                }
                else if (table[idxFirst]->type == "" && table[idxSecond]->type == "")
                {
                    destroyTable(table, m);
                    throw TypeCannotBeInferred(s);
                }
                else if (table[idxSecond]->type == "" && 
                (table[idxFirst]->type == "number" || table[idxFirst]->type == "string"))
                {
                    table[idxSecond]->type = table[idxFirst]->type;
                    cout<<table[idxFirst]->slot + slot<<endl;
                }
                else if (table[idxFirst]->type == "" && 
                (table[idxSecond]->type == "number" || table[idxSecond]->type == "string"))
                {
                    table[idxFirst]->type = table[idxSecond]->type;
                    cout<<table[idxFirst]->slot + slot<<endl;
                }
                else if ((table[idxFirst]->type == "number" && table[idxSecond]->type != "number")
                || (table[idxFirst]->type == "string" && table[idxSecond]->type != "string"))
                {
                    destroyTable(table, m);
                    throw TypeMismatch(s);
                }
                else if ((table[idxFirst]->type == "number" || table[idxFirst]->type == "string")
                && table[idxFirst]->type == table[idxSecond]->type)
                {
                    cout<<table[idxFirst]->slot + slot<<endl;
                }
           }    
           else if (test == "variable")
           {    
                if (idxFirst == -1)
                {
                    destroyTable(table, m);
                    throw Undeclared(name);
                }
                else if (table[idxFirst]->isFun)
                {   
                    destroyTable(table, m);
                    throw TypeMismatch(s);
                }
                else if (table[idxFirst]->type == "" && table[idxSecond]->type == "")
                {
                    destroyTable(table, m);
                    throw TypeCannotBeInferred(s);
                }
                else if (table[idxFirst]->type == "" && (table[idxSecond]->type == "number" ||
                table[idxSecond]->type == "string"))
                {
                    table[idxFirst]->type = table[idxSecond]->type;
                    cout<<table[idxSecond]->slot + table[idxFirst]->slot<<endl;
                }
                else if (table[idxSecond]->type == "" && (table[idxFirst]->type == "number" ||
                table[idxFirst]->type == "string"))
                {
                    table[idxSecond]->type = table[idxFirst]->type;
                    cout<<table[idxSecond]->slot + table[idxFirst]->slot<<endl;
                }
                else if ((table[idxFirst]->type == "number" || table[idxFirst]->type == "string")
                && table[idxFirst]->type == table[idxSecond]->type)
                {
                    cout<<table[idxSecond]->slot + table[idxFirst]->slot<<endl;
                }
                else if ((table[idxFirst]->type == "number" && table[idxFirst]->type == "string")
                || (table[idxFirst]->type == "string" && table[idxFirst]->type == "number"))
                {
                    destroyTable(table, m);
                    throw TypeMismatch(s);
                }
           }
           else if (test == "Invalid")
           {
               destroyTable(table, m);
               throw InvalidInstruction(s);
           }
        } 
        else if (isCALL(s) && dectecMethod != "")
        {           
            string value = s.substr(5);
            string name = "";
            string charRe ="";
            int idxFun = 0, slot = 0;
            string test = CallOperator(table, m, value, idxFun, name, slot, charRe);
            if (test == "Undeclared")
            {
                destroyTable(table, m);
                throw Undeclared(charRe);
            }
            else if (test == "Invalid")
            {
                destroyTable(table, m);
                throw InvalidInstruction(s);
            }
            else if (test == "TypeMisMatch")
            {
                destroyTable(table, m);
                throw TypeMismatch(s);
            }
            else if (test == "Khong the suy dien")
            {
                destroyTable(table, m);
                throw TypeCannotBeInferred(s);
            }
            else
            {
                cout<<slot<<endl;
            }   
        }
        else if (isLOOKUP(s) && dectecMethod != "")
        {
            string name = s.substr(7);
            int res = LOOKUP(table, m, name);
            if (res == -1) 
            {
                destroyTable(table, m);
                throw Undeclared(name);
            }
            else cout<<res<<endl;
        }
        else if (s == "BEGIN" && dectecMethod != "")
        {
            scope++;
        }
        else if (s == "END" && dectecMethod != "")
        {
            if (scope == 0)
            {
                destroyTable(table, m);
                throw UnknownBlock();
            }
            else
            {
                for (int i = 0; i < m; i++)
                {
                    if (table[i] && table[i]->scope >= scope)
                    {
                        deleteData(table, i);
                    }
                }
                scope--;
            }
        }
        else if (s == "PRINT" && dectecMethod != "")
        {
            PRINT(table, m);
        }
        else 
        {
            destroyTable(table, m);
            throw InvalidInstruction(s);
        }
    }
    if (scope > 0)
    {
        destroyTable(table, m);
        throw UnclosedBlock(scope);
    }
    destroyTable(table, m);
    ifs.close();
}