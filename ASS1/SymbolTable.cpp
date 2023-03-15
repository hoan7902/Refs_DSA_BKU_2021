#include "SymbolTable.h"
void SymbolTable::run(string filename)
{   
    fstream ifs;
    ifs.open(filename,ios::in);
    Danhsach l;
    khoitaodanhsach(l);
    int n = 0;
    int tamvuc = 0;
    while (!ifs.eof())
    {
        //======HÀM INSERT
        string s;
        getline(ifs, s);
            if (s == "BEGIN")
            {
                tamvuc += 1;
            }
            else if (s == "END")
            {
                tamvuc -= 1;
                if (n > 0)
                {
                    int dem = 0;
                    for (Node* k = l.phead; k != NULL; k = k->pnext)
                    {
                        if (k->data.scope > tamvuc)
                        {
                            dem += 1;
                            Node* tam = k;
                            l.phead = l.phead->pnext;
                            tam = NULL;
                            delete tam;
                        }
                        else continue;
                    }
                    n=n-dem;
                }
            }
            else if (regex_match(s, regex("^INSERT[ ]{1}[a-z]{1}[a-zA-Z0-9_]{0,}[ ]{1}(string|number)")))
            {
                int vt1 = s.find(" ");
                int vt2 = s.find(" ", vt1 + 1);
                string bien, loai;
                bien = s.substr(vt1 + 1, vt2 - vt1 - 1);
                loai = s.substr(vt2 + 1);
                Symbol tam;
                tam.identifier = bien;
                tam.type = loai;
                tam.scope = tamvuc;
                if (n == 0)
                {
                    Node* p = khoitaonode(tam);
                    l.phead = l.ptail = p;
                    n = n + 1;;
                    cout << "success" << endl;
                }
                else if (n > 0)
                {
                    bool kt = true;
                    for (Node* k = l.phead; k != NULL; k = k->pnext)
                    {
                        if (tam.identifier == k->data.identifier && tam.scope == k->data.scope) kt = false;
                    }
                    if (kt == true)
                    {
                        Node* p = khoitaonode(tam);
                        p->pnext = l.phead;
                        l.phead = p;
                        n = n + 1;;
                        cout << "success" << endl;
                    }
                    else {
                        throw Redeclared(s);
                    }
                }
            }
            //=======HÀM ASSIGN
            else if (regex_match(s, regex("^ASSIGN [a-z][a-zA-Z0-9_]* ((([a-z][a-zA-Z0-9_]*(([(][)])|([(]((([0-9]+,)|('[0-9a-zA-Z ]+',)|(([a-z][a-zA-Z0-9_]*),))*)(([0-9]+)|('[0-9a-zA-Z ]+')|([a-z][a-zA-Z0-9_]*))[)]))))|(('[0-9a-zA-Z ]+')|([0-9]+)))")))
            {
                int vt1 = s.find(" ");
                int vt2 = s.find(" ", vt1 + 1);
                string bien, value;
                bien = s.substr(vt1 + 1, vt2 - vt1 - 1);
                value = s.substr(vt2 + 1);
                if (value[0] >= '0' && value[0] <= '9') // KIỂU NUMBER
                {
                    bool test = false;
                    for (Node* k = l.phead; k != NULL; k = k->pnext)
                    {
                        if (k->data.identifier == bien && k->data.type == "number")
                        {   
                            cout << "success" << endl;
                            test = true;
                            break;
                        }
                        else if (k->data.identifier == bien && k->data.type != "number")
                        {       
                            throw TypeMismatch(s);
                            test = true;
                        }
                    }
                    if (!test) throw Undeclared(s);
                }
                else if (value[0] >= 'a' && value[0] <= 'z') //GÁN CHO BIẾN KHÁC
                {
                    bool kt1 = false, kt2 = false;
                    string kieu1, kieu2;
                    for (Node* k = l.phead; k != NULL; k = k->pnext)
                    {
                        if (k->data.identifier == value)
                        {
                            kt1 = true;
                            kieu1 = k->data.type;
                            break;
                        }
                    }
                    for (Node* k = l.phead; k != NULL; k = k->pnext)
                    {
                        if (k->data.identifier == bien)
                        {
                            kt2 = true;
                            kieu2 = k->data.type;
                            break;
                        }
                    }
                    if (kt1 == true && kt2 == true && kieu1 == kieu2)
                    {
                        cout << "success" << endl;
                    }
                    else if (kt2 == true && kt1 == true && kieu1 != kieu2)
                    {
                        throw TypeMismatch(s);
                    }
                    else throw Undeclared(s);
                }
                else //TRƯỜNG HỢP CÒN LẠI LÀ KIỂU STRING
                {
                    bool test = false;
                    for (Node* k = l.phead; k != NULL; k = k->pnext)
                    {
                        if (k->data.identifier == bien && k->data.type == "string")
                        {
                            cout << "success" << endl;
                            test = true;
                            break;
                        }
                        else if (k->data.identifier == bien && k->data.type != "string")
                        {
                            throw TypeMismatch(s);
                            test = true;
                        }
                    }
                    if (!test) throw Undeclared(s);
                }
            }
            else if (regex_match(s, regex("^LOOKUP[ ]{1}[a-z]{1}[a-zA-Z0-9_]{0,}")))
            {
                bool kt = false;
                string con;
                con = s.substr(7);
                for (Node* k = l.phead; k != NULL; k = k->pnext)
                {
                    if (k->data.identifier == con)
                    {
                        cout << k->data.scope << endl;
                        kt = true;
                        break;
                    }
                }
                if (!kt)
                {
                    throw Undeclared(s);
                }
            }
            ////////HÀM PRINT
            else if (s == "PRINT")
            {
                if (n == 0) continue;
                else
                {
                    string end;
                    for (Node* k = l.phead; k != NULL; k = k->pnext)
                    {
                        int test = end.find(" " + k->data.identifier + "/");
                        if (test == -1 && tamvuc >= k->data.scope)
                        {
                            end = " " + k->data.identifier + "//" + to_string(k->data.scope) + end;
                        }
                    }
                    end = end.erase(0, 1);
                    cout << end << endl;
                }
            }
            else if (s == "RPRINT")
            {
                if (n == 0) continue;
                else
                {
                    string end;
                    for (Node* k = l.phead; k != NULL; k = k->pnext)
                    {
                        int test = end.find(" " + k->data.identifier + "/");
                        if (test == -1 && tamvuc >= k->data.scope)
                        {
                            end = end + " " + k->data.identifier + "//" + to_string(k->data.scope);
                        }
                    }
                    end = end.erase(0, 1);
                    cout << end << endl;
                }
            }
            else
            {
                throw InvalidInstruction(s);
            }
            if (tamvuc < 0)
            {
                throw UnknownBlock();
            }
        }
        if (tamvuc > 0)
        {
            throw UnclosedBlock(tamvuc);
        }
    xoadanhsach(l);
    ifs.close();
}

