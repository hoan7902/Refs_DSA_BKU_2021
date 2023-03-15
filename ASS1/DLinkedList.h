#include<iostream>
using namespace std;
template <class T>
class DLinkedList 
{
public:
    class Node;     //forward declaration
    Node* head;
    Node* tail;
    int count;
public:
    DLinkedList() 
    {
        this->head=this->tail=nullptr;
        this->count=0;
    }
    ~DLinkedList()
    {
        this->tail=this->head=nullptr;
        delete this->tail;
        delete this->head;
    }
    void add(const T& e);
    void add(int index, const T& e);
    T removeAt(int index);
    bool removeItem(const T& removeItem);
    bool empty();
    int size();
    void clear();
    T get(int index);
    void set(int index, const T& e);
    int indexOf(const T& item);
    bool contains(const T& item);
    void toString();
    public:
    class Node
    {
    private:
        T data;
        Node *next;
        Node *previous;
        friend class DLinkedList<T>;

    public:
        Node()
        {
            this->previous = NULL;
            this->next = NULL;
        }

        Node(const T &data)
        {
            this->data = data;
            this->previous = NULL;
            this->next = NULL;
        }
    };
};
template<class T>
void DLinkedList<T>::toString()
{
    for (Node* k=this->head;k!=NULL;k=k->next)
    {
        cout<<k->data<<" ";
    }
}
template <class T>
void DLinkedList<T>::add(const T& e) {
    /* Insert an element into the end of the list. */
    Node* p= new Node(e);
    if (this->head == nullptr)
    {
        this->head = this->tail = p;
        this->count++;
    }
    else
    {
        this->tail->next = p;
        p->previous = this->tail;
        this->tail = p;
        this->count++;
    }
}
template<class T>
void DLinkedList<T>::add(int index, const T& e) {
    /* Insert an element into the list at given index. */
    Node* p = new Node(e);
    if (index <= 0 || this->head==NULL)
    {
        if (this->head == NULL)
        {
            this->head = this->tail = p;
            this->count++;
        }
        else
        {
            p->next = this->head;
            this->head->previous = p;
            this->head = p;
            this->count++;
        }
    }
    else if (index >= this->count)
    {
        add(e);
    }
    else
    {
        Node* vitri = this->head;
        int dem = 0;
        while (vitri != NULL)
        {
            if (dem == index) break;
            dem++;
            vitri = vitri->next;
        }
        Node* dau = this->head;
        while (dau->next != vitri) dau = dau->next;
        dau->next = p; p->previous = dau;
        p->next = vitri; vitri->previous = p;
        this->count++;
    }
}

template<class T>
int DLinkedList<T>::size() {
    /* Return the length (size) of list */
   int dem = 0;
    for (Node* k = this->head; k != NULL; k = k->next)
    {
        dem++;
    }
    return dem;
}
template<class T>
T DLinkedList<T>::get(int index) {
    /* Give the data of the element at given index in the list. */
    Node* k = this->head;
    int dem = 0;
    while (k != NULL)
    {
        if (dem == index) break;
        dem++; k = k->next;
    }
    return k->data;
}

template <class T>
void DLinkedList<T>::set(int index, const T& e) {
    /* Assign new value for element at given index in the list */
    Node* k = this->head;
    int dem = 0;
    while (k != NULL)
    {
        if (dem == index) break;
        dem++; k = k->next;
    }
    k->data = e;
}

template<class T>
bool DLinkedList<T>::empty() {
    /* Check if the list is empty or not. */
    if (this->count == 0) return true;
    return false;
}

template<class T>
int DLinkedList<T>::indexOf(const T& item) {
    /* Return the first index wheter item appears in list, otherwise return -1 */
    int index = 0;
    Node* k = this->head;
    while (k != NULL)
    {
        if (k->data == item) return index;
        index++;
        k = k -> next;
    }
    return -1;
}

