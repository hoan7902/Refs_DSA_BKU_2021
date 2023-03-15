#include<iostream>
#include<regex>
using namespace std;
int dem  = 0;
class SplayTree {
    struct Node {
        int val;
        Node* pLeft;
        Node* pRight;
        Node* pParent;
        Node(int val = 0, Node* l = nullptr, Node* r = nullptr, Node* par = nullptr) : val(val), pLeft(l), pRight(r), pParent(par) { }
    };
    Node* root;

    // print the tree structure for local testing
    void printBinaryTree(string prefix, const Node* root, bool isLeft, bool hasRightSibling) {
        if (!root && isLeft && hasRightSibling) {
            cout << prefix << "├──\n";
        }
        if (!root) return;
        cout << prefix;
        if (isLeft && hasRightSibling) 
            cout << "├──";
        else 
            cout << "└──";
        cout << root->val << '\n';
        printBinaryTree(prefix + (isLeft && hasRightSibling ? "|  " : "   "), root->pLeft, true, root->pRight);
        printBinaryTree(prefix + (isLeft && hasRightSibling ? "|  " : "   "), root->pRight, false, root->pRight);
    }

    void printPreorder(Node* p) {
        if (!p) {
            return;
        }
        cout << p->val << ' ';
        printPreorder(p->pLeft);
        printPreorder(p->pRight);
    }
public:
    SplayTree() {
        root = nullptr;
    }
    ~SplayTree() {
        // Ignore deleting all nodes in the tree
    }

    void printBinaryTree() {
        printBinaryTree("", root, false, false);
    }

    void printPreorder() {
        printPreorder(root);
        cout << "\n";
    }

    void right_Rotate(Node* x)
    {
       Node* y = x->pLeft;
       x->pLeft = y->pRight;
       if (y->pRight)
       {
           y->pRight->pParent = x;
       }
       y->pParent = x->pParent;
       if (x->pParent == NULL) this->root = y;
       else if (x->pParent->pLeft == x) x->pParent->pLeft = y;
       else x->pParent->pRight = y;
       y->pRight = x;
       x->pParent = y;
    }
    void left_Rotate(Node* x)
    {
        Node* y = x->pRight;
        x->pRight = y->pLeft;
        if (y->pLeft)
        {
            y->pLeft->pLeft = x;
        }
        y->pParent = x->pParent;
        if (x->pParent == NULL) this->root = y;
        else if (x->pParent->pLeft == x) x->pParent->pLeft = y;
        else x->pParent->pLeft = y;
        y->pLeft = x;
        x->pParent = y;
    }
    void splay(Node* p)
    {
        // To Do
        while (p->pParent)
        {
            if (p->pParent->pParent == NULL)
            {
                if (p->pParent->pLeft == p)
                {
                    right_Rotate(p);
                }
                else left_Rotate(p);
            }
            else 
            {
                Node* cha = p->pParent;
                Node* ongnoi = cha->pParent;
                if (p == cha->pLeft && cha == ongnoi->pLeft) // LL
                {
                    right_Rotate(ongnoi);
                    right_Rotate(cha);
                }
                else if (p ==  cha->pRight && cha == ongnoi->pRight) // RR
                {
                    left_Rotate(ongnoi);
                    left_Rotate(cha);
                }
                else if (p == cha->pLeft && cha == ongnoi->pRight) // RL
                {
                    right_Rotate(cha);
                    left_Rotate(ongnoi);
                }
                else //LR
                {
                    left_Rotate(cha);
                    right_Rotate(ongnoi);
                }
            }
        }
        
    }

    void insert(int val)
    {
        // To Do
        Node* n = new Node(val);
        Node* y = NULL;
        Node* temp = this->root;
        while (temp)
        {
            y = temp;
            if (temp->val > val) temp = temp->pLeft;
            else temp = temp->pRight;
        }
        n->pParent = y;
        if (y == NULL)
        {
            this->root = n;
        }
        if (n->val < y->val) y->pLeft = n;
        else y->pRight = n;
        splay(n);
    }
};
void reheapDown(vector<int> &maxHeap, int numberOfElements, int index)
{   
    if (index >= numberOfElements || index < 0) return;
    int max = index;
    int left = 2*index + 1;
    int right = 2*index + 2;
    if (left < numberOfElements && maxHeap[max] < maxHeap[left]) max = left;
    if (right < numberOfElements && maxHeap[max] < maxHeap[right]) max = right;
    if (max != index)
    {
        swap (maxHeap[max], maxHeap[index]);
        dem++;
        reheapDown(maxHeap, numberOfElements, max);
    }
}
void reheapUp(vector<int> &maxHeap, int numberOfElements, int index)
{   
    if (index >= numberOfElements || index < 0) return;
    if (index > 0)
    {
        int parent = (index - 1) / 2;
        if (maxHeap[parent] < maxHeap[index])
        {
            swap(maxHeap[parent], maxHeap[index]);
            reheapUp(maxHeap, numberOfElements, parent);
        }
    }
}
void buildHeap(vector<int> maxHeap, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        reheapDown(maxHeap, n, i);
    }
}
void insertHeap(vector<int> h, int value)
{
    h.push_back(value);
    int i = h.size() - 1;
    while (i > 0 && h[(i - 1) / 2] < h[i])
    {
        swap(h[(i - 1) / 2], h[i]);
        i = (i - 1) / 2;
    }
}
void pop(vector<int> &h)
{   
    if (h.size() == 0) return;
    h[0] = h[h.size() - 1];
    h.pop_back();
    reheapDown(h, h.size(), 0);
}
int main()
{   
    int n; cin >> n;
    double x = (n+1)/2;
    while (n < x*x)
    {
        x = x - 1;
    }
    cout<<x;
}