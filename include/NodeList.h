#ifndef NodeList_hpp
#define NodeList_hpp

template <typename T>
class NodeList
{
  private:
    struct Node
    {
        T elem;
        Node *prev;
        Node *next;
    };

  public:
    class Iterator
    {
      public:
        T& operator*();
        bool operator==(const Iterator &p) const;
        bool operator!=(const Iterator &p) const;
        Iterator& operator++();
        Iterator& operator--();
        Iterator operator++(int);
        Iterator operator--(int);
        friend class NodeList;
      protected:
        Node *v;
        Iterator(Node *u);
    };
    
    class ConstIterator : public NodeList<T>::Iterator
    {
      public:
        const T& operator*();
    };

  public:
    NodeList();
    int size() const;
    bool empty() const;
    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin();
    ConstIterator cend();
    void insertFront(const T &e);
    void insertBack(const T &e);
    void insert(const Iterator &p, const T &e);
    void eraseFront();
    void eraseBack();
    void erase(const Iterator &p);
    
    // housekeeping functions
    ~NodeList();
    NodeList(const NodeList&);
    NodeList& operator=(const NodeList&);
    
private:
    void clear();
    
  private:
    int n;
    Node *header;
    Node *trailer;
};

template <typename T>
NodeList<T>::NodeList():
n(0),
header(new Node),
trailer(new Node)
{
    header->next = trailer;
    trailer->prev = header;
}

template <typename T>
int NodeList<T>::size() const
{
    return n;
}

template <typename T>
bool NodeList<T>::empty() const
{
    return size() == 0;
}

template <typename T>
void NodeList<T>::insert(const Iterator &p, const T &e)
{
    //insert e before p
    Node* w = p.v;
    Node* u = w->prev;
    Node* v = new Node;
    v->elem = e;
    
    v->next = w;
    w->prev = v;
    
    v->prev = u;
    u->next = v;
    
    n++;
}

template <typename T>
void NodeList<T>::insertFront(const T &e)
{
    insert(begin(), e);
}

template <typename T>
void NodeList<T>::insertBack(const T &e)
{
    insert(end(), e);
}

template <typename T>
void NodeList<T>::erase(const Iterator &p)
{
    Node* v = p.v;
    Node* w = v->next;
    Node* u = v->prev;
    
    u->next = w;
    w->prev = u;
    
    delete v;
    
    n--;
}

template <typename T>
void NodeList<T>::eraseFront()
{
    erase(begin());
}

template <typename T>
void NodeList<T>::eraseBack()
{
    erase(end());
}

template <typename T>
typename NodeList<T>::Iterator NodeList<T>::begin()
{
    return Iterator(header->next);
}

template <typename T>
typename NodeList<T>::Iterator NodeList<T>::end()
{
    return Iterator(trailer);
}

template <typename T>
typename NodeList<T>::ConstIterator NodeList<T>::begin() const
{
    return ConstIterator(header->next);
}

template <typename T>
typename NodeList<T>::ConstIterator NodeList<T>::end() const
{
    return ConstIterator(trailer);
}

template <typename T>
typename NodeList<T>::ConstIterator NodeList<T>::cbegin()
{
    return ConstIterator(header->next);
}

template <typename T>
typename NodeList<T>::ConstIterator NodeList<T>::cend()
{
    return ConstIterator(trailer);
}

template <typename T>
void NodeList<T>::clear()
{
    while(!empty())
    {
        eraseFront();
    }
}

///
/// Housekeeping
///

template <typename T>
NodeList<T>::~NodeList()
{
    clear();
}

template <typename T>
NodeList<T>::NodeList(const NodeList& other):
NodeList()
{
    for(auto it = other.begin(); it != other.end(); ++it)
    {
        insertBack(*it);
    }
}

template <typename T>
NodeList<T>& NodeList<T>::operator=(const NodeList<T>& other)
{
    if(this != &other)
    {
        clear();
        for(auto it = other.begin(); it != other.end(); ++it)
        {
            insertBack(*it);
        }
    }
    return *this;
}

///
/// NodeList::Iterator
///

template <typename T>
NodeList<T>::Iterator::Iterator(Node* u)
{
    v = u;
}

template <typename T>
T& NodeList<T>::Iterator::operator*()
{
    return v->elem;
}

template <typename T>
bool NodeList<T>::Iterator::operator==(const Iterator& p) const
{
    return v == p.v;
}

template <typename T>
bool NodeList<T>::Iterator::operator!=(const Iterator& p) const
{
    return v != p.v;
}

template <typename T>
typename NodeList<T>::Iterator& NodeList<T>::Iterator::operator++()
{
    v = v->next;
    return *this;
}

template <typename T>
typename NodeList<T>::Iterator& NodeList<T>::Iterator::operator--()
{
    v = v->prev;
    return *this;
}

template <typename T>
typename NodeList<T>::Iterator NodeList<T>::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
typename NodeList<T>::Iterator NodeList<T>::Iterator::operator--(int)
{
    v = v->prev;
    return *this;
}

template <typename T>
const T& NodeList<T>::ConstIterator::operator*()
{
    return Iterator::v->elem;
}


#endif /* NodeList_hpp */