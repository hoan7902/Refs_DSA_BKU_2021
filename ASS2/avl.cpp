#include<iostream>
#include<math.h>
using namespace std;
class Node
{
    public:
    int key;
    Node* left;
    Node* right;
    int height;
};
Node* newNode(int x)
{
    Node* p= new Node;
    p->left=p->right=NULL;
    p->height=1;
    p->key=x;
    return p;
}
Node* MinValueInRight(Node* root)
{   
    Node* temp=root;
    while(temp->left!=NULL)
    {
        temp=temp->left;
    }
    return temp;
}
int height(Node* x)
{
    if (x==NULL) return 0;
    return x->height;
}
Node* rigthrotate(Node* root)
{
    Node* temp=root->left;
    Node* temp2=temp->right;
    temp->right=root;
    root->left=temp2;
    temp->height=max(height(temp->left),height(temp->right))+1;
    root->height=max(height(root->left),height(root->right))+1;
    return temp;
}
Node* leftrotate(Node* root)
{   
    Node* temp=root->right;
    Node* temp2=temp->left;
    temp->left=root;
    root->right=temp2;
    temp->height=max(height(temp->left),height(temp->right))+1;
    root->height=max(height(root->left),height(root->right))+1;
    return temp;
}
int getBalance(Node* x)
{
    if (x==NULL) return 0;
    return height(x->left)-height(x->right);
}
Node* insert(Node* root,int key)
{
    if (root==NULL) return newNode(key);
    if (key<root->key)
    {
        root->left=insert(root->left,key);
    }
    else if (key>root->key)
    {
        root->right=insert(root->right,key);
    }
    else return root;
    root->height=1+max(height(root->left),height(root->right));
    if (getBalance(root)>1 && key<root->left->key)//left left case
    {
        return rigthrotate(root);
    }
    else if (getBalance(root)<-1 && key>root->right->key)//right right case
    {
        return leftrotate(root);
    }
    else if (getBalance(root)>1 && key>root->left->key)//left right case
    {
        root->left=leftrotate(root->left);
        return rigthrotate(root);
    }
    else if (getBalance(root)<-1 && key<root->right->key)//right left case
    {
        root->right=rigthrotate(root->right);
        return leftrotate(root);
    }
    return root;
}
void preorder(Node* root)
{
    if (root != NULL)
    {
        cout << root->key << " ";
        preorder(root->left);
        preorder(root->right);
    }
}
Node* deletion(Node* root,int key)
{
    if (root==NULL) return root;
    if (key<root->key)
    {
        root->left=deletion(root->left,key);
    }
    else if (key>root->key)
    {
        root->right=deletion(root->right,key);
    }
    else
    {
        if (root->left==NULL)
        {
            Node* temp=root->right;
            delete root;
            return temp;
        }
        else if (root->right==NULL)
        {
            Node* temp=root->left;
            delete root;
            return temp;
        }
        Node* temp=MinValueInRight(root->right);
        root->key=temp->key;
        root->right=deletion(root->right,temp->key);
    }
    if (root==NULL) return root;
    root->height=1+max(height(root->left),height(root->right));
    if (getBalance(root)>1 && getBalance(root->left)>=0)
    {
        return rigthrotate(root);
    }
    else if (getBalance(root)<-1 && getBalance(root->right)<=0)
    {
        return leftrotate(root);
    }
    else if (getBalance(root)>1 && getBalance(root->left)<0)
    {
        root->left=leftrotate(root->left);
        return rigthrotate(root);
    }
    else if (getBalance(root)<-1 && getBalance(root)>0)
    {
        root->right=rigthrotate(root->right);
        return leftrotate(root);
    }
    return root;
}
void preOrder(Node *root)
{
    if(root != NULL)
    {
        cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}
int main()
{
Node *root = NULL;
     
    /* Constructing tree given in
    the above figure */
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);
     
    /* The constructed AVL Tree would be
                30
            / \
            20 40
            / \ \
        10 25 50
    */
    cout << "Preorder traversal of the "
            "constructed AVL tree is \n";
    preOrder(root);
     
    return 0;
}