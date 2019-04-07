#ifndef QUEUE_H
#define QUEUE_H
template<typename T>
struct LinkNode
{
    T data;
    LinkNode<T>*link;
    LinkNode(LinkNode<T>*p = 0) { link = p; }
    LinkNode(T&item, LinkNode<T>*p = 0) { data = item; link = p; }
};

template<class T>
class LinkedQueue {
    LinkNode<T>*front, *rear;
public:
    LinkedQueue() :front(0), rear(0) {}
    ~LinkedQueue();
    bool EnQueue(T x);
    bool DeQueue(T&x);
    T Getfront();
    T Getrear();
    void MakeEmpty();
    bool IsEmpty() { return front == 0; }
};

template<class T>
inline LinkedQueue<T>::~LinkedQueue()
{
    MakeEmpty();
}

template<class T>
inline bool LinkedQueue<T>::EnQueue(T x)
{
    if (front == 0) {
        front = rear = new LinkNode<T>(x);
        if (front == 0)return false;
    }
    else {
        rear->link = new LinkNode<T>(x);
        if (rear->link == 0)return false;
        rear = rear->link;
    }
    return true;
}

template<class T>
inline bool LinkedQueue<T>::DeQueue(T & x)
{
    if (IsEmpty() == true)return false;
    LinkNode<T>*p = front;
    x = front->data;
    front = front->link;
    delete p;
    return true;
}

template<class T>
inline T LinkedQueue<T>::Getfront()
{
    if (IsEmpty())return 0;
    return	front->data;
}

template<class T>
inline T LinkedQueue<T>::Getrear()
{
    if (IsEmpty())return 0;
    return rear->data;
}

template<class T>
inline void LinkedQueue<T>::MakeEmpty()
{
    LinkNode<T>*p = 0;
    while (front != 0) {
        p = front;
        front = front->link;
        delete p;
    }
}

#endif // QUEUE_H
