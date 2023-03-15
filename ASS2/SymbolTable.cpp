#include "SymbolTable.h"
////////////////FUNCTION
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void SymbolTable::run(string filename)
{
    fstream ifs;
    ifs.open(filename,ios::in);
    int tamvuc=0;
    splay_Tree* T=new_splay_tree();
    DS Store_del;
    while(!ifs.eof())
    {   
        string s;
        getline(ifs,s);
        if (s=="BEGIN")
        {
            tamvuc++;
        }
        else if (s == "END")
        {      
            if (tamvuc == 0)
            {
                deleteTree(T->root);
                delete T;
                throw UnknownBlock();
            }
            if (Store_del.head)
            {
                if (Store_del.tail->add->data.scope == tamvuc)
                {
                    Node* mark = Store_del.tail->add;
                    while (Store_del.head->add->data.scope != tamvuc)
                    {
                        Node* temp = Store_del.pop_front();
                        Store_del.push_back(temp);
                    }
                    while (Store_del.head && Store_del.head->add->data.scope == tamvuc)
                    {
                        Store_del.pop_front();
                        deleteNode(T,Store_del.pop_front());
                    }
                }
            }
            tamvuc--;
        }
        else if (regex_match(s,regex("^INSERT [a-z][a-zA-Z0-9_]* ((number|string)|(([(](()|((number)|(string))((,number)|(,string))*)[)])->(string|number))) (true|false)")))
        {       
           Data data;
           int count_cmp=0,count_splay=0;
           bool test=splitInsert(s,data,tamvuc);
           if (!test)
           {
               deleteTree(T->root);
               delete T;
               throw InvalidDeclaration(s);
           }
           Node* x=newNode(data);
           test=insertInSplayTree(T,x,count_cmp,count_splay);
           if(!test)
           {    
               deleteTree(T->root);
               delete T;
               throw Redeclared(s);
           }
           else 
           {    
               if (tamvuc > 0) Store_del.push_back(T->root);
               cout<<count_cmp<<" "<<count_splay<<endl;
           }
        }
        else if (regex_match(s, regex("^ASSIGN [a-z][a-zA-Z0-9_]* ((([a-z][a-zA-Z0-9_]*(([(][)])|([(]((([0-9]+,)|('[0-9a-zA-Z ]+',)|(([a-z][a-zA-Z0-9_]*),))*)(([0-9]+)|('[0-9a-zA-Z ]+')|([a-z][a-zA-Z0-9_]*))[)]))))|(('[0-9a-zA-Z ]+')|([0-9]+))|([a-z][a-zA-Z0-9_]*))")))
        {   
            Node* store = NULL;
            string name,value;
            int count_cmp=0,count_splay = 0;
            string para_test;
            int check=splitAssign(s,name,value,para_test,T->root);
            Data data_new;
            data_new.identifier_name=name;
            data_new.scope=-1;
            if(check==0 || check==1)
            {       
                searchNode(T->root,data_new);
                if (data_new.scope == -1)
                {   
                    deleteTree(T->root);
                    delete T;
                    throw Undeclared(s);
                }
                if ((check==0 && data_new.type != "number") || (check==1 && data_new.type != "string"))
                {
                    deleteTree(T->root);
                    delete T;
                    throw TypeMismatch(s);
                }
                count_cmp_assign(T->root,data_new,count_cmp,store);
                Splay_Tree_Operation(T,store,count_splay);
                cout<<count_cmp<<" "<<count_splay<<endl;
            }
            else if (check == 69)
            {
                deleteTree(T->root);
                delete T;
                throw Undeclared(s);
            }
            else if (check == 2)
            {   
                Data temp;
                temp.identifier_name = value;
                temp.scope = -1;
                searchNode(T->root,temp); // tìm trong cây thằng sau
                if (temp.scope == -1)
                {
                    deleteTree(T->root);
                    delete T;
                    throw Undeclared(s);
                }
                count_cmp_assign(T->root,temp,count_cmp,store);
                Splay_Tree_Operation(T,store,count_splay);
                int i=0;
                string test = temp.para;
                if (temp.para=="()" && para_test=="()")
                {

                }
                else
                {
                    while (para_test.length()) /// kiem tra o trong ham
                    {
                    int pos = para_test.find(" ");
                    if (pos != string::npos)
                    {
                        string a = para_test.substr(0,pos);
                        if (a == "0" || a == "1")
                        {
                            if (a != test.substr(0,1))
                            {
                                deleteTree(T->root);
                                delete T;
                                throw TypeMismatch(s);
                            }
                            else 
                            {   
                                para_test.erase(0,pos+1);
                                test.erase(0,1);
                            }
                        }
                        else 
                        {
                            Data run;
                            run.identifier_name = a;
                            run.scope = -1;
                            searchNode(T->root,run);
                            if (run.scope == -1)
                            {
                                deleteTree(T->root);
                                delete T;
                                throw TypeMismatch(s);
                            }
                            if ((run.type == "string" && test.substr(0,1)=="0") || (run.type == "number" && test.substr(0,1)=="1"))
                            {
                                deleteTree(T->root);
                                delete T;
                                throw TypeMismatch(s);
                            }
                            para_test.erase(0,pos+1);
                            test.erase(0,1);
                            count_cmp_assign(T->root,run,count_cmp,store);
                            Splay_Tree_Operation(T,store,count_splay);
                        }
                    }
                    }
                }
                searchNode(T->root,data_new);
                if (data_new.scope == -1)
                {   
                    deleteTree(T->root);
                    delete T;
                    throw Undeclared(s);
                }
                count_cmp_assign(T->root,data_new,count_cmp,store);
                Splay_Tree_Operation(T,store,count_splay);
                cout<<count_cmp<<" "<<count_splay<<endl;
            }
            else if (check==3)
            {   
                Data temp;
                temp.identifier_name=value;
                temp.scope=-1;
                searchNode(T->root,temp);
                if (temp.scope == -1)
                {
                    deleteTree(T->root);
                    delete T;
                    throw Undeclared(s);
                }
                searchNode(T->root,data_new);
                if (data_new.scope == -1)
                {
                    deleteTree(T->root);
                    delete T;
                    throw Undeclared(s);
                }
                if (data_new.type != temp.type)
                {
                    deleteTree(T->root);
                    delete T;
                    throw TypeMismatch(s);
                }
                count_cmp_assign(T->root,temp,count_cmp,store);
                Splay_Tree_Operation(T,store,count_splay);
                count_cmp_assign(T->root,data_new,count_cmp,store);
                Splay_Tree_Operation(T,store,count_splay);
                cout<<count_cmp<<" "<<count_splay<<endl;
            }
        }
        else if (regex_match(s,regex("^LOOKUP [a-z] ")))
        {       
            int a=0;
            string name = s.substr(7);
            Data data_new;
            data_new.scope = -1;
            data_new.identifier_name = name;
            searchNode(T->root,data_new);
            if (data_new.scope == -1)
            {   
                deleteTree(T->root);
                delete T;
                throw Undeclared(s);
            }
            Node* store = NULL;
            count_cmp_assign(T->root,data_new,a,store);
            Splay_Tree_Operation(T,store,a);
        }
        else if (s == "PRINT")
        {
            string s;
            PRINT(T->root,s);
            if (s.length()) s.erase(s.length()-1,1);
            cout<<s<<endl;
        }
        else 
        {
            throw InvalidInstruction(s);
        }
    }
    pre(T->root);
}