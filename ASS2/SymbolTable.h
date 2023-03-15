#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
struct Data
{
    string identifier_name;
    string type;
    int scope=0;
    bool isFun=false;
    string para;
};
struct Node{   
    Data data;
    Node* left;
    Node* right;
    Node* parent;
};
struct splay_Tree
{
    Node* root;
};
splay_Tree* new_splay_tree()
{
    splay_Tree* p = new splay_Tree;
    p->root=NULL;
    return p;
}
void leftRotate(splay_Tree*t, Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    if(y->left != NULL) 
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == NULL)
    { //x is root
        t->root = y;
    }
    else if(x == x->parent->left) 
    { //x is left child
        x->parent->left = y;
    }
    else 
    { //x is right child
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(splay_Tree* t, Node *x) 
{
    Node *y = x->left;
    x->left = y->right;
    if(y->right != NULL)
    {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == NULL)
    { //x is root
        t->root = y;
    }
    else if(x == x->parent->right) 
    { //x is left child
        x->parent->right = y;
    }
    else 
    { //x is right child
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}
int CompareData(Data data1,Data data2)
{
    if (data1.scope > data2.scope) return 1;
    else if (data1.scope < data2.scope) return -1;
    return data1.identifier_name.compare(data2.identifier_name);
}
Node* newNode(Data data)
{
    Node* p=new Node;
    p->data=data;
    p->left=p->right=NULL;
    p->parent=NULL;
    return p;
}
void Splay_Tree_Operation(splay_Tree* T,Node* x, int &count_splay)
{   
    if (x != T->root) count_splay++;
    while(x->parent!=NULL)
    {   
        if (x->parent==T->root)
        {   
            if (x==x->parent->left)
            {   
                rightRotate(T,x->parent);
            }
            else 
            {  
                leftRotate(T,x->parent);
            }
        }
        else
        {   
            Node* cha=x->parent;
            Node* ongnoi=cha->parent;
            if (x->parent->left==x && cha->parent->left==cha) //LEFT LEFT
            {   
                rightRotate(T,ongnoi);
                rightRotate(T,cha);
            }
            else if (x->parent->right==x && cha->parent->right==cha) //RIGHT RIGHT
            {   
                leftRotate(T,ongnoi);
                leftRotate(T,cha);
            }
            else if (x->parent->left==x && cha->parent->right==cha) //RIGHT LEFT
            {   
                rightRotate(T,cha);
                leftRotate(T,ongnoi);
            }
            else if (x->parent->right==x && cha->parent->left==cha) //LEFT RIGHT
            {       
                leftRotate(T,cha);
                rightRotate(T,ongnoi);
            }
        }
    }
}
bool insertInSplayTree(splay_Tree* T,Node* x,int & count_cmp,int & count_splay)
{   
    //INSERTTTTTTTTTT
    Node* temp=T->root;
    Node* y=NULL;
    while(temp!=NULL)
    {
        y=temp;
        if (CompareData(temp->data,x->data)>0)
        {   
            count_cmp++;
            temp=temp->left;
        }
        else if (CompareData(temp->data,x->data)<0)
        {
            count_cmp++;
            temp=temp->right;
        }
        else return false;
    }
    x->parent=y;
    if (y==NULL) 
    {   
        T->root=x;
        return true;
    }
    else if (CompareData(y->data,x->data)>0)
    {   
        y->left=x;
    }
    else if (CompareData(y->data,x->data)<0)
    {
        y->right=x;
    }
    ////////////////////////SPLAYYYYYYYYYY
    Splay_Tree_Operation(T,x,count_splay);
    return true;
}
void deleteTree(Node* root){
    if (root==NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
//TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
bool splitInsert(string s, Data &data, int scope_now) ///TÁCH INSERT
{   
    int vt1 = s.find(" ");
    int vt2 = s.find(" ", vt1 + 1);
    int vt3 = s.find(" ",vt2+1);
    data.identifier_name = s.substr(vt1 + 1, vt2 - vt1 - 1);
    if (s[vt2+1]=='(') {
        data.isFun=true;
        int k=vt2+2;
        while (true){
            if (s[k]==')')
            {
                break;
            }
            if (s[k]==',')
            {
                k++;
            }
            if (s[k]=='n') // number
            {
                data.para.push_back('0');
                k+=6;
            }
            if (s[k] == 's') //string
            {
                data.para.push_back('1');
                k+=6;
            }
        }
        k+=3;
        data.type=s.substr(k,6);
    }
    else {
        data.type=s.substr(vt2+1,vt3-vt2-1);
    } 
    string temp=s.substr(vt3+1);  
    if (data.isFun && scope_now!=0 && temp == "false") return false;////khai báo hàm ở tầm tực khác 0 mà static true
    if (temp=="true") data.scope=0; // không phải hàm và static==true thì thuộc khối toàn cục
    else  data.scope=scope_now; // trường hợp còn lại thì có tầm vực bằng tầm vực hiện tại
    return true;
}
Node* maximum(splay_Tree* T,Node *x)
{
    while(x->right!=NULL) x=x->right;
    return x;
}
void deleteNode(splay_Tree* T, Node* x)
{   
    if (x == NULL) return;
    int b=0;
    Splay_Tree_Operation(T,x,b);
    splay_Tree* left_T = new_splay_tree();
    left_T->root=T->root->left;
    if (left_T->root != NULL)
    {
        left_T->root->parent=NULL;
    }
    splay_Tree* right_T = new_splay_tree();
    right_T->root=T->root->right;
    if (right_T->root != NULL)
    {
        right_T->root->parent=NULL;
    }
    delete x;
    if (left_T->root!=NULL)
    {
        if (right_T->root != NULL)
        {
            Node* temp = maximum(T,left_T->root);
            Splay_Tree_Operation(left_T,temp,b);
            left_T->root->right=right_T->root;
            T->root=left_T->root;
        }
        else
        {
            T->root = left_T->root;
        }
    }
    else 
    {
        if(right_T->root) T->root=right_T->root;
        else T->root = NULL;
    }
    delete right_T,left_T;
}
void searchNode(Node* x, Data &data_new)
{
    if (x!=NULL) 
    {
        if (x->data.identifier_name==data_new.identifier_name && x->data.scope > data_new.scope)
        {
                data_new=x->data;
        }
        searchNode(x->left,data_new);
        searchNode(x->right,data_new);
    }
}
void count_cmp_assign(Node* root,Data data, int &count_cmp,Node* &store)
{
    if (root==NULL) return ;
    if (CompareData(root->data,data)==0)
    {
        count_cmp++;
        store=root;
        return;
    }
    else if (CompareData(root->data,data)>0)
    {
        count_cmp++;
        count_cmp_assign(root->left,data,count_cmp,store);
    }
    else if (CompareData(root->data,data)<0)
    {
        count_cmp++;
        count_cmp_assign(root->right,data,count_cmp,store);
    }
}
void pre(Node* root)
{
    if (root==NULL) return;
    cout<<root->data.identifier_name<<" ";
    pre(root->left);
    pre(root->right);
}
// 0: number 1:string 2:ham 3:bien
// 69: khi trong ham co bien chua dc khai bao
int splitAssign(string s, string &name, string &value, string &para, Node* root)
{
    int vt1 = s.find(" ");
    int vt2 = s.find(" ",vt1+1);
    name = s.substr(vt1+1,vt2-vt1-1);
    value = s.substr(vt2+1);
    if (value[0] >= '0' && value[0] <= '9') return 0;
    else if (value[0] == '\'') return 1;
    else if (value[value.length()-1] == ')') 
    {       
        string temp=value;
        int pos = temp.find('(');
        value = value.substr(0,pos); // gán value bằng tên hàm 
        temp.erase(0,pos);// xử lí để temp chỉ còn dạng (....)
        if (temp == "()")
        {
            para = "";
            return 2;
        }
        temp = temp.erase(0,1); // xóa dấu ngoặc đầu 
        temp = temp.erase(temp.length()-1,1); // xóa dấu ngoặc cuối
        while (temp.length() != 0)
        {
            int i = temp.find(",");
            if (i != string :: npos) // tìm ra dấu phẩy
            {
                if (temp[0] >= '1' && temp[0] <= '9')
                {
                    para += "0 ";
                }
                else if (temp[0] == '\'')
                {
                    para += "1 ";
                }
                else
                {
                    string name = temp.substr(0,i);
                    para += name + " ";
                }
                temp.erase(0,i+1);
            }
            else // không còn tim ra dấu phẩy
            {
                if (temp[0] >= '1' && temp[0] <= '9')
                {
                    para += "0 ";
                }
                else if (temp[0] == '\'')
                {   
                    para += "1 ";
                }
                else 
                {
                    string name = temp.substr(0);
                    para += name + " ";
                }
                temp="";
            }
        }
        //if (para.length()) para.erase(para.length()-1,1);
        return 2;
    }
    else return 3;
}
void PRINT(Node* root,string &res)
{
    if (root != NULL)
    {
        res += root->data.identifier_name + "//" + to_string(root->data.scope) + " ";
        PRINT(root->left,res);
        PRINT(root->right,res);
    }
}
class NodeL
{
public:
    Node* add;
    NodeL* next;
    NodeL* prev;
    NodeL(): add(nullptr), next(nullptr), prev(nullptr){};
    NodeL(Node*_add, NodeL* _left=NULL, NodeL* _right=NULL): add(_add), next(_left), prev(_right){}; 
};
class DS
{
public:
    NodeL* head;
    NodeL* tail;
    DS():head(NULL), tail(NULL){};
    void push_back(Node* add)
    {   
        NodeL* temp = new NodeL(add);
        if (head == NULL)
        {
            head = tail = temp;
        }
        else 
        {
            tail->next = temp;
            temp->prev=tail;
            this->tail = this->tail->next;
        }
    }
    Node* pop_front()
    {
        if (this->head == NULL) return NULL;
        Node* res = this->head->add;
        NodeL* temp = this->head;
        this->head = this->head->next;
        if (this->head == NULL)
        {
            this->tail = NULL;
        }
        else 
        {
            this->head->prev = NULL;
        }
        delete temp;
        return res;
    }
};
class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};
#endif