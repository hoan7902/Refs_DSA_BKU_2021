#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
struct Symbol
{
    string identifier;
    string type;
    int scope=0;
    
};
    struct Node
    {
        Symbol data;
        Node* pnext;
    };
    struct Danhsach
    {
        Node* phead;
        Node *ptail;
    };

class SymbolTable {
    Symbol infor;    
public:
    SymbolTable() {}
    void khoitaodanhsach(Danhsach &l)
    {
        l.phead=NULL;
        l.ptail=NULL;
        return;
    }
    Node* khoitaonode(Symbol a)
    {
        Node *p=new Node;
        p->data=a;
        p->pnext=NULL;
        return p;
    }
    void xoadanhsach(Danhsach &l)
    {
        for (Node *k=l.phead;k!=NULL;k=k->pnext)
        {
            Node *tam=l.phead;
            l.phead=l.phead->pnext;
            tam=NULL;
            delete tam;
        }
        return;
    }
    void run(string filename);
};

#endif