template<class T>
bool DLinkedList<T>::contains(const T& item) {
    /* Check if item appears in the list */
    Node* k = this->head;
    while (k != NULL)
    {
        if (k->data == item) return true;
        k = k -> next;
    }
    return false;
}
template <class T>
T DLinkedList<T>::removeAt(int index)
{
    /* Remove element at index and return removed value */
    if (index <= 0)
    {
        Node* tam = this->head;
        this->head = this->head->next;
        this->head->previous=NULL;
        T res = tam->data;
        delete tam;
        this->count--;
        return res;
    }
    else if (index >= count - 1)
    {
        Node* xoa = this->tail;
        Node* cuoi = this->tail->previous;
        cuoi->next = NULL;
        this->tail = cuoi;
        xoa->previous = NULL;
        this->count--;
        T res = xoa->data;
        delete xoa;
        return res;
    }
   
        Node* vitri = this->head;
        int dem = 0;
        while (vitri != NULL)
        {
            if (dem == index) break;
            dem++;
            vitri = vitri->next;
        }
        Node* dau = this->head;
        while (dau->next != vitri) dau = dau->next;
        dau->next = vitri->next;
        (vitri->next)->previous = dau;
        T res = vitri->data;
        delete vitri;
        this->count--;
        return res;
}

template <class T>
bool DLinkedList<T>::removeItem(const T& item)
{
    /* Remove the first apperance of item in list and return true, otherwise return false */
    Node* k = this->head;
    int dem = 0;
    while (k != NULL)
    {
        if (k->data == item)
        {
            removeAt(dem);
            return true;
        }
        dem++;
        k = k->next;
    }
    return false;
}

template<class T>
void DLinkedList<T>::clear() {
    /* Remove all elements in list */
    while (this->head != NULL)
    {
        Node* tam = this->head;
        this->head = this->head->next;
        delete tam;
        this->count--;
    }
    //cout<<"delete successful"<<endl;
}
//phần của tg dênnnnnnnnnnnnnnnnnn
template <class T>
T DLinkedList<T>::removeAt(int index)
{
    /* Remove element at index and return removed value */
    if(index < 0 || index >= this->count) throw std::out_of_range ("Invalid!");
    if (index == 0) {
        Node* p = this->head;
        this->head = this->head->next;
        if(this->head!=nullptr) this->head->previous = nullptr;
        else this->tail = nullptr;
        this->count--;
        T data = p->data;
        delete p;
        return data;
    }
    if(index == this->count-1) {
        Node* p = this->tail;
        Node* q = this->tail->previous;
        q->next = nullptr;
        this->tail = q;
        T data = p->data;
        this->count--;
        delete p;
        return data;
    }
    Node* p = head;
    while (p!=nullptr) {
        if(index==0) {
            Node* q = p->previous;
            Node* r = p->next;
            q->next = r;
            r->previous = q;
            T data = p->data;
            this->count--;
            delete p;
            return data;
        }
        index--;
        p = p->next;
    }
    throw std::out_of_range ("Invalid!");
}

template <class T>
bool DLinkedList<T>::removeItem(const T& item)
{
    /* Remove the first apperance of item in list and return true, otherwise return false */
    int idx = 0;
    Node* p = this->head;
    while(p!=nullptr) {
        if(p->data==item) {
            if(idx==0) {
                Node* p = this->head;
                this->head = this->head->next;
                if(this->head!=nullptr) this->head->previous = nullptr;
                else this->tail = nullptr;
                this->count--;
                delete p;
                return true;
            }
            if (idx==this->count-1) {
                Node* p = this->tail;
                Node* q = this->tail->previous;
                if (q!=nullptr) q->next = nullptr;
                this->tail = q;
                this->count--;
                delete p;
                return true;
            }
            Node* q = p->previous;
            Node* r = p->next;
            q->next = r;
            r->previous = q;
            this->count--;
            delete p;
            return true;
        }
        idx++;
        p = p->next;
    }
    return false;
}

template<class T>
void DLinkedList<T>::clear(){
    /* Remove all elements in list */
    while(this->head!=nullptr) {
        Node* p = this->head;
        this->head = this->head->next;
        if (this->head!=nullptr) this->head->previous = nullptr;
        this->count--;
        delete p;
    }
    this->head = nullptr;
    this->tail = nullptr;
    this->count = 0;
}